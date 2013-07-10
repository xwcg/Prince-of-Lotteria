float4 vecTime;
float4 vecAmbient;
float fAlpha;

float4 ps(float2 tex : TEXCOORD0) : COLOR0
{
	float3 color = 0.75 + 0.25 * sin(0.25 * vecTime.w + 45 * tex.y);
	return float4(color * vecAmbient.rgb, tex.y * fAlpha);
}
  
technique alpha_test
{
	pass p0
	{
		PixelShader = compile ps_2_0 ps();
		AlphaBlendEnable = true;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
	}
}