#include "OBB.h"

OBB::OBB()
{
	_half_u_v_w = { 0.0f, 0.0f, 0.0f};
}

OBB::OBB(DirectX::XMFLOAT3 smallestXYZ, DirectX::XMFLOAT3 biggestXYZ, ID3D11Device* device)
	:BoundingVolume(device)
{
	// BOUNDING VOLUME
	//
	DirectX::XMFLOAT3 halfXYZ;
	halfXYZ.x = (abs(smallestXYZ.x) + abs(biggestXYZ.x)) / 2;
	halfXYZ.y = (abs(smallestXYZ.y) + abs(biggestXYZ.y)) / 2;
	halfXYZ.z = (abs(smallestXYZ.z) + abs(biggestXYZ.z)) / 2;

	_center = { 0.0f, halfXYZ.y, 0.0f };
	_half_u_v_w = { halfXYZ.x, halfXYZ.y, halfXYZ.z };

	DirectX::XMFLOAT3 col(0.0f, 0.0f, 0.0f);
	DirectX::XMFLOAT3 _rightUpNear(biggestXYZ.x, biggestXYZ.y, smallestXYZ.z);
	DirectX::XMFLOAT3 _rightDownNear(biggestXYZ.x, smallestXYZ.y, smallestXYZ.z);
	DirectX::XMFLOAT3 _leftUpNear(smallestXYZ.x, biggestXYZ.y, smallestXYZ.z);
	DirectX::XMFLOAT3 _leftDownNear(smallestXYZ.x, smallestXYZ.y, smallestXYZ.z);

	DirectX::XMFLOAT3 _rightUpFar(biggestXYZ.x, biggestXYZ.y, biggestXYZ.z);
	DirectX::XMFLOAT3 _rightDownFar(biggestXYZ.x, smallestXYZ.y, biggestXYZ.z);
	DirectX::XMFLOAT3 _leftUpFar(smallestXYZ.x, biggestXYZ.y, biggestXYZ.z);
	DirectX::XMFLOAT3 _leftDownFar(smallestXYZ.x, smallestXYZ.y, biggestXYZ.z);

	_vertices.push_back(Vertex_Pos_Col(_rightUpNear, col));
	_vertices.push_back(Vertex_Pos_Col(_rightDownNear, col));
	_vertices.push_back(Vertex_Pos_Col(_rightDownNear, col));
	_vertices.push_back(Vertex_Pos_Col(_leftDownNear, col));
	_vertices.push_back(Vertex_Pos_Col(_leftDownNear, col));
	_vertices.push_back(Vertex_Pos_Col(_leftUpNear, col));
	_vertices.push_back(Vertex_Pos_Col(_leftUpNear, col));
	_vertices.push_back(Vertex_Pos_Col(_rightUpNear, col));
	_vertices.push_back(Vertex_Pos_Col(_rightUpNear, col));
	_vertices.push_back(Vertex_Pos_Col(_rightUpFar, col));
	_vertices.push_back(Vertex_Pos_Col(_rightUpFar, col));
	_vertices.push_back(Vertex_Pos_Col(_rightDownFar, col));
	_vertices.push_back(Vertex_Pos_Col(_rightDownFar, col));
	_vertices.push_back(Vertex_Pos_Col(_rightDownNear, col));
	_vertices.push_back(Vertex_Pos_Col(_rightDownFar, col));
	_vertices.push_back(Vertex_Pos_Col(_leftDownFar, col));
	_vertices.push_back(Vertex_Pos_Col(_leftDownFar, col));
	_vertices.push_back(Vertex_Pos_Col(_leftDownNear, col));
	_vertices.push_back(Vertex_Pos_Col(_leftDownFar, col));
	_vertices.push_back(Vertex_Pos_Col(_leftUpFar, col));
	_vertices.push_back(Vertex_Pos_Col(_leftUpFar, col));
	_vertices.push_back(Vertex_Pos_Col(_leftUpNear, col));
	_vertices.push_back(Vertex_Pos_Col(_leftUpFar, col));
	_vertices.push_back(Vertex_Pos_Col(_rightUpFar, col));

	createVertexBuffer(device);
}

OBB::~OBB()
{
}

void OBB::setHalf_u_v_w(DirectX::XMFLOAT3 half_u_v_w)
{
}

DirectX::XMFLOAT3 OBB::getHalf_u_v_w()
{
	return _half_u_v_w;
}

float OBB::intersectWithRay(DirectX::XMVECTOR rayDir, DirectX::XMVECTOR rayOrigin)
{
	DirectX::XMVECTOR defU = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, _half_u_v_w.x);
	DirectX::XMVECTOR defV = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, _half_u_v_w.y);
	DirectX::XMVECTOR defW = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, _half_u_v_w.z);
	// IMPLEMENT HERE
	float tMin = -1000000, tMax = 1000000;
	DirectX::XMVECTOR center = DirectX::XMLoadFloat3(&getCenter());
	DirectX::XMVECTOR p = DirectX::XMVectorSubtract(center, rayOrigin);
	DirectX::XMVECTOR arr[3] = {defU, defV, defW };

	for (int i = 0; i < 3; i++)
	{
		DirectX::XMVECTOR eVec = DirectX::XMVector3Dot(arr[i], p); // Dot product with DirectXMath returns a vector with the same value copied into the xyzw positions
		DirectX::XMVECTOR fVec = DirectX::XMVector3Dot(arr[i], rayDir); 
		float e = DirectX::XMVectorGetX(eVec);
		float f = DirectX::XMVectorGetX(fVec);
		float w = DirectX::XMVectorGetW(arr[i]);
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
				return -1.0f;
			if (tMax < 0)
				return -1.0f;
		}
		else if ((-e - w) > 0 || (-e + w) < 0)
			return -1.0f;
	}
	if (tMin > 0)
		return tMin;
	else
		return tMax;
}

void OBB::intersectWithBox(DirectX::XMFLOAT3 center, float halfLength)
{
	DirectX::XMFLOAT3 test1 = { _half_u_v_w.x , 0.0f, 0.0f };
	DirectX::XMVECTOR test = DirectX::XMLoadFloat3(&test1);

	//if ((center.x + halfLength) > _center.x + DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(_half_u_v_w.x, 0.0f, 0.0f)), DirectX::XMMatrixInverse(nullptr, _worldMatrix)));
}
