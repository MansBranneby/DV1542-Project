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

float OBB::intersectWithRay(DirectX::XMVECTOR rayDir, DirectX::XMVECTOR rayOrigin)
{
		// IMPLEMENT HERE
		float tMin = -1000000, tMax = 1000000;
		DirectX::XMVECTOR p = DirectX::XMVectorSubtract(getCenter(), rayOrigin);
		DirectX::XMVECTOR arr[3] = { getHalf_u_v_w() };

		for (int i = 0; i < 3; i++)
		{
			DirectX::XMVECTOR eVec = DirectX::XMVector4Dot(arr[i], p); // Dot product with DirectXMath returns a vector with the same value copied into the xyzw positions
			DirectX::XMVECTOR fVec = DirectX::XMVector4Dot(arr[i], rayDir); 
			float e = DirectX::XMVectorGetX(eVec);
			float f = DirectX::XMVectorGetX(fVec);
			float w = DirectX::XMVectorGetZ(arr[i]);
			if (f != 0)
			{
				float t1 = (e + w) / f;
				float t2 = (e - w) / f;
				if (t1 > t2)
				{
					float temp = t1;
					t1 = t2;
					t2 = temp;
				}
				if (t1 > tMin)
					tMin = t1;
				if (t2 < tMax)
					tMax = t2;
				if (tMin > tMax)
					return -1;
				if (tMax < 0)
					return -1;
			}
			else if ((-e - w) > 0 || (-e + w) < 0)
				return -1;
		}
		if (tMin > 0)
			return tMin;
		else
			return tMax;
}
