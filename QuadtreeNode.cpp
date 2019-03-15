#include "QuadtreeNode.h"

std::vector<Mesh*> QuadtreeNode::boundingVolumeMeshTest(std::vector<Mesh*> meshes)
{
	std::vector<Mesh*> intersectedMeshes;
	for (size_t i = 0; i < meshes.size(); i++)
	{
		if (meshes[i]->getBoundingVolume()->intersectWithBox(_centerPos, _halfLength))
			intersectedMeshes.push_back(meshes[i]);
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

QuadtreeNode::QuadtreeNode(float halfLength, DirectX::XMFLOAT3 centerPos, std::vector<Mesh*> meshes, int quadtreeLevels, int currentLevel)
{
	using namespace DirectX;
	
	_quadTreeLevels = quadtreeLevels;
	_halfLength = halfLength;
	_centerPos = centerPos;
	_meshes = boundingVolumeMeshTest(meshes);
	_diagonals.resize(4);

	//Diagonals
	XMVECTOR centerPosVector = XMLoadFloat3(&_centerPos);
	_diagonals[0] = XMVector3Normalize((centerPosVector + XMVECTOR({ _halfLength, _halfLength, _halfLength, 0.0f }) - (centerPosVector + XMVECTOR({-_halfLength, -_halfLength, -_halfLength, 0.0f}))));	  // nearBL -> farTR
	_diagonals[1] = XMVector3Normalize((centerPosVector + XMVECTOR({ _halfLength, -_halfLength, _halfLength, 0.0f }) - (centerPosVector + XMVECTOR({ -_halfLength, _halfLength, -_halfLength, 0.0f })))); // nearTL -> farBR
	_diagonals[2] = XMVector3Normalize((centerPosVector + XMVECTOR({ -_halfLength, -_halfLength, _halfLength, 0.0f }) - (centerPosVector + XMVECTOR({ _halfLength, _halfLength, -_halfLength, 0.0f })))); // nearTR -> farBL
	_diagonals[3] = XMVector3Normalize((centerPosVector + XMVECTOR({ -_halfLength, _halfLength, _halfLength, 0.0f }) - (centerPosVector + XMVECTOR({ _halfLength, -_halfLength, -_halfLength, 0.0f })))); // nearBR -> farTL

	if (currentLevel < quadtreeLevels)
	{
		std::vector<DirectX::XMFLOAT3> centers = calculateCenter();

		for (size_t i = 0; i < 4; i++)
		{
			_children.push_back(new QuadtreeNode(_halfLength / 2, centers[i], _meshes, quadtreeLevels, currentLevel + 1));
		}
	}
}

QuadtreeNode::~QuadtreeNode()
{
	for (size_t i = 0; i < 4; i++)
		delete _children[i];

	for (size_t i = 0; i < _meshes.size(); i++)
		delete _meshes[i];


}

std::vector<Mesh*> QuadtreeNode::getIntersectedMeshes(DirectX::XMVECTOR camPos, DirectX::XMVECTOR lookAt, DirectX::XMVECTOR up, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix, float nearDist, float farDist, float FOV, float aspectRatio)
{
	using namespace DirectX;
	//up = XMVector3Normalize(up);
	//lookAt = XMVector3Normalize(lookAt);
	//XMVECTOR right = XMVector3Cross(up, lookAt);

	////Heights and widths
	//float hNear = 2 * tan(FOV * 0.5) * nearDist;
	//float wNear = hNear * aspectRatio;
	//float hFar = 2 * tan(FOV * 0.5f) * farDist;
	//float wFar = hFar * aspectRatio;

	////Points
	//XMVECTOR farCenter = camPos + lookAt * farDist;
	//XMVECTOR farTopLeft = farCenter + (up * hFar * 0.5f) - (right * wFar * 0.5f);
	//XMVECTOR farTopRight = farCenter + (up * hFar * 0.5f) + (right * wFar * 0.5f);
	//XMVECTOR farBottomLeft = farCenter - (up * hFar * 0.5f) - (right * wFar * 0.5f);
	//XMVECTOR farBottomRight = farCenter - (up * hFar * 0.5f) + (right * wFar * 0.5f);

	//XMVECTOR nearCenter = camPos + lookAt * nearDist;
	//XMVECTOR nearTopLeft = nearCenter + (up * hNear * 0.5f) - (right * wNear * 0.5f);
	//XMVECTOR nearTopRight = nearCenter + (up * hNear * 0.5f) + (right * wNear * 0.5);
	//XMVECTOR nearBottomLeft = nearCenter - (up * hNear * 0.5f) - (right * wNear * 0.5f);
	//XMVECTOR nearBottomRight = nearCenter - (up * hNear * 0.5f) + (right * wNear * 0.5f);

	////Plane normals
	//XMVECTOR normalUp = -XMVector3Normalize(XMVector3Cross(farTopRight - farTopLeft, camPos - farTopLeft));
	//XMVECTOR normalDown = XMVector3Normalize(XMVector3Cross(farBottomLeft - camPos, farBottomRight - camPos));
	//XMVECTOR normalRight = XMVector3Normalize(XMVector3Cross(farBottomRight - farTopRight, camPos - farTopRight));
	//XMVECTOR normalLeft = XMVector3Normalize(XMVector3Cross(farTopLeft - camPos, farBottomLeft - camPos));
	///*XMVECTOR normalNear = lookAt;
	//XMVECTOR normalFar = -lookAt;*/
	//XMVECTOR normalNear = XMVector3Normalize(XMVector3Cross(nearTopRight - nearBottomRight, nearBottomLeft - nearBottomRight));
	//XMVECTOR normalFar = XMVector3Normalize(XMVector3Cross(farTopLeft - farBottomLeft, farBottomRight - farBottomLeft));

	//std::vector<XMVECTOR> normals{ normalUp, normalDown, normalRight, normalLeft, normalNear, normalFar };
	//std::vector<XMVECTOR> points{ farTopLeft, camPos, farTopRight, camPos, nearBottomRight, farBottomLeft };
	//std::vector<float> planeConstants{ -(XMVectorGetX(normalUp)*XMVectorGetX(farTopLeft) + XMVectorGetY(normalUp)*XMVectorGetY(farTopLeft) + XMVectorGetZ(normalUp)*XMVectorGetZ(farTopLeft)),
	//								  -(XMVectorGetX(normalDown)*XMVectorGetX(camPos) + XMVectorGetY(normalDown)*XMVectorGetY(camPos) + XMVectorGetZ(normalDown)*XMVectorGetZ(camPos)),
	//								  -(XMVectorGetX(normalRight)*XMVectorGetX(farTopRight) + XMVectorGetY(normalRight)*XMVectorGetY(farTopRight) + XMVectorGetZ(normalRight)*XMVectorGetZ(farTopRight)),
	//								  -(XMVectorGetX(normalLeft)*XMVectorGetX(camPos) + XMVectorGetY(normalRight)*XMVectorGetY(camPos) + XMVectorGetZ(normalRight)*XMVectorGetZ(camPos)),
	//								  -(XMVectorGetX(normalNear)*XMVectorGetX(nearBottomRight) + XMVectorGetY(normalNear)*XMVectorGetY(nearBottomRight) + XMVectorGetZ(normalNear)*XMVectorGetZ(nearBottomRight)),
	//								  -(XMVectorGetX(normalFar)*XMVectorGetX(farBottomLeft) + XMVectorGetY(normalFar)*XMVectorGetY(farBottomLeft) + XMVectorGetZ(normalFar)*XMVectorGetZ(farBottomLeft)) };

	// PLANE EXTRACTION //
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);
	XMMATRIX viewProj = XMMatrixMultiply(projectionMatrix, viewMatrix);
	
	// Planes
	std::vector <XMVECTOR> frustumPlanes{ 
		viewProj.r[3] + viewProj.r[0], // Left
		viewProj.r[3] - viewProj.r[0], // Right
		viewProj.r[3] + viewProj.r[1], // Bottom
		viewProj.r[3] - viewProj.r[1], // Top
		viewProj.r[3] + viewProj.r[2], // Near
		viewProj.r[3] - viewProj.r[2], // Far
	}; 

	// Normalize planes
	for (size_t i = 0; i < frustumPlanes.size(); i++)
		frustumPlanes[i] = XMPlaneNormalize(frustumPlanes[i]);

	return getMeshes(frustumPlanes, 1);
}

bool QuadtreeNode::intersectWithFrustum(std::vector<DirectX::XMVECTOR> normals, std::vector<DirectX::XMVECTOR> points, std::vector<float> planeConstants)
{
	using namespace DirectX;
	//Intersecton for each plane
	for (size_t i = 0; i < 6; i++)
	{
		float dotValue = -1;
		size_t sIndex = 0;

		//Find best diagonal
		for (size_t j = 0; j < 4; j++)
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
			//float length1 = XMVectorGetX(XMVector3Length((XMVectorGetX(XMVector3Dot(point1 - points[i], normals[i])) / (XMVectorGetX(XMVector3Length(normals[i])) * XMVectorGetX(XMVector3Length(normals[i])))) * normals[i])); // distance from point1 to frustrum plane
			//float length2 = XMVectorGetX(XMVector3Length((XMVectorGetX(XMVector3Dot(point2 - points[i], normals[i])) / (XMVectorGetX(XMVector3Length(normals[i])) * XMVectorGetX(XMVector3Length(normals[i])))) * normals[i])); // distance from point2 to frustrum plane
			//length1 = XMVectorGetX(XMVector3Dot(normals[i], (XMVectorGetX(XMVector3Dot(point1 - points[i], normals[i])) / (XMVectorGetX(XMVector3Length(normals[i])) * XMVectorGetX(XMVector3Length(normals[i])))) * normals[i]))* length1;
			//length2 = XMVectorGetX(XMVector3Dot(normals[i], (XMVectorGetX(XMVector3Dot(point2 - points[i], normals[i])) / (XMVectorGetX(XMVector3Length(normals[i])) * XMVectorGetX(XMVector3Length(normals[i])))) * normals[i]))* length2;
			float length1 = XMVectorGetX(XMVector3Dot(normals[i], point1)) + planeConstants[i];
			float length2 = XMVectorGetX(XMVector3Dot(normals[i], point2)) + planeConstants[i];
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
			if (n < 0 && p < 0)
				return false;
			break;
		}
		case 1:
		{
			float n, p;
			XMVECTOR point1 = centerPosVector + XMVECTOR({ _halfLength, -_halfLength, _halfLength, 0.0f });
			XMVECTOR point2 = centerPosVector + XMVECTOR({ -_halfLength, _halfLength, -_halfLength, 0.0f });
			float length1 = XMVectorGetX(XMVector3Dot(normals[i], point1)) + planeConstants[i];
			float length2 = XMVectorGetX(XMVector3Dot(normals[i], point2)) + planeConstants[i];
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
			if (n < 0 && p < 0)
				return false;
			break;
		}
		case 2:
		{
			float n, p;
			XMVECTOR point1 = centerPosVector + XMVECTOR({ -_halfLength, -_halfLength, _halfLength, 0.0f });
			XMVECTOR point2 = centerPosVector + XMVECTOR({ _halfLength, _halfLength, -_halfLength, 0.0f });
			float length1 = XMVectorGetX(XMVector3Dot(normals[i], point1)) + planeConstants[i];
			float length2 = XMVectorGetX(XMVector3Dot(normals[i], point2)) + planeConstants[i];
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
			if (n < 0 && p < 0)
				return false;
			break;
		}
		case 3:
		{
			float n, p;
			XMVECTOR point1 = centerPosVector + XMVECTOR({ -_halfLength, _halfLength, _halfLength, 0.0f });
			XMVECTOR point2 = centerPosVector + XMVECTOR({ _halfLength, -_halfLength, -_halfLength, 0.0f });
			float length1 = XMVectorGetX(XMVector3Dot(normals[i], point1)) + planeConstants[i];
			float length2 = XMVectorGetX(XMVector3Dot(normals[i], point2)) + planeConstants[i];
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
			if (n < 0 && p < 0)
				return false;
			break;
		}
		default:
			break;
		}
	}

	return true;
}

