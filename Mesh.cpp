#include "Mesh.h"
#include "OBB.h"

Mesh::Mesh()
{
}

Mesh::Mesh(std::string filePath, bool flippedUV, bool normalMapped, ID3D11Device* device, boundingVolumes boundingVolumeChoice)
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
	for (int i = 0; i < vertexIndices.size(); i++)
	{
		int posIndex = vertexIndices[i];
		int uvIndex = uvIndices[i];
		int normalIndex = normalIndices[i];

		DirectX::XMFLOAT3 vertPos = vtxPos[posIndex - 1];
		DirectX::XMFLOAT3 vertNormal = vtxNormal[normalIndex - 1];
		DirectX::XMFLOAT2 vertUV;

		if(flippedUV)
			vertUV = DirectX::XMFLOAT2(vtxUV[uvIndex - 1].x, 1 - vtxUV[uvIndex - 1].y);
		else
			vertUV = vtxUV[uvIndex - 1];
			
		if (normalMapped)
		{
			Vertex_Pos_UV_Normal_Tangent_BiTangent tempTriangle(vertPos, vertUV, vertNormal);
			_vertices_Pos_UV_Normal_Tangent_BiTangent.push_back(tempTriangle);
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
		MessageBox(NULL, L"ERROR TEXTURE", L"Error", MB_OK | MB_ICONERROR);

	// VERTEX BUFFER
	//
	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(Vertex_Pos_UV_Normal) * _vertices_Pos_UV_Normal.size();
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = _vertices_Pos_UV_Normal.data();
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
		_boundingVolume = new OBB(center, half_u_v_w, vertices, device);
		break;

	case AXIS_ALIGNED_BOUNDING_BOX:
		break;

	case SPHERE:
		break;
	}


	if (normalMapped)
	{
		//Calculate triangle tangents
		for (int i = 0; i < _vertices_Pos_UV_Normal.size() / 3; i += 2)
		{
			// Loading from XMFLOAT3 to XMVECTOR in order to use vector subtraction
			DirectX::XMVECTOR vert1 = DirectX::XMLoadFloat3(&_vertices_Pos_UV_Normal.at(i).getPos());
			DirectX::XMVECTOR vert2 = DirectX::XMLoadFloat3(&_vertices_Pos_UV_Normal.at(i + 1).getPos());
			DirectX::XMVECTOR vert3 = DirectX::XMLoadFloat3(&_vertices_Pos_UV_Normal.at(i + 2).getPos());

			// Pos
			DirectX::XMVECTOR v1 = DirectX::XMVectorSubtract(vert2, vert1);
			DirectX::XMVECTOR v2 = DirectX::XMVectorSubtract(vert3, vert1);
		
			// UV
			vert1 = DirectX::XMLoadFloat2(&_vertices_Pos_UV_Normal.at(i).getUV());
			vert2 = DirectX::XMLoadFloat2(&_vertices_Pos_UV_Normal.at(i + 1).getUV());
			vert3 = DirectX::XMLoadFloat2(&_vertices_Pos_UV_Normal.at(i + 2).getUV());

			 // U
			float s1 = DirectX::XMVectorGetX(vert2) - DirectX::XMVectorGetX(vert1);
			float s2 = DirectX::XMVectorGetX(vert3) - DirectX::XMVectorGetX(vert1);

			 // V
			float t1 = DirectX::XMVectorGetY(vert2) - DirectX::XMVectorGetY(vert1);
			float t2 = DirectX::XMVectorGetY(vert3) - DirectX::XMVectorGetY(vert1);

			float r = 1.0f / (s1 * t2 - s2 * t1);
			DirectX::XMVECTOR faceTangent = DirectX::XMVectorSet(r * t2 * DirectX::XMVectorGetX(vert1) - t1 * DirectX::XMVectorGetX(vert2), r * t2 * DirectX::XMVectorGetY(vert1) - t1 * DirectX::XMVectorGetY(vert2), r * t2 * DirectX::XMVectorGetZ(vert1) - t1 * DirectX::XMVectorGetZ(vert2), 1.0f);
			DirectX::XMVECTOR faceBiTangent = DirectX::XMVectorSet(r * -s2 * DirectX::XMVectorGetX(vert1) + s1 * DirectX::XMVectorGetX(vert2), r * -s2 * DirectX::XMVectorGetY(vert1) + s1 * DirectX::XMVectorGetY(vert2), r * -s2 * DirectX::XMVectorGetZ(vert1) + s1 * DirectX::XMVectorGetZ(vert2), 1.0f);	

			//tangent += newTangent
			_vertices_Pos_UV_Normal_Tangent_BiTangent.at(i).setTangent(DirectX::XMFLOAT3(DirectX::XMVectorGetX(faceTangent) + _vertices_Pos_UV_Normal_Tangent_BiTangent.at(i).getTangent().x, DirectX::XMVectorGetY(faceTangent) + _vertices_Pos_UV_Normal_Tangent_BiTangent.at(i).getTangent().y, DirectX::XMVectorGetZ(faceTangent) + _vertices_Pos_UV_Normal_Tangent_BiTangent.at(i).getTangent().z));
			_vertices_Pos_UV_Normal_Tangent_BiTangent.at(i + 1).setTangent(DirectX::XMFLOAT3(DirectX::XMVectorGetX(faceTangent) + _vertices_Pos_UV_Normal_Tangent_BiTangent.at(i + 1).getTangent().x, DirectX::XMVectorGetY(faceTangent) + _vertices_Pos_UV_Normal_Tangent_BiTangent.at(i + 1).getTangent().y, DirectX::XMVectorGetZ(faceTangent) + _vertices_Pos_UV_Normal_Tangent_BiTangent.at(i + 1).getTangent().z));
			_vertices_Pos_UV_Normal_Tangent_BiTangent.at(i + 2).setTangent(DirectX::XMFLOAT3(DirectX::XMVectorGetX(faceTangent) + _vertices_Pos_UV_Normal_Tangent_BiTangent.at(i + 2).getTangent().x, DirectX::XMVectorGetY(faceTangent) + _vertices_Pos_UV_Normal_Tangent_BiTangent.at(i + 2).getTangent().y, DirectX::XMVectorGetZ(faceTangent) + _vertices_Pos_UV_Normal_Tangent_BiTangent.at(i + 2).getTangent().z));
			
			//biTangent += newBiTangent
			_vertices_Pos_UV_Normal_Tangent_BiTangent.at(i).setBiTangent(DirectX::XMFLOAT3(DirectX::XMVectorGetX(faceBiTangent) + _vertices_Pos_UV_Normal_Tangent_BiTangent.at(i).getBiTangent().x, DirectX::XMVectorGetY(faceBiTangent) + _vertices_Pos_UV_Normal_Tangent_BiTangent.at(i).getBiTangent().y, DirectX::XMVectorGetZ(faceBiTangent) + _vertices_Pos_UV_Normal_Tangent_BiTangent.at(i).getBiTangent().z));
			_vertices_Pos_UV_Normal_Tangent_BiTangent.at(i + 1).setBiTangent(DirectX::XMFLOAT3(DirectX::XMVectorGetX(faceBiTangent) + _vertices_Pos_UV_Normal_Tangent_BiTangent.at(i + 1).getBiTangent().x, DirectX::XMVectorGetY(faceBiTangent) + _vertices_Pos_UV_Normal_Tangent_BiTangent.at(i + 1).getBiTangent().y, DirectX::XMVectorGetZ(faceBiTangent) + _vertices_Pos_UV_Normal_Tangent_BiTangent.at(i + 1).getBiTangent().z));
			_vertices_Pos_UV_Normal_Tangent_BiTangent.at(i + 2).setBiTangent(DirectX::XMFLOAT3(DirectX::XMVectorGetX(faceBiTangent) + _vertices_Pos_UV_Normal_Tangent_BiTangent.at(i + 2).getBiTangent().x, DirectX::XMVectorGetY(faceBiTangent) + _vertices_Pos_UV_Normal_Tangent_BiTangent.at(i + 2).getBiTangent().y, DirectX::XMVectorGetZ(faceBiTangent) + _vertices_Pos_UV_Normal_Tangent_BiTangent.at(i + 2).getBiTangent().z));
		}
	}
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
		MessageBox(NULL, L"Error gBillboardVertexBuffer", L"Error", MB_OK | MB_ICONERROR);
	
}

Mesh::Mesh(std::vector <Vertex_Pos_UV> vertices_Pos_UV, ID3D11Device* device)
{

}

Mesh::~Mesh()
{
}

void Mesh::setVertices(std::vector<Vertex_Pos_UV_Normal> vertices)
{
	_vertices_Pos_UV_Normal = vertices;
}

std::vector <Vertex_Pos_UV_Normal> & Mesh::getVertices()
{
	return _vertices_Pos_UV_Normal;
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
	return _vertices_Pos_UV_Normal.size();
}

BoundingVolume * Mesh::getBoundingVolume()
{
	return _boundingVolume;
}
