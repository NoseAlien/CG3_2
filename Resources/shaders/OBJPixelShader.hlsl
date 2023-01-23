#include "OBJShaderHeader.hlsli"

Texture2D<float4> tex : register(t0);  // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);      // 0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
	float4 texcolor = tex.Sample(smp,input.uv);

	float4 shadecolor = {0,0,0,0};
	const float shininess = 4.0f;
	float3 eyedir = normalize(cameraPos - input.worldpos.xyz);

	for (int i = 0; i < DIRLIGHT_NUM; i++)
	{
		if (dirLights[i].active)
		{
			float3 dotlightnormal = dot(dirLights[i].lightv, input.normal);
			float3 reflect = normalize(-dirLights[i].lightv + 2 * dotlightnormal * input.normal);
			float3 ambient = m_ambient;
			float3 diffuse = dotlightnormal * m_diffuse;
			float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

			shadecolor.rgb += (ambient + diffuse + specular) * dirLights[i].lightcolor;
		}
	}

	//点光源
	for (i = 0; i < POINTLIGHT_NUM; i++)
	{
		if (pointLights[i].active)
		{
			float3 lightv = pointLights[i].lightpos - input.worldpos.xyz;
			float d = length(lightv);
			lightv = normalize(lightv);
			float atten = 1.0f / (pointLights[i].lightatten.x + pointLights[i].lightatten.y * d + 
				pointLights[i].lightatten.z * d * d);
			float3 dotlightnormal = dot(lightv, input.normal);
			float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
			float3 diffuse = dotlightnormal * m_diffuse;
			float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

			shadecolor.rgb += atten * (diffuse + specular) * pointLights[i].lightcolor;
		}
	}
	shadecolor.a = m_alpha;

	return shadecolor * texcolor;
}