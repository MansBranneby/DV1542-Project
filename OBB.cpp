#include "OBB.h"

OBB::OBB()
{
	_half_u_v_w = { 0.0f, 0.0f, 0.0f};
}

OBB::OBB(DirectX::XMFLOAT3 smallestXYZ, DirectX::XMFLOAT3 biggestXYZ, ID3D11Device* device, DirectX::XMMATRIX modelMatrix)
{
	DirectX::XMFLOAT3 halfXYZ;
	halfXYZ.x = (abs(smallestXYZ.x) + abs(biggestXYZ.x)) / 2;
	halfXYZ.y = (abs(smallestXYZ.y) + abs(biggestXYZ.y)) / 2;
	halfXYZ.z = (abs(smallestXYZ.z) + abs(biggestXYZ.z)) / 2;

	_center = { 0.0f, halfXYZ.y, 0.0f };
	_half_u_v_w = { halfXYZ.x, halfXYZ.y, halfXYZ.z };

	DirectX::XMFLOAT3 col(1.0f, 1.0f, 1.0f);
	_rightUpNear = { biggestXYZ.x, biggestXYZ.y, smallestXYZ.z };
	_rightDownNear = { biggestXYZ.x, smallestXYZ.y, smallestXYZ.z };
	_leftUpNear = { smallestXYZ.x, biggestXYZ.y, smallestXYZ.z };
	_leftDownNear = { smallestXYZ.x, smallestXYZ.y, smallestXYZ.z };

	_rightUpFar = { biggestXYZ.x, biggestXYZ.y, biggestXYZ.z };
	_rightDownFar = {biggestXYZ.x, smallestXYZ.y, biggestXYZ.z};
	_leftUpFar = { smallestXYZ.x, biggestXYZ.y, biggestXYZ.z };
	_leftDownFar = { smallestXYZ.x, smallestXYZ.y, biggestXYZ.z };

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

	float tMin = -1000000, tMax = 1000000;
	DirectX::XMVECTOR center = DirectX::XMLoadFloat3(&getCenter());
	DirectX::XMVECTOR p = DirectX::XMVectorSubtract(center, rayOrigin);
	DirectX::XMVECTOR arr[3] = {defU, defV, defW };

	for (size_t i = 0; i < 3; i++)
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
			{
				setHighlight(false);
				return -1.0f;
			}
			if (tMax < 0)
			{
				setHighlight(false);
				return -1.0f;
			}
		}
		else if ((-e - w) > 0 || (-e + w) < 0)
		{
			setHighlight(false);
			return -1.0f;
		}
	}
	if (tMin > 0)
	{
		return tMin;
	}
	else
	{
		return tMax;
	}
}

bool OBB::intersectWithBox(DirectX::XMFLOAT3 center, float halfLength)
{
	if ((center.x + halfLength) > _rightUpNear.x || (center.x + halfLength) > _leftUpNear.x || (center.x + halfLength) > _rightDownNear.x || (center.x + halfLength) > _leftDownNear.x ||
		(center.x + halfLength) > _rightUpFar.x || (center.x + halfLength) > _leftUpFar.x || (center.x + halfLength) > _rightDownFar.x || (center.x + halfLength) > _leftDownFar.x)
	{
		if ((center.x - halfLength) < _rightUpNear.x || (center.x - halfLength) < _leftUpNear.x || (center.x - halfLength) < _rightDownNear.x || (center.x - halfLength) < _leftDownNear.x ||
			(center.x - halfLength) < _rightUpFar.x || (center.x - halfLength) < _leftUpFar.x || (center.x - halfLength) < _rightDownFar.x || (center.x - halfLength) < _leftDownFar.x)
		{
			if ((center.y + halfLength) > _rightUpNear.y || (center.y + halfLength) > _leftUpNear.y || (center.y + halfLength) > _rightDownNear.y || (center.y + halfLength) > _leftDownNear.y ||
				(center.y + halfLength) > _rightUpFar.y || (center.y + halfLength) > _leftUpFar.y || (center.y + halfLength) > _rightDownFar.y || (center.y + halfLength) > _leftDownFar.y)
			{
				if ((center.y - halfLength) < _rightUpNear.y || (center.y - halfLength) < _leftUpNear.y || (center.y - halfLength) < _rightDownNear.y || (center.y - halfLength) < _leftDownNear.y ||
					(center.y - halfLength) < _rightUpFar.y || (center.y - halfLength) < _leftUpFar.y || (center.y - halfLength) < _rightDownFar.y || (center.y - halfLength) < _leftDownFar.y)
				{
					if ((center.z + halfLength) > _rightUpNear.z || (center.z + halfLength) > _leftUpNear.z || (center.z + halfLength) > _rightDownNear.z || (center.z + halfLength) > _leftDownNear.z ||
						(center.z + halfLength) > _rightUpFar.z || (center.z + halfLength) > _leftUpFar.z || (center.z + halfLength) > _rightDownFar.z || (center.z + halfLength) > _leftDownFar.z)
					{
						if ((center.z - halfLength) < _rightUpNear.z || (center.z - halfLength) < _leftUpNear.z || (center.z - halfLength) < _rightDownNear.z || (center.z - halfLength) < _leftDownNear.z ||
							(center.z - halfLength) < _rightUpFar.z || (center.z - halfLength) < _leftUpFar.z || (center.z - halfLength) < _rightDownFar.z || (center.z - halfLength) < _leftDownFar.z)
						{
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}

void OBB::setWorldMatrix(ID3D11Device * device, DirectX::XMMATRIX worldMatrix)
{
	_modelMatrix = worldMatrix;

	transform(device, _modelMatrix);
	createVertexBuffer(device);
}


void OBB::transform(ID3D11Device* device, DirectX::XMMATRIX modelMatrix)
{
	for (size_t i = 0; i < _vertices.size(); i++)
	{
		DirectX::XMVECTOR posWS = DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&_vertices[i].getPos()), modelMatrix);
		_vertices[i].setPos({ DirectX::XMVectorGetX(posWS), DirectX::XMVectorGetY(posWS), DirectX::XMVectorGetZ(posWS) });
	}

	_rightUpNear = _vertices[0].getPos();
	_rightDownNear = _vertices[1].getPos();
	_leftDownNear = _vertices[3].getPos();
	_leftUpNear = _vertices[5].getPos();
	_rightUpFar = _vertices[9].getPos();
	_rightDownFar = _vertices[11].getPos();
	_leftDownFar = _vertices[15].getPos();
	_leftUpFar = _vertices[19].getPos();
}
