#include "OBJShaderHeader.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	float3 lightdir = float3(1, -1, 1);
	lightdir = normalize(lightdir);

	float3 lightcolor = float3(1, 1, 1);

	VSOutput output; // ピクセルシェーダーに渡す値
	output.svpos = mul(mat, pos);

	output.color.rgb = dot(-lightcolor, normal) * m_diffuse * lightcolor;
	output.color.a = m_alpha;
	output.uv = uv;

	return output;
}