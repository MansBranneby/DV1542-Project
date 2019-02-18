#include "Mesh.h"
#include "OBB.h"

Mesh::Mesh()
{
}

Mesh::Mesh(std::string filePath, bool flippedUV, ID3D11Device* device, boundingVolumes boundingVolumeChoice)
{
	std::vector<DirectX::XMFLOAT3>vtxPos;
	std::vector<DirectX::XMFLOAT2>vtxUV;
	std::vector<DirectX::XMFLOAT3> vtxNormal;
	std::vector<std::string> materialLibs;
	std::vector<std::string> materials;
	std::vector<std::string> bump;
	std::vector<int> vertexIndices;
	std::vector<int> uvIndices;
	std::vector<int> normalIndices;

	std::ifstream inFile;
	std::string line, special, libraries, material;
	std::istringstream inputString;
	DirectX::XMFLOAT3 tempPos, tempNormal;
	DirectX::XMFLOAT2 tempUV;
	DirectX::XMFLOAT3 biggestXYZ(0.0, 0.0f, 0.0f), smallestXYZ(0.0f, 0.0f, 0.0f);
	inFile.open(filePath);
	while (std::getline(inFile, line))
	{
		inputString.str(line);
		if (line.substr(0, 2) == "v ")
		{
			inputString >> special >> tempPos.x >> tempPos.y >> tempPos.z;
			vtxPos.push_back(tempPos);

			if (biggestXYZ.x < tempPos.x)
				biggestXYZ.x = tempPos.x;
			if (biggestXYZ.y < tempPos.y)
				biggestXYZ.y = tempPos.y;
			if (biggestXYZ.z < tempPos.z)
				biggestXYZ.z = tempPos.z;

			if (smallestXYZ.x > tempPos.x)
				smallestXYZ.x = tempPos.x;
			if (smallestXYZ.y > tempPos.y)
				smallestXYZ.y = tempPos.y;
			if (smallestXYZ.z > tempPos.z)
				smallestXYZ.z = tempPos.z;
		}
		else if (line.substr(0, 3) == "vt ")
		{
			inputString >> special >> tempUV.x >> tempUV.y;
			vtxUV.push_back(tempUV);
		}
		else if (line.substr(0, 3) == "vn ")
		{
			inputString >> special >> tempNormal.x >> tempNormal.y >> tempNormal.z;
			vtxNormal.push_back(tempNormal);
		}
		else if (line.substr(0, 2) == "f ")
		{
			int vertexIndex[3], uvIndex[3], normalIndex[3];
			char skip;
			int size = std::distance(std::istream_iterator<std::string>(inputString), std::istream_iterator<std::string>());
			inputString.clear();
			inputString.str(line);
			inputString >> skip;
			for (int i = 0; i < size - 1; i++)
				inputString >> vertexIndex[i] >> skip >> uvIndex[i] >> skip >> normalIndex[i];

			if (size == 4)
			{
				for (int i = 0; i < 3; i++)
				{
					vertexIndices.push_back(vertexIndex[i]);
					uvIndices.push_back(uvIndex[i]);
					normalIndices.push_back(normalIndex[i]);
				}
			}
			else if (size == 5)
			{
				for (int i = 1; i < 3; i++)
				{
					vertexIndices.push_back(vertexIndex[0]);
					vertexIndices.push_back(vertexIndex[i]);
					vertexIndices.push_back(vertexIndex[i + 1]);

					uvIndices.push_back(uvIndex[0]);
					uvIndices.push_back(uvIndex[i]);
					uvIndices.push_back(uvIndex[i + 1]);

					normalIndices.push_back(normalIndex[0]);
					normalIndices.push_back(normalIndex[i]);
					normalIndices.push_back(normalIndex[i + 1]);
				}
			}
		}
		else if (line.substr(0, 7) == "mtllib ")
		{
			inputString >> special >> libraries;
			materialLibs.push_back(libraries);
		}
		else if (line.substr(0, 7) == "usemtl ")
		{
			inputString >> special >> material;
			materials.push_back(material);
		}
		inputString.clear();
	}
	//Sort
	//std::vector<TriangleVertex>triangles;
	if (flippedUV)
	{
		for (int i = 0; i < vertexIndices.size(); i++)
		{
			int posIndex = vertexIndices[i];
			int uvIndex = uvIndices[i];
			int normalIndex = normalIndices[i];

			DirectX::XMFLOAT3 vertPos = vtxPos[posIndex - 1];
			DirectX::XMFLOAT2 vertUV = DirectX::XMFLOAT2(vtxUV[uvIndex - 1].x, 1 - vtxUV[uvIndex - 1].y);
			DirectX::XMFLOAT3 vertNormal = vtxNormal[normalIndex - 1];

			Vertex* tempTriangleVertex = new Vertex_Pos_UV_Normal(vertPos, vertUV, vertNormal);
			_vertices.push_back(tempTriangleVertex);
		}
	}
	else
	{
		for (int i = 0; i < vertexIndices.size(); i++)
		{
			int posIndex = vertexIndices[i];
			int uvIndex = uvIndices[i];
			int normalIndex = normalIndices[i];

			DirectX::XMFLOAT3 vertPos = vtxPos[posIndex - 1];
			DirectX::XMFLOAT2 vertUV = vtxUV[uvIndex - 1];
			DirectX::XMFLOAT3 vertNormal = vtxNormal[normalIndex - 1];

			Vertex* tempTriangleVertex = new Vertex_Pos_UV_Normal(vertPos, vertUV, vertNormal);
			_vertices.push_back(tempTriangleVertex);
		}
	}
	inFile.close();

	// Read materials
	filePath = "Resources\\MTL files\\" + materialLibs[0];
	inFile.open(filePath);
	while (std::getline(inFile, line))
	{
		inputString.str(line);
		if (line.substr(0, 7) == "map_Kd ")
		{
			inputString >> special >> material;

		}
		/*if (line.substr(0, 5) == "bump ")
		{
			inputString >> special >> material;
		}*/
	}

	std::wstring widestr = std::wstring(material.begin(), material.end());
	wchar_t widecstr[1000] = L"Resources\\Textures\\";
	const wchar_t* widecstr1 = widestr.c_str();
	const wchar_t* fileName = wcscat(widecstr, widecstr1);

	HRESULT hr = CoInitialize(NULL);
	hr = DirectX::CreateWICTextureFromFile(device, fileName, NULL, &_SRV_Texture);
	if (FAILED(hr))
		MessageBox(NULL, L"ERROR TEXTURE -> Mesh.cpp", L"Error", MB_OK | MB_ICONERROR);

	// VERTEX BUFFER
	//
	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(Vertex_Pos_UV_Normal) * _vertices.size();
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &_vertices[0];
	HRESULT result = device->CreateBuffer(&bufferDesc, &data, &_vertexBuffer);
	if (FAILED(result))
		MessageBox(NULL, L"ERROR _vertexBuffer in Mesh.cpp", L"Error", MB_OK | MB_ICONERROR);

	// BOUNDING VOLUME
	//
	DirectX::XMFLOAT3 halfXYZ;
	halfXYZ.x = (abs(smallestXYZ.x) + abs(biggestXYZ.x)) / 2;
	halfXYZ.y = (abs(smallestXYZ.y) + abs(biggestXYZ.y)) / 2;
	halfXYZ.z = (abs(smallestXYZ.z) + abs(biggestXYZ.z)) / 2;

	DirectX::XMVECTOR center = DirectX::XMVectorSet(0.0f, halfXYZ.y, 0.0f, 1.0f);
	DirectX::XMVECTOR half_u_v_w = DirectX::XMVectorSet(halfXYZ.x, halfXYZ.y, halfXYZ.z, 0.0f);

	DirectX::XMFLOAT3 col(0.0f, 0.0f, 0.0f);
	DirectX::XMFLOAT3 rightUpNear(biggestXYZ.x, biggestXYZ.y, smallestXYZ.z);
	DirectX::XMFLOAT3 rightDownNear(biggestXYZ.x, smallestXYZ.y, smallestXYZ.z);
	DirectX::XMFLOAT3 leftUpNear(smallestXYZ.x, biggestXYZ.y, smallestXYZ.z);
	DirectX::XMFLOAT3 leftDownNear(smallestXYZ.x, smallestXYZ.y, smallestXYZ.z);

	DirectX::XMFLOAT3 rightUpFar(biggestXYZ.x, biggestXYZ.y, biggestXYZ.z);
	DirectX::XMFLOAT3 rightDownFar(biggestXYZ.x, smallestXYZ.y, biggestXYZ.z);
	DirectX::XMFLOAT3 leftUpFar(smallestXYZ.x, biggestXYZ.y, biggestXYZ.z);
	DirectX::XMFLOAT3 leftDownFar(smallestXYZ.x, smallestXYZ.y, biggestXYZ.z);
	
	std::vector <Vertex_Pos_Col> vertices;
	vertices.push_back(Vertex_Pos_Col(rightUpNear, col));
	vertices.push_back(Vertex_Pos_Col(rightDownNear, col));
	vertices.push_back(Vertex_Pos_Col(rightDownNear, col));
	vertices.push_back(Vertex_Pos_Col(leftDownNear, col));
	vertices.push_back(Vertex_Pos_Col(leftDownNear, col));
	vertices.push_back(Vertex_Pos_Col(leftUpNear, col));
	vertices.push_back(Vertex_Pos_Col(leftUpNear, col));
	vertices.push_back(Vertex_Pos_Col(rightUpNear, col));
	vertices.push_back(Vertex_Pos_Col(rightUpNear, col));
	vertices.push_back(Vertex_Pos_Col(rightUpFar, col));
	vertices.push_back(Vertex_Pos_Col(rightUpFar, col));
	vertices.push_back(Vertex_Pos_Col(rightDownFar, col));
	vertices.push_back(Vertex_Pos_Col(rightDownFar, col));
	vertices.push_back(Vertex_Pos_Col(rightDownNear, col));
	vertices.push_back(Vertex_Pos_Col(rightDownFar, col));
	vertices.push_back(Vertex_Pos_Col(leftDownFar, col));
	vertices.push_back(Vertex_Pos_Col(leftDownFar, col));
	vertices.push_back(Vertex_Pos_Col(leftDownNear, col));
	vertices.push_back(Vertex_Pos_Col(leftDownFar, col));
	vertices.push_back(Vertex_Pos_Col(leftUpFar, col));
	vertices.push_back(Vertex_Pos_Col(leftUpFar, col));
	vertices.push_back(Vertex_Pos_Col(leftUpNear, col));
	vertices.push_back(Vertex_Pos_Col(leftUpFar, col));
	vertices.push_back(Vertex_Pos_Col(rightUpFar, col));

	switch (boundingVolumeChoice)
	{
	case ORIENTED_BOUNDING_BOX:
		_boundingVolume = new OBB(center, half_u_v_w, vertices);
		break;

	case AXIS_ALIGNED_BOUNDING_BOX:
		break;

	case SPHERE:
		break;
	}
}

Mesh::Mesh(std::vector<Vertex*> vertices, ID3D11Device* device)
{
	_vertices = vertices;

	// Describe the Vertex Buffer
	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	D3D11_SUBRESOURCE_DATA data;

	bufferDesc.ByteWidth = sizeof(_vertices.size());
	data.pSysMem = &_vertices[0];
	HRESULT result = device->CreateBuffer(&bufferDesc, &data, &_vertexBuffer);
	if (FAILED(result))
		MessageBox(NULL, L"Error gBillboardVertexBuffer", L"Error", MB_OK | MB_ICONERROR);
}

Mesh::~Mesh()
{
}

void Mesh::setVertices(std::vector<Vertex*> vertices)
{
	_vertices = vertices;
}

std::vector <Vertex*> & Mesh::getVertices()
{
	return _vertices;
}

ID3D11ShaderResourceView* *Mesh::getSRV_Texture()
{
	return &_SRV_Texture;
}

ID3D11Buffer* *Mesh::getVertexBuffer()
{
	return &_vertexBuffer;
}

int Mesh::getVertCount()
{
	return _vertices.size();
}

BoundingVolume * Mesh::getBoundingVolume()
{
	return _boundingVolume;
}
