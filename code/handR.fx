const float specularBaseStrength = 0.15;
const float specularBaseLightPower = 4;

float fx_obj_lightBrightness = 1.75;
float fx_obj_diffuseLightStrength = 1.25;
float fx_obj_specularLightStrength = 1.0;
float fx_obj_specularLightPower = 4;

float fx_obj_alpharef = 0.5;

float4x4 matWorldViewProj;
float4x4 matWorld;

float4 vecColor;
float4 vecViewPos;
float4 vecLightPos[8];
float4 vecLightDir[8];
float4 vecLightColor[8];
float fAmbient;
float4 vecLight;
float fAlbedo;
float fPower;
float4 vecSunColor;
float4 vecTime;

float4 vecAmbient;
float4 vecDiffuse;
float4 vecSpecular;
float4 vecEmissive;

texture entSkin1;
sampler smpColor = sampler_state { Texture = <entSkin1>; };

struct VS_OUT
{
	float4 Pos : POSITION;
	float3 Texcoord_Spec : TEXCOORD0; // xy = tex coord, z = spec power
	float4 wPos : TEXCOORD1;
	float3 Normal : TEXCOORD2;
	float3 ViewDir : TEXCOORD3;
	float3 SpecAlphaData: TEXCOORD4; // .x = spec power, .y = spec strength, .z = alpha ref
	float3 lPos : TEXCOORD5;
};

VS_OUT VS (uniform float scale, in float4 Pos : POSITION, in float3 Normal : NORMAL, in float2 Tex : TEXCOORD0)
{
	VS_OUT Out = (VS_OUT)0;
	
	float3 nuPos = Pos.xyz + Normal * scale;
	
	Out.lPos = nuPos;
	
	Out.Pos = mul(float4(nuPos, Pos.w), matWorldViewProj);
	
	Out.Texcoord_Spec.xy = Tex;
	
	Out.wPos.xyz = mul(float4(nuPos, 1), (float4x4)matWorld).xyz;	
	
	Out.ViewDir = vecViewPos - Out.wPos;
	Out.wPos.w = Out.Pos.z;
	
	Out.Normal = normalize(mul(Normal, (float3x3)matWorld));
	
	Out.SpecAlphaData.x = specularBaseLightPower * fx_obj_specularLightPower * 4; // spec power
	Out.SpecAlphaData.y = specularBaseStrength * fx_obj_specularLightStrength; // spec strength
	
	return Out;
}
	
struct Lighting
{
	float3 Diffuse;
	float3 Specular;
};

Lighting calcLighting (float3 wsPos, float3 wsNormal, float3 viewDir, float brightness, float specular, float diffuseStrength, float specularStrength)
{
	Lighting Out = (Lighting)0;
	
	float dott;
	float attenuation;
	float dist;
	float3 isPointSunSpot; // x = point, y = sun
	
	float3 surfacePos;
	
	float3 lightColor;
	float3 lightDir;
	
	float cosAlpha;
	
	for (int i = 0; i < 1; i++)
	{
		if (vecLightPos[i].w == 0)
			continue;
			
		isPointSunSpot.x = 1 - sign(vecLightDir[i].w); // point
		isPointSunSpot.y = step(10000, vecLightDir[i].w); // sun
		
		surfacePos = lerp(wsPos, 0, isPointSunSpot.y);
	
		lightColor = vecLightColor[i].rgb * brightness;
		lightDir = normalize(vecLightPos[i].xyz - surfacePos);
		
		dist = distance(vecLightPos[i].xyz, surfacePos);
		
		cosAlpha = max(0, dot(lightDir, -normalize(vecLightDir[i].xyz)));
		attenuation = lerp(1 - saturate(dist / vecLightPos[i].w), 1, isPointSunSpot.y);
		
		lightColor *= attenuation;
		
		Out.Diffuse += saturate(dot(wsNormal, lightDir)) * lightColor;
		Out.Specular += pow(saturate(dot(wsNormal, normalize(lightDir + viewDir))), specular) * lightColor;
	}
	
	Out.Diffuse *= diffuseStrength;
	Out.Specular *= specularStrength;
	
	return Out;
}

float4 PS (VS_OUT In): COLOR
{
	float4 diffuse = tex2D(smpColor, In.Texcoord_Spec.xy);
	
	float3 wsNormal = In.Normal.xyz;
	
	Lighting L = calcLighting(In.wPos.xyz, wsNormal, In.ViewDir, fx_obj_lightBrightness, In.SpecAlphaData.x, fx_obj_diffuseLightStrength, In.SpecAlphaData.y);
	
	// add entity color
	diffuse.rgb += (vecColor.rgb * 2 - 1);
	
	float4 color = float4((vecLight.rgb * vecSunColor.rgb + 0.25f) * diffuse.rgb * (1.f - saturate(L.Diffuse)) + diffuse.rgb * L.Diffuse + L.Specular + L.Specular, diffuse.a);
	
	return color;
}

technique t
{
	pass p1
	{
		//CullMode = None;
		
		VertexShader = compile vs_3_0 VS(0.0);
		PixelShader = compile ps_3_0 PS();
		
		AlphaBlendEnable = true;
	}
}

technique fallback
{
	pass one
	{
	}
}