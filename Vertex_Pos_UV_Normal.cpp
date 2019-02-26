#include "Vertex_Pos_UV_Normal.h"

Vertex_Pos_UV_Normal::Vertex_Pos_UV_Normal()
{
	_pos = { 0.0f, 0.0f, 0.0f };
	_UV = { 0.0f, 0.0f };
	_normal = { 0.0f, 1.0f, 0.0f };
}

Vertex_Pos_UV_Normal::Vertex_Pos_UV_Normal(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT2 UV, DirectX::XMFLOAT3 normal)
{
	_pos = pos;
	_UV = UV;
	_normal = normal;
}

Vertex_Pos_UV_Normal::Vertex_Pos_UV_Normal(DirectX::XMFLOAT3 pos)
{
	_pos = pos;
	_UV = { 0.0f, 0.0f };
	_normal = { 0.0f, 0.0f, 0.0f };
}

Vertex_Pos_UV_Normal::~Vertex_Pos_UV_Normal()
{
}

void Vertex_Pos_UV_Normal::setPos(DirectX::XMFLOAT3 pos)
{
	_pos = pos;
}

void Vertex_Pos_UV_Normal::setUV(DirectX::XMFLOAT2 UV)
{
	_UV = UV;
}

void Vertex_Pos_UV_Normal::setNormal(DirectX::XMFLOAT3 normal)
{
	_normal = normal;
}

DirectX::XMFLOAT3 Vertex_Pos_UV_Normal::getPos() const
{
	return _pos;
}

DirectX::XMFLOAT2 Vertex_Pos_UV_Normal::getUV()  const
{
	return _UV;
}

DirectX::XMFLOAT3 Vertex_Pos_UV_Normal::getNorm() const
{
	return _normal;
}
