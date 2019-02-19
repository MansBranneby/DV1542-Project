#include "Vertex_Pos_UV_Normal.h"

Vertex_Pos_UV_Normal::Vertex_Pos_UV_Normal()
{
	_pos = { 0.0f, 0.0f, 0.0f };
	_UV = { 0.0f, 0.0f };
	_normal = { 0.0f, 0.0f, 0.0f };
}

Vertex_Pos_UV_Normal::Vertex_Pos_UV_Normal(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT2 UV, DirectX::XMFLOAT3 normal)
{
	_pos = pos;
	_UV = UV;
	_normal = normal;
}

Vertex_Pos_UV_Normal::~Vertex_Pos_UV_Normal()
{
}

void Vertex_Pos_UV_Normal::setPos(float x, float y, float z)
{
	_pos = { x, y, z };
}

void Vertex_Pos_UV_Normal::setUV(float u, float v)
{
	_UV = { u, v };
}

void Vertex_Pos_UV_Normal::setNorm(float normX, float normY, float normZ)
{
	_normal = { normX, normY, normZ };
}

void Vertex_Pos_UV_Normal::setAll(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT2 UV, DirectX::XMFLOAT3 normal)
{
	_pos = pos;
	_UV = UV;
	_normal = normal;
}

DirectX::XMFLOAT3 Vertex_Pos_UV_Normal::getPos()
{
	return _pos;
}

DirectX::XMFLOAT2 Vertex_Pos_UV_Normal::getUV()
{
	return _UV;
}

DirectX::XMFLOAT3 Vertex_Pos_UV_Normal::getNorm()
{
	return _normal;
}
