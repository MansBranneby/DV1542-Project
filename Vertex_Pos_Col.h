#pragma once
#include "Vertex.h"
#include <DirectXMath.h>

class Vertex_Pos_Col : public Vertex
{
private:
	DirectX::XMFLOAT3 _col;

public:
	Vertex_Pos_Col();
	Vertex_Pos_Col(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 col);
	~Vertex_Pos_Col();

	virtual void setCol(DirectX::XMFLOAT3 col);

	virtual DirectX::XMFLOAT3 getCol();
};