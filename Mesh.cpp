#include "Mesh.h"
#include "OBB.h"

void Mesh::loadOBJ(std::string filePath, ID3D11Device* device, bool flippedUV, bool normalMapped, boundingVolumes boundingVolumeChoice)
{
	std::vector<DirectX::XMFLOAT3>vtxPos;
	std::vector<DirectX::XMFLOAT2>vtxUV;
	std::vector<DirectX::XMFLOAT3> vtxNormal;
	std::vector<std::string> materialLibs;
	std::vector<std::string> materials;
	std::vector<int> vertexIndices;
	std::vector<int> uvIndices;
	std::vector<int> normalIndices;

	std::ifstream inFile;
	std::string line, special, libraries, material, bump;
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
	for (int i = 0; i < vertexIndices.size(); i++)
	{
		int posIndex = vertexIndices[i];
		int uvIndex = uvIndices[i];
		int normalIndex = normalIndices[i];

		DirectX::XMFLOAT3 vertPos = vtxPos[posIndex - 1];
		DirectX::XMFLOAT3 vertNormal = vtxNormal[normalIndex - 1];
		DirectX::XMFLOAT2 vertUV;
		int vertIndex = posIndex - 1;

		if (flippedUV)
			vertUV = DirectX::XMFLOAT2(vtxUV[uvIndex - 1].x, 1 - vtxUV[uvIndex - 1].y);
		else
			vertUV = vtxUV[uvIndex - 1];

		if (normalMapped)
		{
			Vertex_Pos_UV_Normal_Tangent tempTriangle(vertPos, vertUV, vertNormal, vertIndex);
			_vertices_Pos_UV_Normal_Tangent.push_back(tempTriangle);

			Vertex_Pos_UV_Normal tempTriangleVertex(vertPos, vertUV, vertNormal);
			_vertices_Pos_UV_Normal.push_back(tempTriangleVertex);
		}
		else
		{
			Vertex_Pos_UV_Normal tempTriangleVertex(vertPos, vertUV, vertNormal);
			_vertices_Pos_UV_Normal.push_back(tempTriangleVertex);
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
		else if (line.substr(0, 9) == "map_Bump " || line.substr(0, 5) == "bump ")
		{
			inputString >> special >> bump;
		}
		inputString.clear();
	}

	// Texure
	std::wstring widestr = std::wstring(material.begin(), material.end());
	wchar_t widecstr[1000] = L"Resources\\Textures\\";
	const wchar_t* widecstr1 = widestr.c_str();
	const wchar_t* fileName = wcscat(widecstr, widecstr1);

	HRESULT hr = CoInitialize(NULL);
	hr = DirectX::CreateWICTextureFromFile(device, fileName, NULL, &_SRV_Texture);
	if (FAILED(hr))
		MessageBox(NULL, L"ERROR TEXTURE", L"Error", MB_OK | MB_ICONERROR);

	if (normalMapped)
	{
		// Normal Texture
		std::wstring widestrBump = std::wstring(bump.begin(), bump.end());
		wchar_t widecstrBump[1000] = L"Resources\\Textures\\";
		const wchar_t* widecstrBump1 = widestrBump.c_str();
		const wchar_t* fileNameBump = wcscat(widecstrBump, widecstrBump1);

		hr = CoInitialize(NULL);
		hr = DirectX::CreateWICTextureFromFile(device, fileNameBump, NULL, &_SRV_Normal);
		if (FAILED(hr))
			MessageBox(NULL, L"ERROR NORMAL", L"Error", MB_OK | MB_ICONERROR);
	}
	inFile.close();

	switch (boundingVolumeChoice)
	{
	case ORIENTED_BOUNDING_BOX:
		_boundingVolume = new OBB(smallestXYZ, biggestXYZ, device, _modelMatrix);
		break;

	case AXIS_ALIGNED_BOUNDING_BOX:
		break;

	case SPHERE:
		break;
	}

	if (normalMapped)
	{
		//Initialize
		std::vector<DirectX::XMVECTOR> tangents;
		tangents.resize(_vertices_Pos_UV_Normal_Tangent.size());
		std::fill(tangents.begin(), tangents.end(), DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f));

		//Calculate triangle tangents
		for (int i = 0; i < _vertices_Pos_UV_Normal_Tangent.size(); i += 3)
		{
			// Loading from XMFLOAT3 to XMVECTOR in order to use vector subtraction
			DirectX::XMVECTOR vert0 = DirectX::XMLoadFloat3(&_vertices_Pos_UV_Normal_Tangent.at(i).getPos());
			DirectX::XMVECTOR vert1 = DirectX::XMLoadFloat3(&_vertices_Pos_UV_Normal_Tangent.at(i + 1).getPos());
			DirectX::XMVECTOR vert2 = DirectX::XMLoadFloat3(&_vertices_Pos_UV_Normal_Tangent.at(i + 2).getPos());

			// Pos
			DirectX::XMVECTOR v0 = DirectX::XMVectorSubtract(vert1, vert0);
			DirectX::XMVECTOR v1 = DirectX::XMVectorSubtract(vert2, vert0);

			// UV
			DirectX::XMVECTOR uv0 = DirectX::XMLoadFloat2(&_vertices_Pos_UV_Normal_Tangent.at(i).getUV());
			DirectX::XMVECTOR uv1 = DirectX::XMLoadFloat2(&_vertices_Pos_UV_Normal_Tangent.at(i + 1).getUV());
			DirectX::XMVECTOR uv2 = DirectX::XMLoadFloat2(&_vertices_Pos_UV_Normal_Tangent.at(i + 2).getUV());

			// U
			float s1 = DirectX::XMVectorGetX(uv1) - DirectX::XMVectorGetX(uv0);
			float s2 = DirectX::XMVectorGetX(uv2) - DirectX::XMVectorGetX(uv0);

			// V
			float t1 = DirectX::XMVectorGetY(uv1) - DirectX::XMVectorGetY(uv0);
			float t2 = DirectX::XMVectorGetY(uv2) - DirectX::XMVectorGetY(uv0);

			float r = 1.0f / (s1 * t2 - s2 * t1);

			// Tangent
			DirectX::XMVECTOR faceTangent = DirectX::XMVectorSet(
				(t2 * DirectX::XMVectorGetX(v0) - t1 * DirectX::XMVectorGetX(v1)) * r,
				(t2 * DirectX::XMVectorGetY(v0) - t1 * DirectX::XMVectorGetY(v1)) * r,
				(t2 * DirectX::XMVectorGetZ(v0) - t1 * DirectX::XMVectorGetZ(v1)) * r, 1.0f);

			tangents.at(_vertices_Pos_UV_Normal_Tangent.at(i).getVertIndex()) = DirectX::XMVectorAdd(tangents.at(_vertices_Pos_UV_Normal_Tangent.at(i).getVertIndex()), faceTangent);
		}
		for (int i = 0; i < _vertices_Pos_UV_Normal_Tangent.size(); i++)
		{
			DirectX::XMFLOAT3 tangent = {
				DirectX::XMVectorGetX(tangents.at(_vertices_Pos_UV_Normal_Tangent.at(i).getVertIndex())),
				DirectX::XMVectorGetY(tangents.at(_vertices_Pos_UV_Normal_Tangent.at(i).getVertIndex())),
				DirectX::XMVectorGetZ(tangents.at(_vertices_Pos_UV_Normal_Tangent.at(i).getVertIndex())) };

			_vertices_Pos_UV_Normal_Tangent.at(i).setTangent(tangent);
		}
	}
}

