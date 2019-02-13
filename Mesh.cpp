#include "Mesh.h"

Mesh::Mesh()
{
}

Mesh::Mesh(std::string filePath, bool flippedUV, ID3D11ShaderResourceView** resourceView, ID3D11Device* device)
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
	std::string line, special, libraries, material;
	std::istringstream inputString;
	DirectX::XMFLOAT3 tempPos, tempNormal;
	DirectX::XMFLOAT2 tempUV;

	inFile.open(filePath);
	while (std::getline(inFile, line))
	{
		inputString.str(line);
		if (line.substr(0, 2) == "v ")
		{
			inputString >> special >> tempPos.x >> tempPos.y >> tempPos.z;
			vtxPos.push_back(tempPos);
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


			inputString >> skip; // Fortsättning av OBJLoader
			for (int i = 0; i < size - 1; i++)
				inputString >> vertexIndex[i] >> skip >> uvIndex[i] >> skip >> normalIndex[i];

			if (size == 4)
			{
				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[1]);
				vertexIndices.push_back(vertexIndex[2]);
				uvIndices.push_back(uvIndex[0]);
				uvIndices.push_back(uvIndex[1]);
				uvIndices.push_back(uvIndex[2]);
				normalIndices.push_back(normalIndex[0]);
				normalIndices.push_back(normalIndex[1]);
				normalIndices.push_back(normalIndex[2]);
			}
			else if (size == 5)
			{
				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[1]);
				vertexIndices.push_back(vertexIndex[2]);
				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[2]);
				vertexIndices.push_back(vertexIndex[3]);
				uvIndices.push_back(uvIndex[0]);
				uvIndices.push_back(uvIndex[1]);
				uvIndices.push_back(uvIndex[2]);
				uvIndices.push_back(uvIndex[0]);
				uvIndices.push_back(uvIndex[2]);
				uvIndices.push_back(uvIndex[3]);
				normalIndices.push_back(normalIndex[0]);
				normalIndices.push_back(normalIndex[1]);
				normalIndices.push_back(normalIndex[2]);
				normalIndices.push_back(normalIndex[0]);
				normalIndices.push_back(normalIndex[2]);
				normalIndices.push_back(normalIndex[3]);
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
	std::vector<TriangleVertex>triangles;
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

			TriangleVertex tempTriangle(vertPos, vertUV, vertNormal);
			triangles.push_back(tempTriangle);
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

			TriangleVertex tempTriangle(vertPos, vertUV, vertNormal);
			triangles.push_back(tempTriangle);
		}
	}
	inFile.close();

	// Read materials
	filePath = "Resources\\MaterialLibraries\\" + materialLibs[0];
	inFile.open(filePath);
	while (std::getline(inFile, line))
	{
		inputString.str(line);
		if (line.substr(0, 7) == "map_Kd ")
		{
			inputString >> special >> material;
		}
	}

	std::wstring widestr = std::wstring(material.begin(), material.end());
	wchar_t widecstr[1000] = L"Resources\\Textures\\";
	const wchar_t* widecstr1 = widestr.c_str();
	const wchar_t* fileName = wcscat(widecstr, widecstr1);

	HRESULT hr = CoInitialize(NULL);
	hr = DirectX::CreateWICTextureFromFile(device, fileName, NULL, resourceView);
	if (FAILED(hr))
		MessageBox(NULL, L"ERROR TEXTURE", L"Error", MB_OK | MB_ICONERROR);

	_vertices = triangles;
}

Mesh::~Mesh()
{
}

void Mesh::setVertices(std::vector<TriangleVertex> vertices)
{
	_vertices = vertices;
}

void Mesh::setHitBox()
{

}

std::vector <TriangleVertex> & Mesh::getVertices()
{
	return _vertices;
}

int Mesh::getVertCount()
{
	return _vertices.size();
}