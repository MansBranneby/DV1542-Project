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
			float width = j / _widthFactor;		// scale x
			float height = _greyValues[k] / _heightFactor;	// scale y
			float depth = i / _depthFactor;		//scale z

			int index = (i * _terrainHeight + j);

			DirectX::XMFLOAT3 pos = { width, height, depth };
			_heightMap[index] = pos;
			k++;
		}
	}

	// Define all triangles with stored vertices
	_vertices_Pos_UV_Normal.resize(_terrainSize);
	for (int i = 0; i < _terrainHeight; i++)
	{
		for (int j = 0; j < _terrainWidth; j++)
		{
			DirectX::XMFLOAT3 pos(_heightMap[i * _terrainWidth + j]);
			_vertices_Pos_UV_Normal[i * _terrainWidth + j].setPos(pos);

			DirectX::XMFLOAT3 normal(0.0f, 1.0f, 0.0f);
			_vertices_Pos_UV_Normal[i * _terrainWidth + j].setNormal(normal);
		}
	}

	int numFaces = (_terrainWidth - 1) * (_terrainHeight - 1) * 2;
	std::vector <int> indices(numFaces * 3);

	// fill UVs
	k = 0;
	int U = 0;
	int V = 0;
	for (int i = 0; i < _terrainHeight - 1; i++)
	{
		for (int j = 0; j < _terrainWidth - 1; j++)
		{
			indices[k] = i * _terrainWidth + j;
			_vertices_Pos_UV_Normal[i * _terrainWidth + j].setUV(DirectX::XMFLOAT2(U, V + 1.0f));


			indices[k + 1] = i * _terrainWidth + j + 1;
			_vertices_Pos_UV_Normal[i * _terrainWidth + j + 1].setUV(DirectX::XMFLOAT2(U + 1.0f, V + 1.0f));


			indices[k + 2] = (i + 1) * _terrainWidth + j;
			_vertices_Pos_UV_Normal[(i + 1)* _terrainWidth + j].setUV(DirectX::XMFLOAT2(U, V));


			indices[k + 3] = (i + 1) * _terrainWidth + j;
			_vertices_Pos_UV_Normal[(i + 1) * _terrainWidth + j].setUV(DirectX::XMFLOAT2(U, V));


			indices[k + 4] = i * _terrainWidth + j + 1;
			_vertices_Pos_UV_Normal[i * _terrainWidth + j + 1].setUV(DirectX::XMFLOAT2(U + 1.0f, V + 1.0f));


			indices[k + 5] = (i + 1) * _terrainWidth + j;
			_vertices_Pos_UV_Normal[(i + 1)* _terrainWidth + j + 1].setUV(DirectX::XMFLOAT2(U + 1.0f, V));

			k += 6;
			U++;
		}
		U = 0;
		V++;
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
