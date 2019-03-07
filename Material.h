#pragma once
#include <DirectXMath.h>

class Material
{
private:
	DirectX::XMFLOAT3 _ambientCol;
	DirectX::XMFLOAT3 _diffuseCol;
	DirectX::XMFLOAT3 _specularCol;

public:
	Material();
	~Material();

	void setMaterial(DirectX::XMFLOAT3 _ambientCol, DirectX::XMFLOAT3 _diffuseCol, DirectX::XMFLOAT3 _specularCol);
};