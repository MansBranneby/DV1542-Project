#include "QuadtreeNode.h"

std::vector<Mesh*> QuadtreeNode::intersectWith()
{
	std::vector<Mesh*> intersectedMeshes;
	for (int i = 0; i < _meshes.size(); i++)
	{
		if (_meshes[i]->getBoundingVolume()->intersectWithBox(_centerPos, _halfLength))
			intersectedMeshes.push_back(_meshes[i]);
	}

	return intersectedMeshes;
}

std::vector<DirectX::XMFLOAT3> QuadtreeNode::calculateCenter()
{
	std::vector<DirectX::XMFLOAT3> newCenterPos;
	newCenterPos.push_back({ _centerPos.x + _halfLength / 2, _centerPos.y, _centerPos.z + _halfLength / 2 }); // UR
	newCenterPos.push_back({ _centerPos.x + _halfLength / 2, _centerPos.y, _centerPos.z - _halfLength / 2 }); // BR
	newCenterPos.push_back({ _centerPos.x - _halfLength / 2, _centerPos.y, _centerPos.z + _halfLength / 2 }); // UL
	newCenterPos.push_back({ _centerPos.x - _halfLength / 2, _centerPos.y, _centerPos.z - _halfLength / 2 }); // BL

	return newCenterPos;
}

QuadtreeNode::QuadtreeNode(float halfLength, DirectX::XMFLOAT3 centerPos, std::vector<Mesh*> _meshes, int quadtreeLevels, int currentLevel)
{
	using namespace DirectX;
	
	_quadTreeLevels = quadtreeLevels;
	_halfLength = halfLength;
	_centerPos = centerPos;

	//Diagonals
	XMVECTOR centerPosVector = XMLoadFloat3(&_centerPos);
	_diagonals[0] = (centerPosVector + XMVECTOR({ _halfLength, _halfLength, _halfLength, 0.0f }) - (centerPosVector + XMVECTOR({-_halfLength, -_halfLength, -_halfLength, 0.0f})));	  // nearBL -> farTR
	_diagonals[1] = (centerPosVector + XMVECTOR({ _halfLength, -_halfLength, _halfLength, 0.0f }) - (centerPosVector + XMVECTOR({ -_halfLength, _halfLength, -_halfLength, 0.0f }))); // nearTL -> farBR
	_diagonals[2] = (centerPosVector + XMVECTOR({ -_halfLength, -_halfLength, _halfLength, 0.0f }) - (centerPosVector + XMVECTOR({ _halfLength, _halfLength, -_halfLength, 0.0f }))); // nearTR -> farBL
	_diagonals[3] = (centerPosVector + XMVECTOR({ -_halfLength, _halfLength, _halfLength, 0.0f }) - (centerPosVector + XMVECTOR({ _halfLength, -_halfLength, -_halfLength, 0.0f }))); // nearBR -> farTL

	if (currentLevel < quadtreeLevels)
	{
		std::vector<DirectX::XMFLOAT3> centers = calculateCenter();
		std::vector<Mesh*> intersectedMeshes = intersectWith();

		for (int i = 0; i < 4; i++)
		{
			_children.push_back(new QuadtreeNode(_halfLength / 2, centers[i], intersectedMeshes, quadtreeLevels, currentLevel + 1));
		}
	}
}

std::vector<Mesh*> QuadtreeNode::getIntersectedMeshes(DirectX::XMVECTOR camPos, DirectX::XMVECTOR lookAt, DirectX::XMVECTOR up, float nearDist, float farDist, float FOV, float aspectRatio)
{
	using namespace DirectX;
	up = XMVector3Normalize(up);
	lookAt = XMVector3Normalize(lookAt);
	XMVECTOR right = XMVector3Cross(up, lookAt);

	//Heights and widths
	float hNear = 2 * tan(FOV / 2) * nearDist;
	float wNear = hNear * aspectRatio;
	float hFar = 2 * tan(FOV / 2) * nearDist;
	float wFar = hFar * aspectRatio;

	//Points
	XMVECTOR farCenter = camPos + lookAt * farDist;
	XMVECTOR farTopLeft = farCenter + (up * hFar / 2) - (right * wFar / 2);
	XMVECTOR farTopRight = farCenter + (up * hFar / 2) + (right * wFar / 2);
	XMVECTOR farBottomLeft = farCenter - (up * hFar / 2) - (right * wFar / 2);
	XMVECTOR farBottomRight = farCenter - (up * hFar / 2) + (right * wFar / 2);

	XMVECTOR nearCenter = camPos + lookAt * nearDist;
	XMVECTOR nearTopLeft = nearCenter + (up * hNear / 2) - (right * wNear / 2);
	XMVECTOR nearTopRight = nearCenter + (up * hNear / 2) + (right * wNear / 2);
	XMVECTOR nearBottomLeft = nearCenter - (up * hNear / 2) - (right * wNear / 2);
	XMVECTOR nearBottomRight = nearCenter - (up * hNear / 2) + (right * wNear / 2);

	//Plane normals
	XMVECTOR normalUp = -XMVector3Cross(farTopLeft - farTopRight, camPos - farTopRight);
	XMVECTOR normalDown = -normalUp;
	XMVECTOR normalRight = XMVector3Cross(farTopRight - farBottomRight, camPos - farBottomRight);
	XMVECTOR normalLeft = -normalRight;
	XMVECTOR normalNear = lookAt;
	XMVECTOR normalFar = -lookAt;

	std::vector<XMVECTOR> normals{ normalUp, normalDown, normalRight, normalLeft, normalNear, normalFar };
	std::vector<XMVECTOR> points{ camPos, camPos, camPos, camPos, nearCenter, farCenter };

	return getMeshes(normals, points, 1);
}

