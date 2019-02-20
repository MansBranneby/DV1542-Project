#pragma once
#include <DirectXMath.h>

class Vertex_Pos_UV_Normal
{
private:
	DirectX::XMFLOAT3 _pos;
	DirectX::XMFLOAT2 _UV;
	DirectX::XMFLOAT3 _normal;

public:
	Vertex_Pos_UV_Normal();
	Vertex_Pos_UV_Normal(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT2 UV, DirectX::XMFLOAT3 normal);
	~Vertex_Pos_UV_Normal();

	DirectX::XMFLOAT3 getPos() const;
	DirectX::XMFLOAT2 getUV() const;
	DirectX::XMFLOAT3 getNorm() const;

};
