#include <transform>
#include <bones>
#include <fog>
#include <pos>
#include <normal>
#include <lights>
#include <texture>
#include <color>

struct vsOut
{
	float4 Pos: POSITION;
	float  Fog:	FOG;		
	float4 Ambient:COLOR;
	float2 Tex: TEXCOORD0;
};

vsOut bones_VS
(
	in float4 inPos : POSITION, 
	in float3 inNormal : NORMAL,
	in float2 inTex : TEXCOORD0,
	in int4 inBoneIndices : BLENDINDICES,
	in float4 inBoneWeights : BLENDWEIGHT
)
{
	vsOut Out;

	float4 Pos = DoBones(inPos,inBoneIndices,inBoneWeights);

	Out.Pos = DoTransform(Pos);
	Out.Tex	= DoTexture(inTex);
	Out.Fog	= DoFog(inPos);
	
	Out.Ambient = DoAmbient();

	float3 P = DoPos(Pos);
	float3 N = DoNormal(DoBones(inNormal,inBoneIndices,inBoneWeights));
	
	for (int i=0; i<iLights; i++)
		Out.Ambient.xyz += DoLight(P,N,i) * 0.5 * vecDiffuse;
		
	return Out;		
}

technique t
{
	pass p
	{
		VertexShader = compile vs_2_0 bones_VS();
	}
}

technique fallback
{
	pass
	{
	}
}