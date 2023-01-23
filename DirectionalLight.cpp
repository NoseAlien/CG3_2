#include "DirectionalLight.h"

using namespace DirectX;

void DirectionalLight::SetLightDir(const XMVECTOR& lightdir)
{
	this->lightdir = XMVector3Normalize(lightdir);
}