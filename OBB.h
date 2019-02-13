#pragma once
#include "BoundingVolume.h"

class OBB : public BoundingVolume
{
private:
	DirectX::XMVECTOR _half_u_v_w;
	DirectX::XMMATRIX _worldMatrix;

public:
	OBB();
	OBB(DirectX::XMVECTOR center, DirectX::XMVECTOR half_u_v_w);
	~OBB();

	void setHalf_u_v_w(DirectX::XMVECTOR half_u_v_w);

	DirectX::XMVECTOR getHalf_u_v_w();
	virtual float intersectWithRay();
};