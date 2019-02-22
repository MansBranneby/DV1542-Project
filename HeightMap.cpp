#include "HeightMap.h"

HeightMap::HeightMap()
{
}

HeightMap::HeightMap(std::string filePath)
{
	LoadHeightMap(filePath);
}

HeightMap::~HeightMap()
{
}

void HeightMap::LoadHeightMap(std::string filePath)
{
	std::string line;
	std::istringstream inputString;

	std::ifstream inFile("heightmap.pgm");

	if (!inFile.is_open())
		MessageBox(NULL, L"LoadHeightMap: unable to locate file.", L"Error", MB_OK | MB_ICONERROR);

	inFile.open(filePath);

	//std::getline(inFile, line);
	while (std::getline(inFile, line))

	//inputString.str(line);
	if(line.substr(0, 2) != "P2")
		MessageBox(NULL, L"LoadHeightMap: faulty version.", L"Error", MB_OK | MB_ICONERROR);

	std::getline(inFile, line);
	inputString.str(line);
	inputString >> _terrainWidth >> _terrainHeight >> _maxValue;

	inFile.close();
}
