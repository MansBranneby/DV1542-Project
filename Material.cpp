#include "Material.h"

Material::Material()
{
	_ambientCol = { 1.0f, 1.0f, 1.0f, };
	_diffuseCol = { 0.5f, 0.5f, 0.5f, };
	_specularCol = { 0.5f, 0.5f, 0.5f, };
	_specularExp = 96.0f;
	_padding[0] = 0.0f;
	_padding[1] = 96.0f;
}

Material::~Material()
{
}

void Material::setAmbientCol(DirectX::XMFLOAT3 ambientCol)
{
	_ambientCol = ambientCol;
}

void Material::setDiffuseCol(DirectX::XMFLOAT3 diffuseCol)
{
	_diffuseCol = diffuseCol;
}

void Material::setSpecularCol(DirectX::XMFLOAT3 specularCol)
{
	_specularCol = specularCol;
}

void Material::setSpecularExp(float specularExp)
{
	_specularExp = specularExp;
}
