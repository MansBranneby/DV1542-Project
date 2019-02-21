#pragma once
#include <DirectXMath.h>

class Vertex_Pos_UV_Normal_Tangent_BiTangent
{
private:
	DirectX::XMFLOAT3 _pos;
	DirectX::XMFLOAT2 _UV;
	DirectX::XMFLOAT3 _normal;
	DirectX::XMFLOAT3 _tangent;
	DirectX::XMFLOAT3 _biTangent;
	int _vertIndex;

public:
	Vertex_Pos_UV_Normal_Tangent_BiTangent();
	Vertex_Pos_UV_Normal_Tangent_BiTangent(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT2 UV, DirectX::XMFLOAT3 normal, int vertIndex);
	~Vertex_Pos_UV_Normal_Tangent_BiTangent();

	void setTangent(DirectX::XMFLOAT3 tangent);
	void setBiTangent(DirectX::XMFLOAT3 biTangent);

	DirectX::XMFLOAT3 getTangent() const;
	DirectX::XMFLOAT3 getBiTangent() const;

	DirectX::XMFLOAT3 getPos() const;
	DirectX::XMFLOAT2 getUV() const;
	DirectX::XMFLOAT3 getNorm() const;
	int getVertIndex() const;
};