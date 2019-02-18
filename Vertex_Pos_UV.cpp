#include "Vertex_Pos_UV.h"

Vertex_Pos_UV::Vertex_Pos_UV()
{
}

Vertex_Pos_UV::Vertex_Pos_UV(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT2 UV)
	:Vertex(pos)
{
	_UV = UV;
}

Vertex_Pos_UV::~Vertex_Pos_UV()
{
}

void Vertex_Pos_UV::setUV(DirectX::XMFLOAT2 UV)
{
	_UV = UV;
}

DirectX::XMFLOAT2 Vertex_Pos_UV::getUV()
{
	return _UV;
}