bool QuadtreeNode::intersectWithFrustum(std::vector<DirectX::XMVECTOR> normals, std::vector<DirectX::XMVECTOR> points)
{
	using namespace DirectX;
	//Intersecton for each plane
	for (int i = 0; i < 6; i++)
	{
		float dotValue = -1;
		int sIndex = -1;

		//Find best diagonal
		for (int j = 1; j < 4; j++)
		{
			float newDotValue = XMVectorGetX(XMVector3Dot(normals[i], _diagonals[j]));
			if (newDotValue > dotValue)
			{
				dotValue = newDotValue;
				sIndex = j;
			}
		}

		//Test Intersection
		XMVECTOR centerPosVector = XMLoadFloat3(&_centerPos);
		switch (sIndex)
		{
		case 0:
		{
			float n, p;
			XMVECTOR point1 = centerPosVector + XMVECTOR({ _halfLength, _halfLength, _halfLength, 0.0f });
			XMVECTOR point2 = centerPosVector + XMVECTOR({ -_halfLength, -_halfLength, -_halfLength, 0.0f });
			float length1 = XMVectorGetX(XMVector3Length(XMVectorGetX(XMVector3Dot(point1, normals[i])) / XMVectorGetX(XMVector3Length(point1)) * normals[i])); // distance from point1 to frustrum plane
			float length2 = XMVectorGetX(XMVector3Length(XMVectorGetX(XMVector3Dot(point2, normals[i])) / XMVectorGetX(XMVector3Length(point2)) * normals[i])); // distance from point2 to frustrum plane
			if (length1 < length2)
			{
				n = length1;
				p = length2;
			}
			else
			{
				n = length2;
				p = length1;
			}
			if (n > 0)
				return false;
			break;
		}
		case 1:
		{
			float n, p;
			XMVECTOR point1 = centerPosVector + XMVECTOR({ _halfLength, -_halfLength, _halfLength, 0.0f });
			XMVECTOR point2 = centerPosVector + XMVECTOR({ -_halfLength, _halfLength, -_halfLength, 0.0f });
			float length1 = XMVectorGetX(XMVector3Length(XMVectorGetX(XMVector3Dot(point1, normals[i])) / XMVectorGetX(XMVector3Length(point1)) * normals[i])); // distance from point1 to frustrum plane
			float length2 = XMVectorGetX(XMVector3Length(XMVectorGetX(XMVector3Dot(point2, normals[i])) / XMVectorGetX(XMVector3Length(point2)) * normals[i])); // distance from point2 to frustrum plane
			if (length1 < length2)
			{
				n = length1;
				p = length2;
			}
			else
			{
				n = length2;
				p = length1;
			}
			if (n > 0)
				return false;
			break;
		}
		case 2:
		{
			float n, p;
			XMVECTOR point1 = centerPosVector + XMVECTOR({ -_halfLength, -_halfLength, _halfLength, 0.0f });
			XMVECTOR point2 = centerPosVector + XMVECTOR({ _halfLength, _halfLength, -_halfLength, 0.0f });
			float length1 = XMVectorGetX(XMVector3Length(XMVectorGetX(XMVector3Dot(point1, normals[i])) / XMVectorGetX(XMVector3Length(point1)) * normals[i])); // distance from point1 to frustrum plane
			float length2 = XMVectorGetX(XMVector3Length(XMVectorGetX(XMVector3Dot(point2, normals[i])) / XMVectorGetX(XMVector3Length(point2)) * normals[i])); // distance from point2 to frustrum plane
			if (length1 < length2)
			{
				n = length1;
				p = length2;
			}
			else
			{
				n = length2;
				p = length1;
			}
			if (n > 0)
				return false;
			break;
		}
		case 3:
		{
			float n, p;
			XMVECTOR point1 = centerPosVector + XMVECTOR({ -_halfLength, _halfLength, _halfLength, 0.0f });
			XMVECTOR point2 = centerPosVector + XMVECTOR({ _halfLength, -_halfLength, -_halfLength, 0.0f });
			float length1 = XMVectorGetX(XMVector3Length(XMVectorGetX(XMVector3Dot(point1, normals[i])) / XMVectorGetX(XMVector3Length(point1)) * normals[i])); // distance from point1 to frustrum plane
			float length2 = XMVectorGetX(XMVector3Length(XMVectorGetX(XMVector3Dot(point2, normals[i])) / XMVectorGetX(XMVector3Length(point2)) * normals[i])); // distance from point2 to frustrum plane
			if (length1 < length2)
			{
				n = length1;
				p = length2;
			}
			else
			{
				n = length2;
				p = length1;
			}
			if (n > 0)
				return false;
			break;
		}
		default:
			break;
		}
	}

	return true;
}

std::vector<Mesh*> QuadtreeNode::getMeshes(std::vector<DirectX::XMVECTOR> normals, std::vector<DirectX::XMVECTOR> points, int currentLevel)
{
	std::vector<Mesh*> returnMeshes;
	std::vector<Mesh*> tempMeshes;
	if (intersectWithFrustum(normals, points) && currentLevel < _quadTreeLevels)
	{
		for (int i = 0; i < 4; i++)
		{
			tempMeshes = _children[i]->getMeshes(normals, points, currentLevel + 1);
			for (int j = 0; j < tempMeshes.size(); j++)
				returnMeshes.push_back(tempMeshes[j]);
		}
	}
	else if (intersectWithFrustum(normals, points) && currentLevel == _quadTreeLevels)
		returnMeshes = _meshes;
	return returnMeshes;
}
