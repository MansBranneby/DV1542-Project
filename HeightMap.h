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

class HeightMap
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
	std::vector <Vertex_Pos_UV_Normal> _vertices_Pos_UV_Normal;

	void loadHeightMap(std::string filePath);
	void createVertexBuffer(ID3D11Device* device);
public:
	HeightMap();
	HeightMap(std::string filePath, float widthFactor, float heightFactor, float depthFactor, ID3D11Device* device);
	~HeightMap();
	
	ID3D11Buffer** getVertexBuffer();
	int getVertCount();
};