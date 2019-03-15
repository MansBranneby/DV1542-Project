#include "BoundingVolume.h"

void BoundingVolume::createVertexBuffer(ID3D11Device* device)
{
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

BoundingVolume::BoundingVolume()
{
}

BoundingVolume::~BoundingVolume()
{
	_vertexBuffer->Release();
}

void BoundingVolume::setCenter(DirectX::XMFLOAT3 center)
{
	_center = center;
}

void BoundingVolume::setHighlight(bool highlighted)
{
	_highlighted = highlighted;
	if (_highlighted == true)
	{
		for (size_t i = 0; i < _vertices.size(); i++)
			_vertices.at(i).setCol(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
	}
	else
	{
		for (size_t i = 0; i < _vertices.size(); i++)
			_vertices.at(i).setCol(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	}
}

ID3D11Buffer ** BoundingVolume::getVertexBuffer()
{
	return &_vertexBuffer;
}

DirectX::XMFLOAT3 BoundingVolume::getCenter() const
{
	return _center;
}

std::vector<Vertex_Pos_Col>& BoundingVolume::getVertices()
{
	return _vertices;
}

size_t BoundingVolume::getVertCount()
{
	return _vertices.size();
}

bool BoundingVolume::getHighlighted()
{
	return _highlighted;
}
