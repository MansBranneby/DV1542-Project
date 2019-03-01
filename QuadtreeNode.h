#pragma once
#include "Mesh.h"

class QuadtreeNode
{
private:
	float _halfLength;
	DirectX::XMFLOAT3 _centerPos;
	std::vector <Mesh*> _meshes;
	std::vector <QuadtreeNode*> _children[4];
	std::vector <DirectX::XMVECTOR> _diagonals;
	std::vector<Mesh*> intersectWith();
	std::vector <DirectX::XMFLOAT3> calculateCenter();

public:
	QuadtreeNode(float halfLength, DirectX::XMFLOAT3 centerPos, std::vector <Mesh*> _meshes, int quadtreeLevels, int currentLevel);

	bool intersectWithFrustum(DirectX::XMVECTOR camPos, DirectX::XMVECTOR lookAt, DirectX::XMVECTOR up, float nearDist, float farDist, float FOV, float aspectRatio);
};