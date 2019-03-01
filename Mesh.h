#pragma once
#include "Vertex_Pos_UV_Normal_Tangent.h"
#include "Vertex_Pos_UV_Normal.h"
#include "Vertex_Pos_UV.h"
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
	std::vector <Vertex_Pos_UV_Normal_Tangent> _vertices_Pos_UV_Normal_Tangent;
	std::vector <Vertex_Pos_UV_Normal> _vertices_Pos_UV_Normal;
	std::vector <Vertex_Pos_Col> _vertices_Pos_Col;
	std::vector <Vertex_Pos_UV> _vertices_Pos_UV;

	ID3D11ShaderResourceView* _SRV_Texture;
	ID3D11ShaderResourceView* _SRV_Normal;
	ID3D11Buffer* _vertexBuffer;
	ID3D11Buffer* _vertexBufferNormalMap;
	DirectX::XMMATRIX _modelMatrix;
	BoundingVolume* _boundingVolume;
	void transform();

public:
	Mesh();
	Mesh(std::string filePath, bool flippedUV, bool normalMapped, ID3D11Device* device, boundingVolumes boundingVolumeChoice, DirectX::XMMATRIX modelMatrix);
	Mesh(std::vector <Vertex_Pos_Col> vertices_Pos_Col, ID3D11Device* device);
	Mesh(std::vector <Vertex_Pos_UV> vertices_Pos_UV, ID3D11Device* device);
	~Mesh();

	void setVertices(std::vector <Vertex_Pos_UV_Normal> vertices);

	std::vector <Vertex_Pos_UV_Normal> & getVertices();
	ID3D11ShaderResourceView** getSRV_Texture();
	ID3D11ShaderResourceView** getSRV_Normal();
	ID3D11Buffer** getVertexBuffer();
	ID3D11Buffer** getVertexBufferNormalMap();
	int getVertCount();

	BoundingVolume* getBoundingVolume();
};