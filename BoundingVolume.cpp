#include "BoundingVolume.h"

BoundingVolume::BoundingVolume()
{
}

BoundingVolume::BoundingVolume(DirectX::XMVECTOR center, std::vector <Vertex_Pos_Col> vertices, ID3D11Device* device)
{
	_center = center;
	_vertices = vertices;

	// Describe the Vertex Buffer
	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	D3D11_SUBRESOURCE_DATA data;

	// bounding volume
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.ByteWidth = sizeof(Vertex_Pos_Col) * _vertices.size();
	data.pSysMem = _vertices.data();
	HRESULT result = device->CreateBuffer(&bufferDesc, &data, &_vertexBuffer);
	if (FAILED(result))
		MessageBox(NULL, L"Error gBillboardVertexBuffer", L"Error", MB_OK | MB_ICONERROR);

}

BoundingVolume::~BoundingVolume()
{
}

void BoundingVolume::setWorldMatrix(DirectX::XMMATRIX worldMatrix)
{
	_worldMatrix = worldMatrix;
}

void BoundingVolume::setCenter(DirectX::XMVECTOR center)
{
	_center = center;
}

void BoundingVolume::setHighlight(bool highlighted)
{
	_highlighted = highlighted;
	if (_highlighted == true)
	{
		for (int i = 0; i < _vertices.size(); i++)
			_vertices.at(i).setCol(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
	}
	else
	{
		for (int i = 0; i < _vertices.size(); i++)
			_vertices.at(i).setCol(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	}
}

ID3D11Buffer ** BoundingVolume::getVertexBuffer()
{
	return &_vertexBuffer;
}

DirectX::XMVECTOR BoundingVolume::getCenter()
{
	return _center;
}

std::vector<Vertex_Pos_Col>& BoundingVolume::getVertices()
{
	return _vertices;
}

int BoundingVolume::getVertCount()
{
	return _vertices.size();
}
