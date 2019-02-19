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

	void setCol(DirectX::XMFLOAT3 col);

	DirectX::XMFLOAT3 getCol();
};