#include "OBB.h"

OBB::OBB()
{
	_half_u_v_w = { 0.0f, 0.0f, 0.0f, 0.0f };
}

OBB::OBB(DirectX::XMVECTOR center, DirectX::XMVECTOR half_u_v_w)
	:BoundingVolume(center)
{
}

OBB::~OBB()
{
}

void OBB::setHalf_u_v_w(DirectX::XMVECTOR half_u_v_w)
{
}

DirectX::XMVECTOR OBB::getHalf_u_v_w()
{
	return _half_u_v_w;
}

float OBB::intersectWithRay()
{
	return 0.0f;
}
