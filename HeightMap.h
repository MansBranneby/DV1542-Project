#pragma once

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
	int _imageSize;
	int _maxValue;
	std::vector <int> _greyValues;

	void LoadHeightMap(std::string filePath);

public:
	HeightMap();
	HeightMap(std::string filePath);
	~HeightMap();
};