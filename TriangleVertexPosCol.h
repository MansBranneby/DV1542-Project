#pragma once
#include <DirectXMath.h>

class TriangleVertexPosCol
{
private:
	DirectX::XMVECTOR _pos;
	DirectX::XMFLOAT3 _col;

public:
	TriangleVertexPosCol();
	TriangleVertexPosCol(DirectX::XMVECTOR pos, DirectX::XMFLOAT3 col);
	~TriangleVertexPosCol();

	void setPos(DirectX::XMVECTOR pos);
	void setCol(DirectX::XMFLOAT3 col);

	DirectX::XMVECTOR getPos();
	DirectX::XMFLOAT3 getCol();
};