#include "LightGroup.h"

ID3D12Device* LightGroup::device = nullptr;


void LightGroup::StaticInitialize(ID3D12Device* device)
{
	assert(!LightGroup::device);
	assert(device);
	LightGroup::device = device;
}

LightGroup* LightGroup::Create()
{
	LightGroup* instance = new LightGroup();
	instance->Initialize();
	return instance;
}

void LightGroup::Initialize()
{
	HRESULT result;

	//ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(constBuff) + 0xff) & ~0xff;//256バイトアラインメント
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//定数バッファの生成
	result = device->CreateCommittedResource(
		&cbHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	assert(SUCCEEDED(result));

	TransferConstBuffer();
}

void LightGroup::Update()
{
	if (dirty)
	{
		TransferConstBuffer();
		dirty = false;
	}
}

void LightGroup::Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex)
{
	cmdList->SetGraphicsRootConstantBufferView(rootParameterIndex,
		constBuff->GetGPUVirtualAddress());
}

void LightGroup::TransferConstBuffer()
{
	HRESULT result;
	ConstBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result))
	{
		for (int i = 0; i < DirLightNum; i++)
		{
			if (dirLights[i].IsActive())
			{
				constMap->dirLights[i].active = 1;
				constMap->dirLights[i].lightv = dirLights[i].GetLightDir();
				constMap->dirLights[i].lightcolor = dirLights[i].GetLightColor();
			}
			else
			{
				constMap->dirLights[i].active = 0;
			}
		}
		for (int i = 0; i < PointLightNum; i++)
		{
			if (pointLights[i].IsActive())
			{
				constMap->pointLights[i].active = 1;
				constMap->pointLights[i].lightpos = 
					pointLights[i].GetLightPos();
				constMap->pointLights[i].lightcolor = 
					pointLights[i].GetLightColor();
				constMap->pointLights[i].lightatten = 
					pointLights[i].GetLightAtten();
			}
			else
			{
				constMap->pointLights[i].active = 0;
			}
		}
		for (int i = 0; i < SpotLightNum; i++)
		{
			if (spotLights[i].IsActive())
			{
				constMap->spotLights[i].active = 1;
				constMap->spotLights[i].lightv =
				{ -spotLights[i].GetLightDir().m128_f32[0],
					-spotLights[i].GetLightDir().m128_f32[1],
					-spotLights[i].GetLightDir().m128_f32[2],
					-spotLights[i].GetLightDir().m128_f32[3] };
				constMap->spotLights[i].lightpos =
					spotLights[i].GetLightPos();
				constMap->spotLights[i].lightcolor =
					spotLights[i].GetLightColor();
				constMap->spotLights[i].lightatten =
					spotLights[i].GetLightAtten();
				constMap->spotLights[i].lightfactoranglecos =
					spotLights[i].GetLightFactorAngleCos();
			}
			else
			{
				constMap->spotLights[i].active = 0;
			}
		}
		for (int i = 0; i < CircleShadowNum; i++)
		{
			if (circleShadows[i].IsActive())
			{
				constMap->circleShadows[i].active = 1;
				constMap->circleShadows[i].dir =
				{ -circleShadows[i].GetDir().m128_f32[0],
					-circleShadows[i].GetDir().m128_f32[1],
					-circleShadows[i].GetDir().m128_f32[2],
					-circleShadows[i].GetDir().m128_f32[3] };
				constMap->circleShadows[i].casterpos =
					circleShadows[i].GetCasterPos();
				constMap->circleShadows[i].distancecasterlight =
					circleShadows[i].GetDistanceCasterLight();
				constMap->circleShadows[i].atten =
					circleShadows[i].GetAtten();
				constMap->circleShadows[i].factoranglecos =
					circleShadows[i].GetFactorAngleCos();
			}
			else
			{
				constMap->circleShadows[i].active = 0;
			}
		}
	}
	constBuff->Unmap(0, nullptr);
}

void LightGroup::SetDirectionalLightActive(int index, bool active)
{
	assert(0 <= index && index < DirLightNum);

	dirLights[index].SetActive(active);
}

