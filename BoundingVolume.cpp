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

void BoundingVolume::setHighlight(bool highlighted)
{
	_highlighted = highlighted;
	if (_highlighted == true)
	{
		for (int i = 0; i < _vertices.size(); i++)
			_vertices.at(i).setCol(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
	}
	else
	{
		for (int i = 0; i < _vertices.size(); i++)
			_vertices.at(i).setCol(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	}
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
