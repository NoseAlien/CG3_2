#pragma once

#include <DirectXMath.h>

class CircleShadow
{
private://�G�C���A�X
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public://�T�u�N���X
	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		XMVECTOR dir;
		XMFLOAT3 casterpos;
		float distancecasterlight;
		XMFLOAT3 atten;
		float pad3;
		XMFLOAT2 factoranglecos;
		unsigned int active;
		float pad4;
	};

private://�����o�ϐ�
	ConstBufferData CBData;
	XMVECTOR dir = { 1,0,0,0 };
	XMFLOAT3 casterpos = { 0,0,0 };
	float distancecasterlight = 100.0f;
	XMFLOAT3 atten = { 0.5f,0.6f,0.0f };
	XMFLOAT2 factoranglecos = { 0.2f,0.5f };
	bool active = false;

public://�����o�ϐ�
	inline void SetDir(const XMVECTOR& dir)
	{
		this->dir = DirectX::XMVector3Normalize(dir);
	};
	inline XMVECTOR GetDir() { return dir; }
	inline void SetCasterPos(const XMFLOAT3& casterpos)
	{
		this->casterpos = casterpos;
	}
	inline const XMFLOAT3& GetCasterPos()
	{
		return casterpos;
	}
	inline void SetDistanceCasterLight(const float distancecasterlight)
	{
		this->distancecasterlight = distancecasterlight;
	}
	inline float GetDistanceCasterLight()
	{
		return distancecasterlight;
	}
	inline void SetAtten(const XMFLOAT3& atten)
	{
		this->atten = atten;
	}
	inline const XMFLOAT3& GetAtten()
	{
		return atten;
	}
	inline void SetFactorAngle(const XMFLOAT2& factorangle)
	{
		this->factoranglecos.x = cosf(DirectX::XMConvertToRadians(factorangle.x));
		this->factoranglecos.y = cosf(DirectX::XMConvertToRadians(factorangle.y));
	}
	inline const XMFLOAT2& GetFactorAngleCos() { return factoranglecos; }
	inline void SetActive(bool active) { this->active = active; }
	inline bool IsActive() { return active; }
};