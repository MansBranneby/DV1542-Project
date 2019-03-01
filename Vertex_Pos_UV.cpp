#include "Vertex_Pos_UV.h"

Vertex_Pos_UV::Vertex_Pos_UV()
{
	_pos = { 0.0f, 0.0f, 0.0f };
	_UV = { 0.0f, 0.0f };
}

Vertex_Pos_UV::Vertex_Pos_UV(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT2 UV)
{
	_pos = pos;
	_UV = UV;
}

Vertex_Pos_UV::~Vertex_Pos_UV()
{
}

void Vertex_Pos_UV::setPos(DirectX::XMFLOAT3 pos)
{
	_pos = pos;
}

DirectX::XMFLOAT3 Vertex_Pos_UV::getPos() const
{
	return _pos;
}
