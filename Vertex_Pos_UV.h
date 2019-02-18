#pragma once
#include "Vertex.h"
#include <DirectXMath.h>

class Vertex_Pos_UV : public Vertex
{
private:
	DirectX::XMFLOAT2 _UV;

public:
	Vertex_Pos_UV();
	Vertex_Pos_UV(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT2 UV);
	~Vertex_Pos_UV();

	virtual void setUV(DirectX::XMFLOAT2 UV);
	virtual DirectX::XMFLOAT2 getUV();
};