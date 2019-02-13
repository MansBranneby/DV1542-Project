#pragma once
#include "BoundingVolume.h"

class OBB : public BoundingVolume
{
private:
	DirectX::XMVECTOR _half_u_v_w;
	DirectX::XMMATRIX _worldMatrix;

public:
	OBB();
	~OBB();

	void setCenter(DirectX::XMVECTOR center);
	void setHalf_u_v_w(DirectX::XMVECTOR half_u_v_w);
	void setWorldMatrix(DirectX::XMMATRIX worldMatrix);

	DirectX::XMVECTOR getHalf_u_v_w();
	virtual float intersectWithRay();
};