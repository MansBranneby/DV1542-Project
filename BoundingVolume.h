#pragma once
#include <DirectXMath.h>

class BoundingVolume
{
private:
	DirectX::XMVECTOR _center;
	DirectX::XMMATRIX _worldMatrix;

public:
	BoundingVolume();
	BoundingVolume(DirectX::XMVECTOR center);
	~BoundingVolume();

	void setWorldMatrix(DirectX::XMMATRIX worldMatrix);
	void setCenter(DirectX::XMVECTOR center);

	DirectX::XMVECTOR getCenter();

	virtual float intersectWithRay(DirectX::XMVECTOR rayDir, DirectX::XMVECTOR rayOrigin) = 0;
};