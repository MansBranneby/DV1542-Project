#include "Vertex_Pos_Col.h"

Vertex_Pos_Col::Vertex_Pos_Col()
{
}

Vertex_Pos_Col::Vertex_Pos_Col(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 col)
	:Vertex(pos)
{
	_col = col;
}

Vertex_Pos_Col::~Vertex_Pos_Col()
{
}

void Vertex_Pos_Col::setCol(DirectX::XMFLOAT3 col)
{
	_col = col;
}

DirectX::XMFLOAT3 Vertex_Pos_Col::getCol()
{
	return _col;
}