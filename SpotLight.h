#pragma once

#include <DirectXMath.h>

class SpotLight
{
private://エイリアス
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public://サブクラス
	//定数バッファ用データ構造体
	struct ConstBufferData
	{
		XMVECTOR lightv;
		XMFLOAT3 lightpos;
		float pad1;
		XMFLOAT3 lightcolor;
		float pad2;
		XMFLOAT3 lightatten;
		float pad3;
		XMFLOAT2 lightfactoranglecos;
		unsigned int active;
		float pad4;
	};

private://メンバ変数
	ConstBufferData CBData;
	XMVECTOR lightdir = { 1,0,0,0 };
	XMFLOAT3 lightpos = { 0,0,0 };
	XMFLOAT3 lightcolor = { 1,1,1 };
	XMFLOAT3 lightatten = { 1.0f,1.0f,1.0f };
	XMFLOAT2 lightFactorAngleCos = { 0.5f,0.2f };
	bool active = false;

public://メンバ変数
	inline void SetLightDir(const XMVECTOR& lightdir)
	{
		this->lightdir = DirectX::XMVector3Normalize(lightdir);
	};
	inline XMVECTOR GetLightDir() { return lightdir; }
	inline void SetLightPos(const XMFLOAT3& lightpos)
	{
		this->lightpos = lightpos;
	}
	inline const XMFLOAT3& GetLightPos()
	{
		return lightpos;
	}
	inline void SetLightColor(const XMFLOAT3& lightcolor)
	{
		this->lightcolor = lightcolor;
	}
	inline const XMFLOAT3& GetLightColor()
	{
		return lightcolor;
	}
	inline void SetLightAtten(const XMFLOAT3& lightatten)
	{
		this->lightatten = lightatten;
	}
	inline const XMFLOAT3& GetLightAtten()
	{
		return lightatten;
	}
	inline void SetLightFactorAngle(const XMFLOAT2& lightFactorAngle)
	{
		this->lightFactorAngleCos.x = cosf(DirectX::XMConvertToRadians(lightFactorAngle.x));
		this->lightFactorAngleCos.y = cosf(DirectX::XMConvertToRadians(lightFactorAngle.y));
	}
	inline const XMFLOAT2& GetLightFactorAngleCos() { return lightFactorAngleCos; }
	inline void SetActive(bool active) { this->active = active; }
	inline bool IsActive() { return active; }
};