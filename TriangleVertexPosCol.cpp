#include "TriangleVertexPosCol.h"

TriangleVertexPosCol::TriangleVertexPosCol()
{
}

TriangleVertexPosCol::TriangleVertexPosCol(DirectX::XMVECTOR pos, DirectX::XMFLOAT3 col)
{
	_pos = pos;
	_col = col;
}

TriangleVertexPosCol::~TriangleVertexPosCol()
{
}

void TriangleVertexPosCol::setPos(DirectX::XMVECTOR pos)
{
	_pos = pos;
}

void TriangleVertexPosCol::setCol(DirectX::XMFLOAT3 col)
{
	_col = col;
}

DirectX::XMVECTOR TriangleVertexPosCol::getPos()
{
	return _pos;
}

DirectX::XMFLOAT3 TriangleVertexPosCol::getCol()
{
	return _col;
}
