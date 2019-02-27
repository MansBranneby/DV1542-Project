#include "HeightMap.h"

HeightMap::HeightMap()
{
}

HeightMap::HeightMap(std::string filePath, float widthFactor, float heightFactor, float depthFactor, ID3D11Device* device)
{
	_widthFactor = widthFactor;
	_heightFactor = heightFactor;
	_depthFactor = depthFactor;

	loadHeightMap(filePath);
	createVertexBuffer(device);
}

HeightMap::~HeightMap()
{
}

ID3D11Buffer ** HeightMap::getVertexBuffer()
{
	return &_vertexBuffer;
}

std::vector<Vertex_Pos_UV_Normal>& HeightMap::getVertices()
{
	return _vertices_Pos_UV_Normal;
}

std::vector<DirectX::XMFLOAT3> HeightMap::getHeightmap()
{
	return _heightmap;
}

std::vector<float> HeightMap::getGreyValues()
{
	return _greyValues;
}

float HeightMap::getHeight(float x, float z)
{
	int index = ((int)z*_depthFactor) * _terrainWidth + (int)x*_widthFactor;

	if (index <= 0 || index == _terrainSize || (int)x * _widthFactor > _terrainWidth || (int)z * _depthFactor > _terrainHeight)
		index = 0;

	return _greyValues[index]/_heightFactor;
}

int HeightMap::getVertCount()
{
	return _vertices_Pos_UV_Normal.size();
}

void HeightMap::loadHeightMap(std::string filePath)
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
		_greyValues.push_back(greyValue);
		inputString.clear();
	}
	inFile.close();

	// Store vertices. Each pixel equals one vertex
	std::vector <DirectX::XMFLOAT3> _heightMap(_terrainSize);
	int k = 0;
	for (int i = 0; i < _terrainHeight; i++)
	{
		for (int j = 0; j < _terrainWidth; j++)
		{
			float width = j / _widthFactor;						// scale x
			float height = _greyValues[k++] / _heightFactor;	// scale y
			float depth = i / _depthFactor;						// scale z

			int index = (i * _terrainWidth + j);

			DirectX::XMFLOAT3 pos = { width, height, depth };
			_heightMap[index] = pos;
		}
	}

	Vertex_Pos_UV_Normal tempVert;

	for (int i = 0; i < _terrainHeight - 1; i++)
	{
		for (int j = 0; j < _terrainWidth - 1; j++)
		{
			// TL 
			tempVert.setPos(_heightMap[i * _terrainWidth + j]);
			_vertices_Pos_UV_Normal.push_back(tempVert);

			// BL
			tempVert.setPos(_heightMap[(i + 1) * _terrainWidth + j]);
			_vertices_Pos_UV_Normal.push_back(tempVert);

			// TR
			tempVert.setPos(_heightMap[i * _terrainWidth + j + 1]);
			_vertices_Pos_UV_Normal.push_back(tempVert);

			// BL
			tempVert.setPos(_heightMap[(i + 1) * _terrainWidth + j]);
			_vertices_Pos_UV_Normal.push_back(tempVert);

			// BR
			tempVert.setPos(_heightMap[(i + 1)* _terrainWidth + j + 1]);
			_vertices_Pos_UV_Normal.push_back(tempVert);
		
			//  TR
			tempVert.setPos(_heightMap[i * _terrainWidth + j + 1]);
			_vertices_Pos_UV_Normal.push_back(tempVert);
		}
	}
}

void HeightMap::createVertexBuffer(ID3D11Device * device)
{
	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	D3D11_SUBRESOURCE_DATA data;
	bufferDesc.ByteWidth = sizeof(Vertex_Pos_UV_Normal) * _vertices_Pos_UV_Normal.size();
	data.pSysMem = _vertices_Pos_UV_Normal.data();
	HRESULT result = device->CreateBuffer(&bufferDesc, &data, &_vertexBuffer);
	if (FAILED(result))
		MessageBox(NULL, L"ERROR _vertexBuffer in HeightMap.cpp", L"Error", MB_OK | MB_ICONERROR);
}
