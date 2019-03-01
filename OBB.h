#pragma once
#include "BoundingVolume.h"

class OBB : public BoundingVolume
{
private:
	DirectX::XMFLOAT3 _half_u_v_w;
	DirectX::XMFLOAT3 _rightUpNear;
	DirectX::XMFLOAT3 _rightDownNear;
	DirectX::XMFLOAT3 _leftUpNear;
	DirectX::XMFLOAT3 _leftDownNear;
	DirectX::XMFLOAT3 _rightUpFar;
	DirectX::XMFLOAT3 _rightDownFar;
	DirectX::XMFLOAT3 _leftUpFar;
	DirectX::XMFLOAT3 _leftDownFar;

public:
	OBB();
	OBB(DirectX::XMFLOAT3 smallestXYZ, DirectX::XMFLOAT3 biggestXYZ, ID3D11Device* device);
	~OBB();

	void setHalf_u_v_w(DirectX::XMFLOAT3 half_u_v_w);

	DirectX::XMFLOAT3 getHalf_u_v_w();
	virtual float intersectWithRay(DirectX::XMVECTOR rayDir, DirectX::XMVECTOR rayOrigin);
	virtual bool intersectWithBox(DirectX::XMFLOAT3 center, float halfLength);

	void transform(DirectX::XMMATRIX worldMatrix);
};