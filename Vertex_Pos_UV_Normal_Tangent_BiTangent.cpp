#include "Vertex_Pos_UV_Normal_Tangent_BiTangent.h"

Vertex_Pos_UV_Normal_Tangent_BiTangent::Vertex_Pos_UV_Normal_Tangent_BiTangent()
{
	_pos = { 0.0f, 0.0f, 0.0f };
	_UV = { 0.0f, 0.0f };
	_normal = { 0.0f, 0.0f, 0.0f };
	_tangent = { 0.0f, 0.0f, 0.0f };
	_biTangent = { 0.0f, 0.0f, 0.0f };
}

Vertex_Pos_UV_Normal_Tangent_BiTangent::Vertex_Pos_UV_Normal_Tangent_BiTangent(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT2 UV, DirectX::XMFLOAT3 normal, int vertIndex)
{
	_pos = pos;
	_UV = UV;
	_normal = normal;
	_tangent = { 0.0f, 0.0f, 0.0f };
	_biTangent = { 0.0f, 0.0f, 0.0f };
	_vertIndex = vertIndex;
}

Vertex_Pos_UV_Normal_Tangent_BiTangent::~Vertex_Pos_UV_Normal_Tangent_BiTangent()
{
}

void Vertex_Pos_UV_Normal_Tangent_BiTangent::setTangent(DirectX::XMFLOAT3 tangent)
{
	_tangent = tangent;
}

void Vertex_Pos_UV_Normal_Tangent_BiTangent::setBiTangent(DirectX::XMFLOAT3 biTangent)
{
	_biTangent = biTangent;
}

DirectX::XMFLOAT3 Vertex_Pos_UV_Normal_Tangent_BiTangent::getTangent() const
{
	return _tangent;
}

DirectX::XMFLOAT3 Vertex_Pos_UV_Normal_Tangent_BiTangent::getBiTangent() const
{
	return _biTangent;
}

DirectX::XMFLOAT3 Vertex_Pos_UV_Normal_Tangent_BiTangent::getPos() const
{
	return _pos;
}

DirectX::XMFLOAT2 Vertex_Pos_UV_Normal_Tangent_BiTangent::getUV() const
{
	return _UV;
}

DirectX::XMFLOAT3 Vertex_Pos_UV_Normal_Tangent_BiTangent::getNorm() const
{
	return _normal;
}

int Vertex_Pos_UV_Normal_Tangent_BiTangent::getVertIndex() const
{
	return _vertIndex;
}
