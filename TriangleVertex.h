#pragma once
#include <DirectXMath.h>

class TriangleVertex
{
private:
	DirectX::XMFLOAT3 _pos;
	DirectX::XMFLOAT2 _UV;
	DirectX::XMFLOAT3 _normal;

public:
	TriangleVertex();
	TriangleVertex(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT2 UV, DirectX::XMFLOAT3 normal);
	~TriangleVertex();

	void setPos(float x, float y, float z);
	void setUV(float u, float v);
	void setNorm(float normX, float normY, float normZ);
	void setAll(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT2 UV, DirectX::XMFLOAT3 normal);

	DirectX::XMFLOAT3 getPos();
	DirectX::XMFLOAT2 getUV();
	DirectX::XMFLOAT3 getNorm();

};
