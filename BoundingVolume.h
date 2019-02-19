#pragma once
#include <DirectXMath.h>
#include <Vertex_Pos_Col.h>
#include <vector>
class BoundingVolume
{
private:
	DirectX::XMVECTOR _center;
	DirectX::XMMATRIX _worldMatrix;
	std::vector <Vertex_Pos_Col> _vertices;
	bool _highlighted;

public:
	BoundingVolume();
	BoundingVolume(DirectX::XMVECTOR center, std::vector <Vertex_Pos_Col> vertices);
	~BoundingVolume();

	void setWorldMatrix(DirectX::XMMATRIX worldMatrix);
	void setCenter(DirectX::XMVECTOR center);
	void setHighlight(bool highlighted);

	DirectX::XMVECTOR getCenter();
	std::vector <Vertex_Pos_Col> & getVertices();
	int getVertCount();

	virtual float intersectWithRay(DirectX::XMVECTOR rayDir, DirectX::XMVECTOR rayOrigin) = 0;
};