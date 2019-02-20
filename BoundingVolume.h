#pragma once
#include <DirectXMath.h>
#include <Vertex_Pos_Col.h>
#include <vector>
#include <d3d11.h>

class BoundingVolume
{
private:
	DirectX::XMVECTOR _center;
	DirectX::XMMATRIX _worldMatrix;
	std::vector <Vertex_Pos_Col> _vertices;
	ID3D11Buffer* _vertexBuffer;
	bool _highlighted;

public:
	BoundingVolume();
	BoundingVolume(DirectX::XMVECTOR center, std::vector <Vertex_Pos_Col> vertices, ID3D11Device* device);
	~BoundingVolume();

	void setWorldMatrix(DirectX::XMMATRIX worldMatrix);
	void setCenter(DirectX::XMVECTOR center);
	void setHighlight(bool highlighted);

	ID3D11Buffer** getVertexBuffer();
	DirectX::XMVECTOR getCenter();
	std::vector <Vertex_Pos_Col> & getVertices();
	int getVertCount();

	virtual float intersectWithRay(DirectX::XMVECTOR rayDir, DirectX::XMVECTOR rayOrigin) = 0;
};