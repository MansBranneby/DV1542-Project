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
	std::ifstream inFile(filePath);
	int greyValue = 0;
	// Check if file exists
	if (!inFile)
		MessageBox(NULL, L"LoadHeightMap: unable to locate file.", L"Error", MB_OK | MB_ICONERROR);
	

	// CHECK VERSION, either P2 or P5 //
	// compare() returns:
	// zero if line and argument are equivalent
	// value < 0 if line is shorter than argument
	// value > 0 if line is bigger than argument
	std::getline(inFile, line);
	if(line.compare("P2") != 0)
		MessageBox(NULL, L"LoadHeightMap: faulty version.", L"Error", MB_OK | MB_ICONERROR);

	std::getline(inFile, line);
	inputString.str(line);
	inputString >> _terrainWidth >> _terrainHeight >> _maxValue;

	// number of grey values in file
	_imageSize = _terrainWidth * _terrainHeight;


	// read grey values (brightness of pixels)
	std::getline(inFile, line);
	inputString.str(line);
	for (int i = 0; i < _imageSize; i++)
	{
		inputString >> greyValue;
		_greyValues.push_back(greyValue);
	}
	inputString.clear();
	
	inFile.close();


}
