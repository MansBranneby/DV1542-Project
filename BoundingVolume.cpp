#include "BoundingVolume.h"

BoundingVolume::BoundingVolume()
{
}

BoundingVolume::BoundingVolume(DirectX::XMVECTOR center)
{
	_center = center;
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
