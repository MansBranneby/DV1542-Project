#include "Material.h"

Material::Material()
{
	_ambientCol = { 0.0f, 0.0f, 0.0f, };
	_diffuseCol = { 0.0f, 0.0f, 0.0f, };
	_specularCol = { 0.0f, 0.0f, 0.0f, };
}

Material::~Material()
{
}

void Material::setMaterial(DirectX::XMFLOAT3 ambientCol, DirectX::XMFLOAT3 diffuseCol, DirectX::XMFLOAT3 specularCol)
{
	_ambientCol = ambientCol;
	_diffuseCol = diffuseCol;
	_specularCol = specularCol;


}
