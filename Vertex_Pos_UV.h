#pragma once
#include <DirectXMath.h>

class Vertex_Pos_UV
{
private:
	DirectX::XMFLOAT3 _pos;
	DirectX::XMFLOAT2 _UV;

public:
	Vertex_Pos_UV();
	Vertex_Pos_UV(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT2 UV);
	~Vertex_Pos_UV();
};