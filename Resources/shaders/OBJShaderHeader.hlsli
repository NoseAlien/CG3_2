static const int POINTLIGHT_NUM = 3;
static const int DIRLIGHT_NUM = 3;

struct DirLight
{
	float3 lightv;
	float3 lightcolor;
	uint active;
};

struct PointLight
{
	float3 lightpos;
	float3 lightcolor;
	float3 lightatten;
	uint active;
};

cbuffer cbuff0 : register(b0)
{
	matrix viewproj;
	matrix world;
	float3 cameraPos;
};

cbuffer cbuff1 : register(b1)
{
	float3 m_ambient  : packoffset(c0); // アンビエント係数
	float3 m_diffuse  : packoffset(c1); // ディフューズ係数
	float3 m_specular : packoffset(c2); // スペキュラー係数
	float m_alpha : packoffset(c2.w);	// アルファ
}

cbuffer cbuff2 : register(b2)
{
	float3 ambientColor;
	DirLight dirLights[DIRLIGHT_NUM];
	PointLight pointLights[POINTLIGHT_NUM];
};

// 頂点シェーダーからピクセルシェーダーへのやり取りに使用する構造体
struct VSOutput
{
	float4 svpos : SV_POSITION; // システム用頂点座標
	float4 worldpos : POSITION;
	float3 normal :NORMAL; // 法線ベクトル
	//float4 color :COLOR;
	float2 uv  :TEXCOORD; // uv値
};
