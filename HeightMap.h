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
	std::vector <float> _greyValues;

	void LoadHeightMap(std::string filePath);

public:
	HeightMap();
	HeightMap(std::string filePath, float heightFactor, float widthFactor, float depthFactor);
	~HeightMap();
};