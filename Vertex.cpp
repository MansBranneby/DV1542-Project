#include "Vertex.h"

Vertex::Vertex()
{
}

Vertex::Vertex(DirectX::XMFLOAT3 pos)
{
	_pos = pos;
}

Vertex::~Vertex()
{
}

void Vertex::setPos(DirectX::XMFLOAT3 pos)
{
	_pos = pos;
}

DirectX::XMFLOAT3 Vertex::getPos() const
{
	return _pos;
}