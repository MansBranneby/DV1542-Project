#pragma once

#include <DirectXMath.h>
#include <DirectXHelpers.h>

// Reading files
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

class HeightMap
{
private:
	int _terrainWidth;
	int _terrainHeight;
	int _maxValue;

	void LoadHeightMap(std::string filePath);

public:
	HeightMap();
	HeightMap(std::string filePath);
	~HeightMap();
};