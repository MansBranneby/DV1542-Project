#include "QuadtreeNode.h"

void QuadtreeNode::intersectWith()
{
	_meshes;
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



void QuadtreeNode::fillNodes()
{ 
}

QuadtreeNode::QuadtreeNode(float halfLength, DirectX::XMFLOAT3 centerPos, std::vector<Mesh*> _meshes, int quadtreeLevels)
{
	_halfLength = halfLength;
	_centerPos = centerPos;
}
