#pragma once
#include "Vertex_Pos_UV_Normal.h"
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
	std::vector <Vertex*> _vertices;
	ID3D11ShaderResourceView* _SRV_Texture = nullptr;
	ID3D11ShaderResourceView* _SRV_Normal = nullptr;
	ID3D11Buffer* _vertexBuffer = nullptr;
	DirectX::XMMATRIX _worldMatrix;
	BoundingVolume* _boundingVolume = nullptr;

public:
	Mesh();
	Mesh(std::string filePath, bool flippedUV, ID3D11Device* device, boundingVolumes boundingVolumeChoice);
	Mesh(std::vector <Vertex*> vertices, ID3D11Device* device);
	~Mesh();
	void setVertices(std::vector <Vertex*> vertices);

	std::vector <Vertex*> & getVertices();
	ID3D11ShaderResourceView** getSRV_Texture();
	ID3D11Buffer** getVertexBuffer();
	int getVertCount();

	BoundingVolume* getBoundingVolume();
};