bool QuadtreeNode::intersectWithFrustum(std::vector<DirectX::XMVECTOR> frustumPlanes)
{
	using namespace DirectX;


	//Intersecton for each plane
	for (size_t i = 0; i < 6; i++)
	{
		float dotValue = -1;
		size_t sIndex = 0;

		//Find best diagonal
		for (size_t j = 0; j < 4; j++)
		{
			float newDotValue = abs(XMVectorGetX(XMVector3Dot(frustumPlanes[i], _diagonals[j])));
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
			XMVECTOR point1 = centerPosVector + XMVECTOR({ _halfLength, _halfLength, _halfLength, 0.0f });
			XMVECTOR point2 = centerPosVector + XMVECTOR({ -_halfLength, -_halfLength, -_halfLength, 0.0f });
			float length1 = XMVectorGetX(XMVector3Dot(frustumPlanes[i], point1)) + XMVectorGetW(frustumPlanes[i]);
			float length2 = XMVectorGetX(XMVector3Dot(frustumPlanes[i], point2)) + XMVectorGetW(frustumPlanes[i]);

			if (length1 < 0 && length2 < 0)
				return false;
			break;
		}
		case 1:
		{
			XMVECTOR point1 = centerPosVector + XMVECTOR({ _halfLength, -_halfLength, _halfLength, 0.0f });
			XMVECTOR point2 = centerPosVector + XMVECTOR({ -_halfLength, _halfLength, -_halfLength, 0.0f });
			float length1 = XMVectorGetX(XMVector3Dot(frustumPlanes[i], point1)) + XMVectorGetW(frustumPlanes[i]);
			float length2 = XMVectorGetX(XMVector3Dot(frustumPlanes[i], point2)) + XMVectorGetW(frustumPlanes[i]);

			if (length1 < 0 && length2 < 0)
				return false;
			break;
		}
		case 2:
		{
			XMVECTOR point1 = centerPosVector + XMVECTOR({ -_halfLength, -_halfLength, _halfLength, 0.0f });
			XMVECTOR point2 = centerPosVector + XMVECTOR({ _halfLength, _halfLength, -_halfLength, 0.0f });
			float length1 = XMVectorGetX(XMVector3Dot(frustumPlanes[i], point1)) + XMVectorGetW(frustumPlanes[i]);
			float length2 = XMVectorGetX(XMVector3Dot(frustumPlanes[i], point2)) + XMVectorGetW(frustumPlanes[i]);

			if (length1 < 0 && length2 < 0)
				return false;
			break;
		}
		case 3:
		{
			XMVECTOR point1 = centerPosVector + XMVECTOR({ -_halfLength, _halfLength, _halfLength, 0.0f });
			XMVECTOR point2 = centerPosVector + XMVECTOR({ _halfLength, -_halfLength, -_halfLength, 0.0f });
			float length1 = XMVectorGetX(XMVector3Dot(frustumPlanes[i], point1)) + XMVectorGetW(frustumPlanes[i]);
			float length2 = XMVectorGetX(XMVector3Dot(frustumPlanes[i], point2)) + XMVectorGetW(frustumPlanes[i]);

			if (length1 < 0 && length2 < 0)
				return false;
			break;
		}
		default:
			break;
		}
	}

	return true;


	return false;
}

