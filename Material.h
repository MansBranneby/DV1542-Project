#pragma once
#include <DirectXMath.h>

class Material
{
private:
	DirectX::XMFLOAT3 _ambientCol;
	float _padding1;
	DirectX::XMFLOAT3 _diffuseCol;
	float _padding2;
	DirectX::XMFLOAT3 _specularCol;
	float _padding3;

	float _specularExp;
	float _padding[3];

public:
	Material();
	~Material();

	void setAmbientCol(DirectX::XMFLOAT3 ambientCol);
	void setDiffuseCol(DirectX::XMFLOAT3 diffuseCol);
	void setSpecularCol(DirectX::XMFLOAT3 specularCol);
	void setSpecularExp(float specularExp);
};