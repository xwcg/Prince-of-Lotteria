// VARIABLES

	bool PASS_SOLID;

	float4x4 matWorld;
	float4x4 matWorldViewProj;

	float4 vecFog;
	float3 vecFogColor;

	float4 vecLightPos[8];
	float4 vecLightColor[8];

	float4 vecAmbient;
	float4 vecLight;
	
	float4 vecViewPos;
	float4 vecColor;
	
	float4 vecSpecular;
	float4 vecEmissive;

	float4 vecTime;

	// tilefactor, shift
	float4 vecSkill41; // .x tilefactor, .yz uv shift

	// turbulence
	float4 vecSkill45; // .x amplitude, .y speed, .z modulation
	
	float lavaFogHigh_flt;
	float lavaFogLow_flt;

// TEXTURES

	// base
	Texture entSkin1;
	sampler smpTBase = sampler_state {Texture = <entSkin1>; MipFilter = Linear;};

	// t0
	Texture entSkin2;
	sampler smpT1 = sampler_state {Texture = <entSkin2>; MipFilter = Linear;};

	// t1
	Texture entSkin3;
	sampler smpT2 = sampler_state {Texture = <entSkin3>; MipFilter = Linear;};

	// t2
	Texture entSkin4;
	sampler smpT3 = sampler_state {Texture = <entSkin4>; MipFilter = Linear;};


  	// mask
	Texture mtlSkin1;
	sampler smpMask = sampler_state {Texture = <mtlSkin1>; MipFilter = Linear; MagFilter = Linear; MinFilter = Linear;};
	

// VERTEXSHADER

	float calcLightFalloff (float4 lightPosition, float3 surfacePosition, float3 normal)
	{
		float falloff = 12;

		float3 lightRay = lightPosition.xyz - surfacePosition;
		float lightRayAngle = dot(normal, normalize(lightRay));

		if ((lightRayAngle >= 0) && (lightPosition.w > 0))
		{
			float lightIntensity = length(lightRay) / lightPosition.w;
			lightIntensity = (1 - step(1, lightIntensity)) * (1 - lightIntensity);

		   return saturate(lightRayAngle * falloff) * lightIntensity;
		}
		else
		   return(0);
	}

	struct VS_IN
	{
	    float4 Pos		 : POSITION;
	    float3 Normal  : NORMAL;
	    float2 Tex     : TEXCOORD0;
	    float3 Tangent : TEXCOORD2;
	};

	struct VS_OUT
	{
		float4 Pos : POSITION;
		float4 Color : COLOR0;
		float3 Tex : TEXCOORD0;
		float3 Fog : TEXCOORD1;
		float3 Diffuse1 : TEXCOORD2;
		float3 Diffuse2 : TEXCOORD3;
		float3 Diffuse3 : TEXCOORD4;
		float3 Normal : TEXCOORD5;
		float3 Look : TEXCOORD6;
	};

	struct PS_IN
	{
		float4 Color : COLOR0;
		float3 Tex : TEXCOORD0;
		float3 Fog : TEXCOORD1;
		float3 Diffuse1 : TEXCOORD2;
		float3 Diffuse2 : TEXCOORD3;
		float3 Diffuse3 : TEXCOORD4;
		float3 Normal : TEXCOORD5;
		float3 Look : TEXCOORD6;
	};

	VS_OUT VS (VS_IN In)
	{
		VS_OUT Out = (VS_OUT)0;

		Out.Pos = mul(In.Pos, matWorldViewProj);
		
		Out.Tex.xy = In.Tex;
		Out.Fog.x  = (1 - saturate((vecFog.y - Out.Pos.z) * vecFog.z));

		float3 pos = mul(In.Pos, matWorld).xyz;
		Out.Look = normalize(pos - vecViewPos.xyz);
		Out.Normal = normalize(mul(In.Normal, (float3x3)matWorld));

		Out.Diffuse1 = calcLightFalloff(vecLightPos[0], pos, Out.Normal) * vecLightColor[0].xyz;
		Out.Diffuse2 = calcLightFalloff(vecLightPos[1], pos, Out.Normal) * vecLightColor[1].xyz;
		Out.Diffuse3 = calcLightFalloff(vecLightPos[2], pos, Out.Normal) * vecLightColor[2].xyz;

		Out.Color = vecAmbient * vecLight;
		
		return Out;
	}



// PIXELSHADER

	float3 tex2D_5 (float2 meshCoord, float2 turb, float2 shift, float tile)
	{
		float4 mask = tex2D(smpMask, meshCoord);

		float2 texCoord = (meshCoord + turb + shift) * tile;

		float3 t0 = tex2D(smpTBase, texCoord).rgb;
		float3 t1 = tex2D(smpT1, texCoord).rgb;
		float3 t2 = tex2D(smpT2, texCoord).rgb;
		float3 t3 = tex2D(smpT3, texCoord).rgb;
		
		float tf = 0.5 + 0.5*sin(vecTime.w/20);
		float tf2 = 0.5 + 0.5*sin(vecTime.w/15);
		float tf3 = 0.5 + 0.5*sin(vecTime.w/10);
		
		float3 color = lerp(t0, t1, tf);
		       color = lerp(color, lerp(t0, t1, tf2), mask.b);
		       color = lerp(color, lerp(t1, t2, tf3), mask.g);
		       color = lerp(color, lerp(t2, t3, tf3), mask.r);
		       
		return(color);
	}

	// returns turbulence coord shift .xy
	float2 turbulence (float2 tex, float amplitude, float speed, float modulation)
	{
		float2 t;
		       t.x =  amplitude * sin(tex.y * modulation + speed);
		       t.y = -amplitude * sin(tex.x * modulation + speed);

		return(t);
	}

	float4 PS (PS_IN In): COLOR
	{
		// coord, uv shift
		float2 coord = In.Tex.xy;
		float2 uvShift = vecSkill41.yz;

		// turbulence
		float2 turb = 0;
		{
			float amplitude = vecSkill45.x;
			float speed = vecSkill45.y;
			float modulation = vecSkill45.z;

			float time_step = vecTime.w;

			turb = turbulence(coord, amplitude, speed * time_step, modulation);
		}

		// tiling
		float tileFactor = vecSkill41.x;

		float3 color = tex2D_5(coord, turb, uvShift, tileFactor);

		float hotness = (color.r + color.g + color.b) / 3;
		hotness = saturate(pow(hotness + 0.5, 4));

		float d = dot(In.Look.xyz, In.Normal.xyz) + 1;

		float edgeGlow = pow(d, 3) * 4;
		float surfaceGlow = 1 - edgeGlow;

		color += 0.1 * vecSpecular.rgb * (0.5 + In.Normal.y) * hotness;
		color += edgeGlow * 0.025;

		// dynamic lights (on cold)
		/*
			color += In.Diffuse1 * (1 - hotness);
			color += In.Diffuse2 * (1 - hotness);
		*/

		// fog (less on hot)
		{
			float fogDensity = In.Fog.x;
			color = lerp(color, vecFogColor, saturate(fogDensity - 0.5 * hotness));
		}

		return(float4(color, 1.0f));
	}



// TECHNIQUE

	technique t
	{
		pass p
		{
	      ZWriteEnable = true;
	      AlphaBlendEnable = false;

			VertexShader = compile vs_3_0 VS();
			PixelShader  = compile ps_3_0 PS();
		}
	}