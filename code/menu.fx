float4 vecViewPort;
float4 vecTime;
float g_menuFxSpeed_var;

float4 PS (in float2 vpos : VPOS) : COLOR0
{
	float3 color = 0;
	
	float2 pos = vpos.xy * vecViewPort.zw;
	float3 time = g_menuFxSpeed_var * vecTime.w * float3(1.2, -0.9, 1.0);
	
	color.rgb = 
		0.5 + 
		0.5 * sin(time + float3(0, 1, 2) + float3(10, -10, 5) * pos.y) * 
		cos(time + float3(3, 2, 1) - float3(-6, 8, -2) * pos.x);
	
	color.rgb = pow(0.5 + color.rgb, 2) - 0.5;
	
	return float4(color, 1);
}

technique
{
	pass
	{
		PixelShader = compile ps_3_0 PS();
	}
}