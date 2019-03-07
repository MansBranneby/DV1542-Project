#pragma once
#include "Vertex_Pos_UV_Normal_Tangent.h"
#include "Vertex_Pos_UV_Normal.h"
#include "Vertex_Pos_UV.h"
#include "BoundingVolume.h"
#include "OBB.h"
#include "Material.h"

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
	Material _material;

	void loadOBJ(std::string filePath, ID3D11Device* device, bool flippedUV, bool normalMapped, boundingVolumes boundingVolumeChoice);
	void transform(ID3D11Device* device, bool normalMapped);
	void createVertexBuffer(ID3D11Device* device, ID3D11DeviceContext* deviceContext, bool normalMapped);

public:
	Mesh();
	Mesh(std::string filePath, bool flippedUV, bool normalMapped, ID3D11Device* device, ID3D11DeviceContext* deviceContext, boundingVolumes boundingVolumeChoice, DirectX::XMMATRIX modelMatrix);
	Mesh(std::vector <Vertex_Pos_Col> vertices_Pos_Col, ID3D11Device* device);
	Mesh(std::vector <Vertex_Pos_UV> vertices_Pos_UV, ID3D11Device* device);
	~Mesh();

	void setVertices(std::vector <Vertex_Pos_UV_Normal> vertices);
	void setModelMatrix(ID3D11Device* device, ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX modelMatrix, bool normalMapped);

	std::vector <Vertex_Pos_UV_Normal> & getVertices();
	ID3D11ShaderResourceView** getSRV_Texture();
	ID3D11ShaderResourceView** getSRV_Normal();
	ID3D11Buffer** getVertexBuffer();
	ID3D11Buffer** getVertexBufferNormalMap();
	int getVertCount();
	DirectX::XMMATRIX getModelMatrix();
	BoundingVolume* getBoundingVolume();
};