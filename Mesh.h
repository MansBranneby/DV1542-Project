#pragma once
#include "TriangleVertex.h"
#include "BoundingVolume.h"


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

class Mesh
{
private:
	std::vector <TriangleVertex> _vertices;
	std::string _filePath;
	DirectX::XMMATRIX _transform;
	BoundingVolume* _hitbox = nullptr;

public:
	Mesh();
	Mesh(std::string filePath, bool flippedUV, ID3D11ShaderResourceView** resourceView, ID3D11Device* device);
	
	~Mesh();
	void setVertices(std::vector <TriangleVertex> vertices);
	void setHitBox();

	std::vector <TriangleVertex> & getVertices();
	int getVertCount();
};