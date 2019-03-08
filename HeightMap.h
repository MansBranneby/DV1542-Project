#pragma once
#include "Vertex_Pos_UV_Normal.h"
 
#include <DirectXMath.h>
#include <DirectXHelpers.h>

// Reading files
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "ConstantBuffer.h"

class Heightmap
{
private:
	int _terrainWidth;
	int _terrainHeight;
	int _terrainSize;
	int _maxValue;
	float _heightFactor;
	float _widthFactor;
	float _depthFactor;
	ID3D11Buffer* _vertexBuffer;
	std::vector <float> _greyValues;
	std::vector <DirectX::XMFLOAT3> _heightmap;
	std::vector <Vertex_Pos_UV_Normal> _vertices_Pos_UV_Normal;
	ConstantBuffer _constantBuffer;

	void loadHeightmap(std::string filePath);
	void createVertexBuffer(ID3D11Device* device);
public:
	Heightmap();
	Heightmap(std::string filePath, float widthFactor, float heightFactor, float depthFactor, ID3D11Device* device);
	~Heightmap();
	
	ID3D11Buffer** getVertexBuffer();
	ID3D11Buffer** getConstantBuffer();
	std::vector <Vertex_Pos_UV_Normal> & getVertices();
	std::vector <DirectX::XMFLOAT3> getHeightmap();
	std::vector <float> getGreyValues();
	float getHeight(float x, float z);

	int getVertCount();
};