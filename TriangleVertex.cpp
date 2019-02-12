#include "TriangleVertex.h"

TriangleVertex::TriangleVertex()
{
	_pos = { 0.0f, 0.0f, 0.0f };
	_UV = { 0.0f, 0.0f };
	_normal = { 0.0f, 0.0f, 0.0f };
}

TriangleVertex::TriangleVertex(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT2 UV, DirectX::XMFLOAT3 normal)
{
	_pos = pos;
	_UV = UV;
	_normal = normal;
}

TriangleVertex::~TriangleVertex()
{
}

void TriangleVertex::setPos(float x, float y, float z)
{
	_pos = { x, y, z };
}

void TriangleVertex::setUV(float u, float v)
{
	_UV = { u, v };
}

void TriangleVertex::setNorm(float normX, float normY, float normZ)
{
	_normal = { normX, normY, normZ };
}

void TriangleVertex::setAll(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT2 UV, DirectX::XMFLOAT3 normal)
{
	_pos = pos;
	_UV = UV;
	_normal = normal;
}

DirectX::XMFLOAT3 TriangleVertex::getPos()
{
	return _pos;
}

DirectX::XMFLOAT2 TriangleVertex::getUV()
{
	return _UV;
}

DirectX::XMFLOAT3 TriangleVertex::getNorm()
{
	return _normal;
}
