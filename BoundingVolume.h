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
	bool _highlighted;

public:
	BoundingVolume();
	BoundingVolume(DirectX::XMVECTOR center, std::vector <TriangleVertexPosCol> vertices);
	~BoundingVolume();

	void setWorldMatrix(DirectX::XMMATRIX worldMatrix);
	void setCenter(DirectX::XMVECTOR center);
	void setHighlight(bool highlighted);

	DirectX::XMVECTOR getCenter();
	std::vector <TriangleVertexPosCol> & getVertices();
	int getVertCount();

	virtual float intersectWithRay(DirectX::XMVECTOR rayDir, DirectX::XMVECTOR rayOrigin) = 0;
};