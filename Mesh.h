#pragma once
#include "TriangleVertex.h"
#include "BoundingVolume.h"
#include "OBB.h"

#include <vector>
#include <d3d11.h>
// Extra
#include <algorithm>
#include <wchar.h>
// Reading files
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
// Load texture
#include "WICTextureLoader.h"
#include <windows.h>

enum boundingVolumes { ORIENTED_BOUNDING_BOX, AXIS_ALIGNED_BOUNDING_BOX, SPHERE };

class Mesh
{
private:
	std::vector <TriangleVertex> _vertices;
	std::string _filePath;
	DirectX::XMMATRIX _transform;
	BoundingVolume* _boundingVolume = nullptr;

public:
	Mesh();
	Mesh(std::string filePath, bool flippedUV, ID3D11ShaderResourceView** resourceView, ID3D11Device* device, boundingVolumes boundingVolumeChoice);
	
	~Mesh();
	void setVertices(std::vector <TriangleVertex> vertices);
	void setHitBox();

	std::vector <TriangleVertex> & getVertices();
	int getVertCount();
	BoundingVolume* getBoundingVolume();
};