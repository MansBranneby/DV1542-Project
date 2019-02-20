#include "Vertex_Pos_UV.h"

Vertex_Pos_UV::Vertex_Pos_UV()
{
}

Vertex_Pos_UV::Vertex_Pos_UV(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT2 UV)
{
	_pos = pos;
	_UV = UV;
}

Vertex_Pos_UV::~Vertex_Pos_UV()
{
}
