#pragma once
#include <DirectXMath.h>

class Vertex
{
private:
	DirectX::XMFLOAT3 _pos;

public:
	Vertex();
	Vertex(DirectX::XMFLOAT3 pos);
	virtual ~Vertex() = 0;

};