void LightGroup::SetDirectionalLightColor(int index, const XMFLOAT3& lightcolor)
{
	assert(0 <= index && index < DirLightNum);

	dirLights[index].SetLightColor(lightcolor);
	dirty = true;
}

void LightGroup::SetDirectionalLightDir(int index, const XMVECTOR& lightdir)
{
	assert(0 <= index && index < DirLightNum);

	dirLights[index].SetLightDir(lightdir);
	dirty = true;
}

void LightGroup::SetPointLightActive(int index, bool active)
{
	assert(0 <= index && index < PointLightNum);

	pointLights[index].SetActive(active);
}

void LightGroup::SetPointLightPos(int index, const XMFLOAT3& lightpos)
{
	assert(0 <= index && index < PointLightNum);

	pointLights[index].SetLightPos(lightpos);
	dirty = true;
}

void LightGroup::SetPointLightColor(int index, const XMFLOAT3& lightcolor)
{
	assert(0 <= index && index < PointLightNum);

	pointLights[index].SetLightColor(lightcolor);
	dirty = true;
}

void LightGroup::SetPointLightAtten(int index, const XMFLOAT3& lightatten)
{
	assert(0 <= index && index < PointLightNum);

	pointLights[index].SetLightAtten(lightatten);
	dirty = true;
}

void LightGroup::SetSpotLightActive(int index, bool active)
{
	assert(0 <= index && index < SpotLightNum);

	spotLights[index].SetActive(active);
}

void LightGroup::SetSpotLightDir(int index, const XMVECTOR& lightdir)
{
	assert(0 <= index && index < SpotLightNum);

	spotLights[index].SetLightDir(lightdir);
	dirty = true;
}

void LightGroup::SetSpotLightPos(int index, const XMFLOAT3& lightpos)
{
	assert(0 <= index && index < SpotLightNum);

	spotLights[index].SetLightPos(lightpos);
	dirty = true;
}

void LightGroup::SetSpotLightColor(int index, const XMFLOAT3& lightcolor)
{
	assert(0 <= index && index < SpotLightNum);

	spotLights[index].SetLightColor(lightcolor);
	dirty = true;
}

void LightGroup::SetSpotLightAtten(int index, const XMFLOAT3& lightatten)
{
	assert(0 <= index && index < SpotLightNum);

	spotLights[index].SetLightAtten(lightatten);
	dirty = true;
}

void LightGroup::SetSpotLightFactorAngle(int index, const XMFLOAT2& lightFactorAngle)
{
	assert(0 <= index && index < SpotLightNum);

	spotLights[index].SetLightFactorAngle(lightFactorAngle);
	dirty = true;
}

void LightGroup::SetCircleShadowActive(int index, bool active)
{
	assert(0 <= index && index < CircleShadowNum);

	circleShadows[index].SetActive(active);
}

void LightGroup::SetCircleShadowCasterPos(int index, const XMFLOAT3& lightpos)
{
	assert(0 <= index && index < CircleShadowNum);

	circleShadows[index].SetCasterPos(lightpos);
	dirty = true;
}

void LightGroup::SetCircleShadowDir(int index, const XMVECTOR& lightdir)
{
	assert(0 <= index && index < CircleShadowNum);

	circleShadows[index].SetDir(lightdir);
	dirty = true;
}

void LightGroup::SetCircleShadowDistanceCasterLight(int index, float distancecasterlight)
{
	assert(0 <= index && index < CircleShadowNum);

	circleShadows[index].SetDistanceCasterLight(distancecasterlight);
	dirty = true;
}

void LightGroup::SetCircleShadowAtten(int index, const XMFLOAT3& atten)
{
	assert(0 <= index && index < CircleShadowNum);

	circleShadows[index].SetAtten(atten);
	dirty = true;
}

void LightGroup::SetCircleShadowFactorAngle(int index, const XMFLOAT2& factorangle)
{
	assert(0 <= index && index < CircleShadowNum);

	circleShadows[index].SetFactorAngle(factorangle);
	dirty = true;
}
