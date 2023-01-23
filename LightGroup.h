#pragma once

#include "PointLight.h"
#include "DirectionalLight.h"
#include "SpotLight.h"

class LightGroup
{
private://�G�C���A�X
	template <class T> using Comptr = Microsoft::WRL::ComPtr<T>;

	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

private:
	static const int DirLightNum = 3;
	static const int PointLightNum = 3;
	static const int SpotLightNum = 3;

public://�T�u�N���X
	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		//�����̐F
		XMFLOAT3 ambientColor;
		float pad1;
		//���s�����p
		DirectionalLight::ConstBufferData dirLights[DirLightNum];
		//�_�����p
		PointLight::ConstBufferData pointLights[PointLightNum];
		//�X�|�b�g���C�g�p
		SpotLight::ConstBufferData pointLights[SpotLightNum];
	};

private:
	Comptr<ID3D12Resource> constBuff;

	DirectionalLight dirLights[DirLightNum];
	PointLight pointLights[PointLightNum];
	SpotLight spotLights[SpotLightNum];

	bool dirty = false;

public:
	void Initialize();
	void Update();
	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex);
	void TransferConstBuffer();

	void SetDirectionalLightActive(int index, bool active);
	void SetDirectionalLightColor(int index, const XMFLOAT3& lightcolor);
	void SetDirectionalLightDir(int index, const XMVECTOR& lightdir);

	void SetPointLightActive(int index, bool active);
	void SetPointLightPos(int index, const XMFLOAT3& lightpos);
	void SetPointLightColor(int index, const XMFLOAT3& lightcolor);
	void SetPointLightAtten(int index, const XMFLOAT3& lightatten);

	void SetSpotLightActive(int index, bool active);
	void SetSpotLightDir(int index, const XMVECTOR& lightdir);
	void SetSpotLightPos(int index, const XMFLOAT3& lightpos);
	void SetSpotLightColor(int index, const XMFLOAT3& lightcolor);
	void SetSpotLightAtten(int index, const XMFLOAT3& lightatten);
	void SetSpotLightFactorAngle(int index, const XMFLOAT2& lightFactorAngle);

private:
	static ID3D12Device* device;

public:
	static void StaticInitialize(ID3D12Device* device);
	static LightGroup* Create();
};