#pragma once
#include <DirectXMath.h>
#include <Vertex_Pos_Col.h>
#include <vector>
#include <d3d11.h>

class BoundingVolume
{
private:
	ID3D11Buffer* _vertexBuffer;
	bool _highlighted;
	
protected:
	DirectX::XMMATRIX _modelMatrix;
	DirectX::XMFLOAT3 _center;
	std::vector <Vertex_Pos_Col> _vertices;

public:
	BoundingVolume();
	~BoundingVolume();

	void setCenter(DirectX::XMFLOAT3 center);
	void setHighlight(bool highlighted);

	virtual void createVertexBuffer(ID3D11Device* device);

	ID3D11Buffer** getVertexBuffer();
	DirectX::XMFLOAT3 getCenter() const;
	std::vector <Vertex_Pos_Col> & getVertices();
	size_t getVertCount();
	bool getHighlighted();

	virtual void setWorldMatrix(ID3D11Device* device, DirectX::XMMATRIX worldMatrix) = 0;
	virtual void transform(ID3D11Device* device, DirectX::XMMATRIX modelMatrix) = 0;
	virtual float intersectWithRay(DirectX::XMVECTOR rayDir, DirectX::XMVECTOR rayOrigin) = 0;
	virtual bool intersectWithBox(DirectX::XMFLOAT3 center, float halfLength) = 0;
};