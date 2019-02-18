#include "Vertex_Pos_UV_Normal.h"

Vertex_Pos_UV_Normal::Vertex_Pos_UV_Normal()
{
	_normal = { 0.0f, 0.0f, 0.0f };
}

Vertex_Pos_UV_Normal::Vertex_Pos_UV_Normal(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT2 UV, DirectX::XMFLOAT3 normal)
	:Vertex_Pos_UV(pos, UV)
{
	_normal = normal;
}

Vertex_Pos_UV_Normal::~Vertex_Pos_UV_Normal()
{
}

void Vertex_Pos_UV_Normal::setNorm(DirectX::XMFLOAT3 normal)
{
	_normal = normal;
}

DirectX::XMFLOAT3 Vertex_Pos_UV_Normal::getNorm() const
{
	return _normal;
}
