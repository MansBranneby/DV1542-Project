#include "BoundingVolume.h"

BoundingVolume::BoundingVolume()
{
}

BoundingVolume::BoundingVolume(DirectX::XMVECTOR center, std::vector <TriangleVertexPosCol> vertices)
{
	_center = center;
	_vertices = vertices;

}

BoundingVolume::~BoundingVolume()
{
}

void BoundingVolume::setWorldMatrix(DirectX::XMMATRIX worldMatrix)
{
	_worldMatrix = worldMatrix;
}

void BoundingVolume::setCenter(DirectX::XMVECTOR center)
{
	_center = center;
}

DirectX::XMVECTOR BoundingVolume::getCenter()
{
	return _center;
}

std::vector<TriangleVertexPosCol>& BoundingVolume::getVertices()
{
	return _vertices;
}

int BoundingVolume::getVertCount()
{
	return _vertices.size();
}
