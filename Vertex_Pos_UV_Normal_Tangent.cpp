#include "Vertex_Pos_UV_Normal_Tangent.h"

Vertex_Pos_UV_Normal_Tangent::Vertex_Pos_UV_Normal_Tangent()
{
	_pos = { 0.0f, 0.0f, 0.0f };
	_UV = { 0.0f, 0.0f };
	_normal = { 0.0f, 0.0f, 0.0f };
	_tangent = { 0.0f, 0.0f, 0.0f };

}

Vertex_Pos_UV_Normal_Tangent::Vertex_Pos_UV_Normal_Tangent(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT2 UV, DirectX::XMFLOAT3 normal, int vertIndex)
{
	_pos = pos;
	_UV = UV;
	_normal = normal;
	_tangent = { 0.0f, 0.0f, 0.0f };
	_vertIndex = vertIndex;
}

Vertex_Pos_UV_Normal_Tangent::~Vertex_Pos_UV_Normal_Tangent()
{
}

void Vertex_Pos_UV_Normal_Tangent::setTangent(DirectX::XMFLOAT3 tangent)
{
	_tangent = tangent;
}

DirectX::XMFLOAT3 Vertex_Pos_UV_Normal_Tangent::getTangent() const
{
	return _tangent;
}

DirectX::XMFLOAT3 Vertex_Pos_UV_Normal_Tangent::getPos() const
{
	return _pos;
}

DirectX::XMFLOAT2 Vertex_Pos_UV_Normal_Tangent::getUV() const
{
	return _UV;
}

DirectX::XMFLOAT3 Vertex_Pos_UV_Normal_Tangent::getNor() const
{
	return _normal;
}

int Vertex_Pos_UV_Normal_Tangent::getVertIndex() const
{
	return _vertIndex;
}
