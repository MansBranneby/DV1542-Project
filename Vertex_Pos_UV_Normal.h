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

	void setPos(float x, float y, float z);
	void setUV(float u, float v);
	void setNorm(float normX, float normY, float normZ);
	void setAll(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT2 UV, DirectX::XMFLOAT3 normal);

	DirectX::XMFLOAT3 getPos();
	DirectX::XMFLOAT2 getUV();
	DirectX::XMFLOAT3 getNorm();

};
