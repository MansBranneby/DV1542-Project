#pragma once
#include "Mesh.h"

class QuadtreeNode
{
private:
	float _halfLength;
	DirectX::XMFLOAT3 _centerPos;
	std::vector <Mesh*> _meshes;
	std::vector <QuadtreeNode*> _children;
	
	void intersectWith();
	std::vector <DirectX::XMFLOAT3> calculateCenter();
	void fillNodes();

public:
	QuadtreeNode(float halfLength, DirectX::XMFLOAT3 centerPos, std::vector <Mesh*> _meshes, int quadtreeLevels);
};