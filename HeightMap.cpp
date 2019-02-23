#include "HeightMap.h"

HeightMap::HeightMap()
{
}

HeightMap::HeightMap(std::string filePath, float heightFactor, float widthFactor, float depthFactor)
{
	_heightFactor = heightFactor;
	_widthFactor = widthFactor;
	_depthFactor = depthFactor;

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
	_terrainSize = _terrainWidth * _terrainHeight;


	// read grey values (brightness of pixels)
	std::getline(inFile, line);
	inputString.str(line);
	for (int i = 0; i < _terrainSize; i++)
	{
		inputString >> greyValue;
		_greyValues.push_back(greyValue); // divide with heightFactor to get desired scale of height. heightFactor is set in the HeightMap constructor
	}
	inputString.clear();
	inFile.close();

	// Store vertices. Each pixel equals one vertex
	std::vector <Vertex_Pos_UV_Normal> _vertices(_terrainSize);

	for (int i = 0; i < _terrainHeight; i++)
	{
		for (int j = 0; j < _terrainWidth; j++)
		{
			float width = j / _widthFactor;		// scale x
			float height = _greyValues.at(i);	// scale y
			float depth = i / _depthFactor;		//scale z

			int index = (i * _terrainHeight + j);

			DirectX::XMFLOAT3 pos = { width, height, depth };
			_vertices.at(index).setPos(pos);

		}
	}


	
}
