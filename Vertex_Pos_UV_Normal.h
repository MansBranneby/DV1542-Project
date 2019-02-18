#pragma once
#include "Vertex_Pos_UV.h"
#include <DirectXMath.h>

class Vertex_Pos_UV_Normal : public Vertex_Pos_UV 
{
private:
	DirectX::XMFLOAT3 _normal;

public:
	Vertex_Pos_UV_Normal();
	Vertex_Pos_UV_Normal(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT2 UV, DirectX::XMFLOAT3 normal);
	~Vertex_Pos_UV_Normal();

	virtual void setNorm(DirectX::XMFLOAT3 normal);
	virtual DirectX::XMFLOAT3 getNorm() const;

};
