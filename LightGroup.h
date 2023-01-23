#pragma once

#include "PointLight.h"
#include "DirectionalLight.h"

class LightGroup
{
private://エイリアス
	template <class T> using Comptr = Microsoft::WRL::ComPtr<T>;

	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

private:
	static const int DirLightNum = 1;
	static const int PointLightNum = 3;

public://サブクラス
	//定数バッファ用データ構造体
	struct ConstBufferData
	{
		//環境光の色
		XMFLOAT3 ambientColor;
		float pad1;
		//平行光源用
		DirectionalLight::ConstBufferData dirLights[DirLightNum];
		//点光源用
		PointLight::ConstBufferData pointLights[PointLightNum];
	};

private:
	Comptr<ID3D12Resource> constBuff;

	DirectionalLight dirLights[DirLightNum];
	PointLight pointLights[PointLightNum];

	bool dirty = false;

public:
	void Initialize();
	void Update();
	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex);
	void TransferConstBuffer();

	void SetDirectionalLightColor(int index, const XMFLOAT3& lightcolor);
	void SetDirectionalLightDir(int index, const XMVECTOR& lightdir);

	void SetPointLightActive(int index, bool active);
	void SetPointLightPos(int index, const XMFLOAT3& lightpos);
	void SetPointLightColor(int index, const XMFLOAT3& lightcolor);
	void SetPointLightAtten(int index, const XMFLOAT3& lightatten);

private:
	static ID3D12Device* device;

public:
	static void StaticInitialize(ID3D12Device* device);
	static LightGroup* Create();
};