#include "Vertex_Pos_Col.h"

Vertex_Pos_Col::Vertex_Pos_Col()
{
	_pos = { 0.0f, 0.0f, 0.0f };
	_col = { 0.0f, 0.0f, 0.0f };
}

Vertex_Pos_Col::Vertex_Pos_Col(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 col)
{
	_pos = pos;
	_col = col;
}

Vertex_Pos_Col::~Vertex_Pos_Col()
{
}

void Vertex_Pos_Col::setPos(DirectX::XMFLOAT3 pos)
{
	_pos = pos;
}

void Vertex_Pos_Col::setCol(DirectX::XMFLOAT3 col)
{
	_col = col;
}

DirectX::XMFLOAT3 Vertex_Pos_Col::getPos()
{
	return _pos;
}

DirectX::XMFLOAT3 Vertex_Pos_Col::getCol()
{
	return _col;
}
