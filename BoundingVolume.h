#pragma once
#include <DirectXMath.h>
#include <Vertex_Pos_Col.h>
#include <vector>
#include <d3d11.h>

class BoundingVolume
{
private:
	DirectX::XMMATRIX _worldMatrix;
	ID3D11Buffer* _vertexBuffer;
	bool _highlighted;
	
protected:
	DirectX::XMFLOAT3 _center;
	std::vector <Vertex_Pos_Col> _vertices;

public:
	BoundingVolume();
	BoundingVolume(ID3D11Device* device);
	~BoundingVolume();

	void setWorldMatrix(DirectX::XMMATRIX worldMatrix);
	void setCenter(DirectX::XMFLOAT3 center);
	void setHighlight(bool highlighted);

	virtual void createVertexBuffer(ID3D11Device* device);
	virtual void transform(DirectX::XMMATRIX worldMatrix);

	ID3D11Buffer** getVertexBuffer();
	DirectX::XMFLOAT3 getCenter() const;
	std::vector <Vertex_Pos_Col> & getVertices();
	int getVertCount();

	virtual float intersectWithRay(DirectX::XMVECTOR rayDir, DirectX::XMVECTOR rayOrigin) = 0;
	virtual bool intersectWithBox(DirectX::XMFLOAT3 center, float halfLength) = 0;
};