std::vector<Mesh*> QuadtreeNode::getMeshes(std::vector<DirectX::XMVECTOR> normals, std::vector<DirectX::XMVECTOR> points, std::vector<float> planeConstants, int currentLevel)
{
	std::vector<Mesh*> returnMeshes;
	std::vector<Mesh*> tempMeshes;
	if (intersectWithFrustum(normals, points, planeConstants) && currentLevel < _quadTreeLevels)
	{
		for (size_t i = 0; i < 4; i++)
		{
			tempMeshes = _children[i]->getMeshes(normals, points, planeConstants, currentLevel + 1);
			for (size_t j = 0; j < tempMeshes.size(); j++)
				returnMeshes.push_back(tempMeshes[j]);
		}
	}
	else if (intersectWithFrustum(normals, points, planeConstants) && currentLevel == _quadTreeLevels)
		returnMeshes = _meshes;

	return returnMeshes;
}

std::vector<Mesh*> QuadtreeNode::getMeshes(std::vector<DirectX::XMVECTOR> frustumPlanes, int currentLevel)
{
	std::vector<Mesh*> returnMeshes;
	std::vector<Mesh*> tempMeshes;
	if (intersectWithFrustum(frustumPlanes) && currentLevel < _quadTreeLevels)
	{
		for (size_t i = 0; i < 4; i++)
		{
			tempMeshes = _children[i]->getMeshes(frustumPlanes, currentLevel + 1);
			for (size_t j = 0; j < tempMeshes.size(); j++)
				returnMeshes.push_back(tempMeshes[j]);
		}
	}
	else if (intersectWithFrustum(frustumPlanes) && currentLevel == _quadTreeLevels)
		returnMeshes = _meshes;

	return returnMeshes;
}
