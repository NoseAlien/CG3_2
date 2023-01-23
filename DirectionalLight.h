#pragma once
#include <d3dx12.h>
#include <wrl.h>
#include <DirectXMath.h>

class DirectionalLight
{
private:

	template <class T> using Comptr = Microsoft::WRL::ComPtr<T>;

	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	struct ConstBufferData
	{
		XMVECTOR lightv;
		XMFLOAT3 lightcolor;
	};

private:
	Comptr<ID3D12Resource> constBuff;
	XMVECTOR lightdir = { 1,0,0,0 };
	XMFLOAT3 lightcolor = { 1,1,1 };

public:
	void SetLightDir(const XMVECTOR& lightdir);
	inline XMVECTOR GetLightDir() { return lightdir; };
	inline void SetLightColor(const XMFLOAT3& lightcolor) { this->lightcolor = lightcolor; };
	inline XMFLOAT3 GetLightColor() { return lightcolor; };
};