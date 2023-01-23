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
			constMap->dirLights[i].lightv = dirLights[i].GetLightDir();
			constMap->dirLights[i].lightcolor = dirLights[i].GetLightColor();
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
	}
	constBuff->Unmap(0, nullptr);
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
