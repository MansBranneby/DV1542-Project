#pragma once
#include <DirectXMath.h>

class Material
{
private:
	DirectX::XMFLOAT3 _ambientCol;
	DirectX::XMFLOAT3 _diffuseCol;
	DirectX::XMFLOAT3 _specularCol;
	float _specularExp;
	float _padding[2];

public:
	Material();
	~Material();

	void setAmbientCol(DirectX::XMFLOAT3 ambientCol);
	void setDiffuseCol(DirectX::XMFLOAT3 diffuseCol);
	void setSpecularCol(DirectX::XMFLOAT3 specularCol);
	void setSpecularExp(float specularExp);
};