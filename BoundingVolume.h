#pragma once
#include <DirectXMath.h>
#include <TriangleVertexPosCol.h>
#include <vector>
class BoundingVolume
{
private:
	DirectX::XMVECTOR _center;
	DirectX::XMMATRIX _worldMatrix;
	std::vector <TriangleVertexPosCol> _vertices;

public:
	BoundingVolume();
	BoundingVolume(DirectX::XMVECTOR center, std::vector <TriangleVertexPosCol> vertices);
	~BoundingVolume();

	void setWorldMatrix(DirectX::XMMATRIX worldMatrix);
	void setCenter(DirectX::XMVECTOR center);

	DirectX::XMVECTOR getCenter();
	std::vector <TriangleVertexPosCol> & getVertices();

	virtual float intersectWithRay(DirectX::XMVECTOR rayDir, DirectX::XMVECTOR rayOrigin) = 0;
};