#pragma once
#include "BoundingVolume.h"

class OBB : public BoundingVolume
{
private:
	DirectX::XMFLOAT3 _half_u_v_w;
	DirectX::XMMATRIX _worldMatrix;

public:
	OBB();
	OBB(DirectX::XMFLOAT3 smallestXYZ, DirectX::XMFLOAT3 biggestXYZ, ID3D11Device* device);
	~OBB();

	void setHalf_u_v_w(DirectX::XMFLOAT3 half_u_v_w);

	DirectX::XMFLOAT3 getHalf_u_v_w();
	virtual float intersectWithRay(DirectX::XMVECTOR rayDir, DirectX::XMVECTOR rayOrigin);
	virtual void intersectWithBox(DirectX::XMFLOAT3 center, float halfLength);
};