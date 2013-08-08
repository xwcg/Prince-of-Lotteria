float4x4 matWorldViewProj;
float4 vecTime;
struct vsOut
{
	float4 outPos : POSITION;
	float2 outTex : TEXCOORD0;
	float4 outInfo : TEXCOORD1;
};

vsOut animTree_VS(
	uniform float scale,
	in float4 inPos: 	POSITION, 
	in float3 inNormal :	NORMAL,
	in float2 inTex: 	TEXCOORD0)
{
	vsOut Out;
	
	inPos.x += 0.2 * sin(vecTime.w + inPos.y);
	inPos.z += 0.5 * cos(vecTime.w + inPos.y);
	
	inPos.xyz += inNormal * scale;
	
	Out.outInfo = inPos;
	
	Out.outTex = inTex;
	Out.outPos = mul(inPos, matWorldViewProj);

	return Out;		
}

float4 ps(float2 tex : TEXCOORD0, float2 pos : TEXCOORD1) : COLOR0
{
	float3 color = 0.75 + 0.25 * sin(-vecTime.w + 1 * pos.y);
	
	return 0.2 * float4(color, 1) * saturate(0.125 * (pos.y + 20));
}
  
    technique alpha_test
    {
      pass p0
      {
		VertexShader = compile vs_2_0 animTree_VS(0.0);
		AlphaBlendEnable = true;
		//SrcBlend = One;
		//DestBlend = One;
      }
      pass p0
      {
		VertexShader = compile vs_2_0 animTree_VS(1.0);
		PixelShader = compile ps_2_0 ps();
		AlphaBlendEnable = true;
		SrcBlend = One;
		DestBlend = One;
      }
    }