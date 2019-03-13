#pragma once
#include <d3d11.h>

class ConstantBuffer
{
private:
	ID3D11Buffer* _constantBuffer = nullptr;

public:
	ConstantBuffer();
	~ConstantBuffer();
	
	void createConstantBuffer(ID3D11Device* device, const void* data, UINT dataSize);
	ID3D11Buffer** getConstantBuffer();
};