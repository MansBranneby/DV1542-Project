#pragma once
#include "Mesh.h"

class QuadtreeNode
{
private:
	int _quadTreeLevels;
	float _halfLength;
	DirectX::XMFLOAT3 _centerPos;
	std::vector <Mesh*> _meshes;
	std::vector <QuadtreeNode*> _children;
	std::vector <DirectX::XMVECTOR> _diagonals;
	std::vector<Mesh*> boundingVolumeMeshTest(std::vector<Mesh*> meshes);
	std::vector <DirectX::XMFLOAT3> calculateCenter();
	
	bool intersectWithFrustum(std::vector<DirectX::XMVECTOR> normals, std::vector<DirectX::XMVECTOR> points, std::vector<float> planeConstants);
	bool intersectWithFrustum(std::vector<DirectX::XMVECTOR> frustumPlanes); // Plane extraction
	std::vector<Mesh*> getMeshes(std::vector<DirectX::XMVECTOR> normals, std::vector<DirectX::XMVECTOR> points, std::vector<float> planeConstants, int currentLevel);
	std::vector<Mesh*> getMeshes(std::vector<DirectX::XMVECTOR> frustumPlanes, int currentLevel); // Plane extraction

public:
	QuadtreeNode(float halfLength, DirectX::XMFLOAT3 centerPos, std::vector <Mesh*> meshes, int quadtreeLevels, int currentLevel);
	std::vector<Mesh*> getIntersectedMeshes(DirectX::XMVECTOR camPos, DirectX::XMVECTOR lookAt, DirectX::XMVECTOR up, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix, float nearDist, float farDist, float FOV, float aspectRatio);
};