void Mesh::transform(ID3D11Device* device, bool normalMapped)
{
	if (_vertices_Pos_UV_Normal_Tangent.size() > 0)
	{
		for (int i = 0; i < getVertCount(); i++)
		{
			DirectX::XMVECTOR posWS = DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&_vertices_Pos_UV_Normal_Tangent[i].getPos()), _modelMatrix);
			_vertices_Pos_UV_Normal_Tangent[i].setPos({ DirectX::XMVectorGetX(posWS), DirectX::XMVectorGetY(posWS), DirectX::XMVectorGetZ(posWS) });
		}
	}
	if (_vertices_Pos_UV_Normal.size() > 0)
	{
		for (int i = 0; i < getVertCount(); i++)
		{
			DirectX::XMVECTOR posWS = DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&_vertices_Pos_UV_Normal[i].getPos()), _modelMatrix);
			_vertices_Pos_UV_Normal[i].setPos({ DirectX::XMVectorGetX(posWS), DirectX::XMVectorGetY(posWS), DirectX::XMVectorGetZ(posWS) });
		}
	}
	if (_vertices_Pos_Col.size() > 0)
	{
		for (int i = 0; i < getVertCount(); i++)
		{
			DirectX::XMVECTOR posWS = DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&_vertices_Pos_Col[i].getPos()), _modelMatrix);
			_vertices_Pos_Col[i].setPos({ DirectX::XMVectorGetX(posWS), DirectX::XMVectorGetY(posWS), DirectX::XMVectorGetZ(posWS) });
		}
	}
	if (_vertices_Pos_UV.size() > 0)
	{
		for (int i = 0; i < getVertCount(); i++)
		{
			DirectX::XMVECTOR posWS = DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&_vertices_Pos_UV[i].getPos()), _modelMatrix);
			_vertices_Pos_UV[i].setPos({ DirectX::XMVectorGetX(posWS), DirectX::XMVectorGetY(posWS), DirectX::XMVectorGetZ(posWS) });
		}
	}

	getBoundingVolume()->setWorldMatrix(device, _modelMatrix);
}

