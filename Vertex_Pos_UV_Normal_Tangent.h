#pragma once
#include <DirectXMath.h>

class Vertex_Pos_UV_Normal_Tangent
{
private:
	DirectX::XMFLOAT3 _pos;
	DirectX::XMFLOAT2 _UV;
	DirectX::XMFLOAT3 _normal;
	DirectX::XMFLOAT3 _tangent;
	int _vertIndex;

public:
	Vertex_Pos_UV_Normal_Tangent();
	Vertex_Pos_UV_Normal_Tangent(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT2 UV, DirectX::XMFLOAT3 normal, int vertIndex);
	~Vertex_Pos_UV_Normal_Tangent();

	void setPos(DirectX::XMFLOAT3 pos);
	void setTangent(DirectX::XMFLOAT3 tangent);

	DirectX::XMFLOAT3 getTangent() const;
	DirectX::XMFLOAT3 getPos() const;
	DirectX::XMFLOAT2 getUV() const;
	DirectX::XMFLOAT3 getNor() const;
	int getVertIndex() const;
};