void Mesh::createVertexBuffer(ID3D11Device* device, ID3D11DeviceContext* deviceContext, bool normalMapped)
{
	// VERTEX BUFFER
//
	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	D3D11_SUBRESOURCE_DATA data;

	bufferDesc.ByteWidth = sizeof(Vertex_Pos_UV_Normal) * _vertices_Pos_UV_Normal.size();
	data.pSysMem = _vertices_Pos_UV_Normal.data();

	HRESULT result = device->CreateBuffer(&bufferDesc, &data, &_vertexBuffer);
	if (FAILED(result))
		MessageBox(NULL, L"ERROR _vertexBuffer in Mesh.cpp", L"Error", MB_OK | MB_ICONERROR);

	if (normalMapped)
	{
		bufferDesc.ByteWidth = sizeof(Vertex_Pos_UV_Normal_Tangent) * _vertices_Pos_UV_Normal_Tangent.size();
		data.pSysMem = _vertices_Pos_UV_Normal_Tangent.data();

		result = device->CreateBuffer(&bufferDesc, &data, &_vertexBufferNormalMap);
		if (FAILED(result))
			MessageBox(NULL, L"ERROR _vertexBufferNormalMap in Mesh.cpp", L"Error", MB_OK | MB_ICONERROR);
	}
}

Mesh::Mesh()
{
}

Mesh::Mesh(std::string filePath, bool flippedUV, bool normalMapped, ID3D11Device* device, ID3D11DeviceContext* deviceContext, boundingVolumes boundingVolumeChoice, DirectX::XMMATRIX modelMatrix)
{
	_modelMatrix = modelMatrix;
	
	loadOBJ(filePath, device, flippedUV, normalMapped, boundingVolumeChoice);
	transform(device, normalMapped);
	getBoundingVolume()->setWorldMatrix(device, modelMatrix);
	createVertexBuffer(device, deviceContext, normalMapped);
}

Mesh::Mesh(std::vector<Vertex_Pos_Col> vertices_Pos_Col, ID3D11Device * device)
{
	_vertices_Pos_Col = vertices_Pos_Col;

	// Describe the Vertex Buffer
	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	D3D11_SUBRESOURCE_DATA data;
	// billboard
	bufferDesc.ByteWidth = sizeof(Vertex_Pos_Col) * _vertices_Pos_Col.size();
	data.pSysMem = _vertices_Pos_Col.data();
	HRESULT result = device->CreateBuffer(&bufferDesc, &data, &_vertexBuffer);
	if (FAILED(result))
		MessageBox(NULL, L"Error vertexBuffer in Mesh.cpp", L"Error", MB_OK | MB_ICONERROR);
	
}

Mesh::~Mesh()
{
}

void Mesh::setVertices(std::vector<Vertex_Pos_UV_Normal> vertices)
{
	_vertices_Pos_UV_Normal = vertices;
}

void Mesh::setModelMatrix(ID3D11Device* device, ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX modelMatrix, bool normalMapped)
{
	_modelMatrix = modelMatrix;
	transform(device, normalMapped);
	createVertexBuffer(device, deviceContext, normalMapped);
}

std::vector <Vertex_Pos_UV_Normal> & Mesh::getVertices()
{
	return _vertices_Pos_UV_Normal;
}

ID3D11ShaderResourceView* *Mesh::getSRV_Texture()
{
	return &_SRV_Texture;
}

ID3D11ShaderResourceView ** Mesh::getSRV_Normal()
{
	return &_SRV_Normal;
}

ID3D11Buffer* *Mesh::getVertexBuffer()
{
	return &_vertexBuffer;
}

ID3D11Buffer ** Mesh::getVertexBufferNormalMap()
{
	return &_vertexBufferNormalMap;
}

int Mesh::getVertCount()
{
	if (_vertices_Pos_UV_Normal_Tangent.size() > 0)
		return _vertices_Pos_UV_Normal_Tangent.size();
	else if (_vertices_Pos_UV_Normal.size() > 0)
		return  _vertices_Pos_UV_Normal.size();
	else if (_vertices_Pos_Col.size() > 0)
		return  _vertices_Pos_Col.size();
	else if (_vertices_Pos_UV.size() > 0)
		return  _vertices_Pos_UV.size();
	else
		return 0;
}

DirectX::XMMATRIX Mesh::getModelMatrix()
{
	return _modelMatrix;
}

BoundingVolume * Mesh::getBoundingVolume()
{
	return _boundingVolume;
}
