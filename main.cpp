#pragma once
#include <windows.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include "bth_image.h"

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

// Extra
#include <algorithm>
#include <wchar.h>
// Own classes
#include "Mesh.h"
#include "TriangleVertex.h"

// DirectXTK
#include "CommonStates.h"
#include "DDSTextureLoader.h"
#include "DirectXHelpers.h"
#include "Effects.h"
#include "GamePad.h"
#include "GeometricPrimitive.h"
#include "GraphicsMemory.h"
#include "Keyboard.h"
#include "Model.h"
#include "Mouse.h"
#include "PostProcess.h"
#include "PrimitiveBatch.h"
#include "ScreenGrab.h"
#include "SimpleMath.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "VertexTypes.h"
#include "WICTextureLoader.h"
using namespace DirectX;

using Microsoft::WRL::ComPtr; //Behövs denna?

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

HWND InitWindow(HINSTANCE hInstance);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HRESULT CreateDirect3DContext(HWND wndHandle);

#define WIDTH 768.0f
#define HEIGHT 768.0f

// SwapChain //
IDXGISwapChain* gSwapChain = nullptr;

// Device and DeviceContext //
ID3D11Device* gDevice = nullptr;
ID3D11DeviceContext* gDeviceContext = nullptr;

// VIEWS //
ID3D11DepthStencilView* gDSV = nullptr;

ID3D11RenderTargetView* gBackbufferRTV = nullptr;
ID3D11RenderTargetView* gRenderTargetsDeferred[3] = {};

ID3D11ShaderResourceView *gShaderResourceDeferred[3] = {};
ID3D11ShaderResourceView* gShaderResourceObjTexture = nullptr;

// SAMPLERS //
ID3D11SamplerState *gSamplerState = nullptr;

// BUFFERS //
ID3D11Buffer* gVertexBuffer = nullptr;
ID3D11Buffer* gVertexBufferFSQuad = nullptr;
ID3D11Buffer* gBillboardVertexBuffer = nullptr;
ID3D11Buffer* gConstantBuffer = nullptr;
ID3D11Buffer* gConstantBufferLight = nullptr;
ID3D11Buffer* gConstantBufferCamera = nullptr;
ID3D11Buffer* gConstantBufferBillboard = nullptr;

ID3D11InputLayout* gVertexLayout = nullptr;
ID3D11InputLayout* gVertexLayoutFSQuad = nullptr;
ID3D11InputLayout* gBillboardLayoutPosCol = nullptr;


//TEXTURES
ID3D11Texture2D *gTexDeferredPos = nullptr;
ID3D11Texture2D *gTexDeferredNor = nullptr;
ID3D11Texture2D *gTexDeferredCol = nullptr;

// SHADERS //
ID3D11VertexShader* gVertexShader = nullptr;
ID3D11VertexShader* gVertexShaderSP = nullptr;
ID3D11VertexShader* gBillboardVertexShader = nullptr;
ID3D11PixelShader* gPixelShader = nullptr;
ID3D11PixelShader* gPixelShaderSP = nullptr;
ID3D11PixelShader* gPixelShaderBoundingVolume = nullptr;
ID3D11PixelShader* gPixelShaderBillboard = nullptr;
ID3D11GeometryShader* gGeometryShader = nullptr;
ID3D11GeometryShader* gBillboardGeometryShader = nullptr;

// GLOBALS //
float gFloat = 1.0f;
float gDist = 0.0f;
float gRotation = 0.0f;
float gIncrement = 0;
float gClearColour[3] = {};
Mesh* gPillar = nullptr;

struct PerFrameMatrices {
	XMMATRIX World, WorldViewProj;
};
PerFrameMatrices gMatricesPerFrame;
ID3D11Buffer* gMatrixPerFrameBuffer = nullptr;

struct Lights
{
	XMVECTOR lightPos = { 0.0f, 0.0f, -2.0f };
	XMVECTOR lightCol = { 1.0f, 1.0f, 1.0f };
};
Lights gLight;

struct CameraData
{
	XMVECTOR camPos;
};
CameraData gCameraData;

struct BillboardData
{
	float billboardHeight = 2.0f;
	float billboardWidth = 2.0f;
	float padding1, padding2;
};
BillboardData gBillboardData;

struct TriangleVertexUV
{
	float x, y, z;
	float u, v;
};

struct billboardPoint
{
	float x, y, z;
	float r, g, b;
};

struct Camera
{
	XMVECTOR pos = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX projection;
};
Camera gCamera;

// MESHES //
void createMeshes()
{
	gPillar = new Mesh("Resources\\Meshes\\LP_Pillar_Textured.obj", true, &gShaderResourceObjTexture, gDevice, ORIENTED_BOUNDING_BOX);
}

HRESULT createShaders()
{
	// Binary Large OBject (BLOB), for compiled shader, and errors.
	ID3DBlob* pVS = nullptr;
	ID3DBlob* errorBlob = nullptr;

	// Vertexshader //

	HRESULT result = D3DCompileFromFile(
		L"Vertex.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"VS_main",		// entry point
		"vs_5_0",		// shader model (target)
		D3DCOMPILE_DEBUG,	// shader compile options (DEBUGGING)
		0,				// IGNORE...DEPRECATED.
		&pVS,			// double pointer to ID3DBlob		
		&errorBlob		// pointer for Error Blob messages.
	);
	
	// compilation failed?
	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			// release "reference" to errorBlob interface object
			errorBlob->Release();
		}
		if (pVS)
			pVS->Release();
		return result;
	}

	gDevice->CreateVertexShader(
		pVS->GetBufferPointer(),
		pVS->GetBufferSize(),
		nullptr,
		&gVertexShader
	);
	
	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{
			"POSITION",		// "semantic" name in shader
			0,				// "semantic" index (not used)
			DXGI_FORMAT_R32G32B32_FLOAT, // size of ONE element (3 floats)
			0,							 // input slot
			0,							 // offset of first element
			D3D11_INPUT_PER_VERTEX_DATA, // specify data PER vertex
			0							 // used for INSTANCING (ignore)
		},
		{
			"TEXCOORD",
			0,				// same slot as previous (same vertexBuffer)
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			12,							// offset of FIRST element (after POSITION)
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		{
			"COLOUR",
			0,				// same slot as previous (same vertexBuffer)
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			20,							// offset of FIRST element (after POSITION)
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
	};

	gDevice->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pVS->GetBufferPointer(), pVS->GetBufferSize(), &gVertexLayout);

	pVS->Release();

	pVS = nullptr;
	//Billboard vertex shader
	// https://msdn.microsoft.com/en-us/library/windows/desktop/hh968107(v=vs.85).aspx
	result = D3DCompileFromFile(
		L"BillboardVertexShader.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"VS_main",		// entry point
		"vs_5_0",		// shader model (target)
		D3DCOMPILE_DEBUG,	// shader compile options (DEBUGGING)
		0,				// IGNORE...DEPRECATED.
		&pVS,			// double pointer to ID3DBlob		
		&errorBlob		// pointer for Error Blob messages.
	);

	// compilation failed?
	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			// release "reference" to errorBlob interface object
			errorBlob->Release();
		}
		if (pVS)
			pVS->Release();
		return result;
	}

	gDevice->CreateVertexShader(
		pVS->GetBufferPointer(),
		pVS->GetBufferSize(),
		nullptr,
		&gBillboardVertexShader
	);

	// BillboardLayout
	D3D11_INPUT_ELEMENT_DESC billboardInputDesc[] = {
		{
			"POSITION",		// "semantic" name in shader
			0,				// "semantic" index (not used)
			DXGI_FORMAT_R32G32B32_FLOAT, // size of ONE element (3 floats)
			0,							 // input slot
			0,							 // offset of first element
			D3D11_INPUT_PER_VERTEX_DATA, // specify data PER vertex
			0							 // used for INSTANCING (ignore)
		},
		{
			"COLOUR",
			0,				// same slot as previous (same vertexBuffer)
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			12,							// offset of FIRST element (after POSITION)
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
	};

	result = gDevice->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pVS->GetBufferPointer(), pVS->GetBufferSize(), &gBillboardLayoutPosCol);
	if (FAILED(result))
		MessageBox(NULL, L"Error billboardVertexBuffer", L"Error", MB_OK | MB_ICONERROR);
	// we do not need anymore this COM object, so we release it.
	pVS->Release();

	////GeometryShader
	ID3DBlob* pGS = nullptr;
	if (errorBlob) errorBlob->Release();
	errorBlob = nullptr;

	result = D3DCompileFromFile(
		L"GeometryShader.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"GS_main",		// entry point
		"gs_5_0",		// shader model (target)
		D3DCOMPILE_DEBUG,	// shader compile options (DEBUGGING)
		0,				// IGNORE...DEPRECATED.
		&pGS,			// double pointer to ID3DBlob		
		&errorBlob		// pointer for Error Blob messages.
	);

	// compilation failed?
	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			// release "reference" to errorBlob interface object
			errorBlob->Release();
		}
		if (pGS)
			pGS->Release();
		return result;
	}

	gDevice->CreateGeometryShader(
		pGS->GetBufferPointer(),
		pGS->GetBufferSize(),
		nullptr,
		&gGeometryShader
	);

	// we do not need anymore this COM object, so we release it.
	pGS->Release();

	////GeometryShader for billboarding
	pGS = nullptr;
	if (errorBlob) errorBlob->Release();
	errorBlob = nullptr;

	result = D3DCompileFromFile(
		L"BillboardGeometryShader.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"GS_main",		// entry point
		"gs_5_0",		// shader model (target)
		D3DCOMPILE_DEBUG,	// shader compile options (DEBUGGING)
		0,				// IGNORE...DEPRECATED.
		&pGS,			// double pointer to ID3DBlob		
		&errorBlob		// pointer for Error Blob messages.
	);

	// compilation failed?
	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			// release "reference" to errorBlob interface object
			errorBlob->Release();
		}
		if (pGS)
			pGS->Release();
		return result;
	}

	gDevice->CreateGeometryShader(
		pGS->GetBufferPointer(),
		pGS->GetBufferSize(),
		nullptr,
		&gBillboardGeometryShader
	);

	// we do not need anymore this COM object, so we release it.
	pGS->Release();

	////create pixel shader
	ID3DBlob* pPS = nullptr;
	if (errorBlob) errorBlob->Release();
	errorBlob = nullptr;

	result = D3DCompileFromFile(
		L"Fragment.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"PS_main",		// entry point
		"ps_5_0",		// shader model (target)
		D3DCOMPILE_DEBUG,	// shader compile options
		0,				// effect compile options
		&pPS,			// double pointer to ID3DBlob		
		&errorBlob			// pointer for Error Blob messages.
	);

	// compilation failed?
	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			// release "reference" to errorBlob interface object
			errorBlob->Release();
		}
		if (pPS)
			pPS->Release();
		return result;
	}

	gDevice->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &gPixelShader);

	pPS->Release();

	////create pixel shader billboard
	pPS = nullptr;
	if (errorBlob) errorBlob->Release();
	errorBlob = nullptr;

	result = D3DCompileFromFile(
		L"BillboardPixelShader.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"PS_main",		// entry point
		"ps_5_0",		// shader model (target)
		D3DCOMPILE_DEBUG,	// shader compile options
		0,				// effect compile options
		&pPS,			// double pointer to ID3DBlob		
		&errorBlob			// pointer for Error Blob messages.
	);

	// compilation failed?
	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			// release "reference" to errorBlob interface object
			errorBlob->Release();
		}
		if (pPS)
			pPS->Release();
		return result;
	}

	HRESULT hr = gDevice->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &gPixelShaderBillboard);
	if(FAILED(hr))
		MessageBox(NULL, L"Error billboardVertexBuffer", L"Error", MB_OK | MB_ICONERROR);
	// we do not need anymore this COM object, so we release it.
	pPS->Release();

	//// Pixel shader for boundingVolumes
	//pPS = nullptr;
	//if (errorBlob) errorBlob->Release();
	//errorBlob = nullptr;

	//result = D3DCompileFromFile(
	//	L"BoundingVolumePixelShader.hlsl", // filename
	//	nullptr,		// optional macros
	//	nullptr,		// optional include files
	//	"PS_main",		// entry point
	//	"ps_5_0",		// shader model (target)
	//	D3DCOMPILE_DEBUG,	// shader compile options
	//	0,				// effect compile options
	//	&pPS,			// double pointer to ID3DBlob		
	//	&errorBlob			// pointer for Error Blob messages.
	//);

	//// compilation failed?
	//if (FAILED(result))
	//{
	//	if (errorBlob)
	//	{
	//		OutputDebugStringA((char*)errorBlob->GetBufferPointer());
	//		// release "reference" to errorBlob interface object
	//		errorBlob->Release();
	//	}
	//	if (pPS)
	//		pPS->Release();
	//	return result;
	//}

	//hr = gDevice->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &gPixelShaderBoundingVolume);
	//if (FAILED(hr))
	//	MessageBox(NULL, L"Error gPixelShaderBoundingVolume", L"Error", MB_OK | MB_ICONERROR);
	//// we do not need anymore this COM object, so we release it.
	//pPS->Release();


	return S_OK;
}

HRESULT createShadersSP()
{
	// Binary Large OBject (BLOB), for compiled shader, and errors.
	ID3DBlob* pVS = nullptr;
	ID3DBlob* errorBlob = nullptr;

	// Vertexshader //
	HRESULT result = D3DCompileFromFile(
		L"VertexSP.hlsl",	// filename
		nullptr,			// optional macros
		nullptr,			// optional include files
		"VS_main",			// entry point
		"vs_5_0",			// shader model (target)
		D3DCOMPILE_DEBUG,	// shader compile options (DEBUGGING)
		0,					// IGNORE...DEPRECATED.
		&pVS,				// double pointer to ID3DBlob		
		&errorBlob			// pointer for Error Blob messages.
	);

	// compilation failed?
	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			// release "reference" to errorBlob interface object
			errorBlob->Release();
		}
		if (pVS)
			pVS->Release();
		return result;
	}

	result = gDevice->CreateVertexShader(
		pVS->GetBufferPointer(),
		pVS->GetBufferSize(),
		nullptr,
		&gVertexShaderSP
	);
	if (FAILED(result))
		MessageBox(NULL, L"Error1", L"Error", MB_OK | MB_ICONERROR);

	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{
			"POSITION",						// "semantic" name in shader
			0,								// "semantic" index (not used)
			DXGI_FORMAT_R32G32_FLOAT,		// size of ONE element (3 floats)
			0,								// input slot
			0,								// offset of first element
			D3D11_INPUT_PER_VERTEX_DATA,	// specify data PER vertex
			0								// used for INSTANCING (ignore)
		},
		{
			"UV_COORD",
			0,								// same slot as previous (same vertexBuffer)
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			12,								// offset of FIRST element (after POSITION)
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
	};

	result = gDevice->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pVS->GetBufferPointer(), pVS->GetBufferSize(), &gVertexLayoutFSQuad);
	if (FAILED(result))
		MessageBox(NULL, L"Error1", L"Error", MB_OK | MB_ICONERROR);

	pVS->Release();

	// Pixelshader //
	ID3DBlob* pPS = nullptr;
	if (errorBlob) errorBlob->Release();
	errorBlob = nullptr;

	result = D3DCompileFromFile(
		L"FragmentSP.hlsl", // filename
		nullptr,			// optional macros
		nullptr,			// optional include files
		"PS_main",			// entry point
		"ps_5_0",			// shader model (target)
		D3DCOMPILE_DEBUG,	// shader compile options
		0,					// effect compile options
		&pPS,				// double pointer to ID3DBlob		
		&errorBlob			// pointer for Error Blob messages.
	);

	// compilation failed?
	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			// release "reference" to errorBlob interface object
			errorBlob->Release();
		}
		if (pPS)
			pPS->Release();
		return result;
	}

	result = gDevice->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &gPixelShaderSP);
	if (FAILED(result))
		MessageBox(NULL, L"Error1", L"Error", MB_OK | MB_ICONERROR);
	
	return S_OK;
}

void createTriangleData()
{
	/*std::string filePath = "Resources\\Meshes\\LP_Pillar_Textured.obj";
	bool flippedUV = true;
	mesh.setVertices(LoadOBJ(filePath, flippedUV, &gShaderResourceObjTexture));*/
	// Describe the Vertex Buffer
	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	// what type of buffer will this be?
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	// what type of usage (press F1, read the docs)
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	// how big in bytes each element in the buffer is.
	bufferDesc.ByteWidth = sizeof(TriangleVertex) * gPillar->getVertCount();

	// this struct is created just to set a pointer to the
	// data containing the vertices.
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &gPillar->getVertices()[0];

	// create a Vertex Buffer
	HRESULT result = gDevice->CreateBuffer(&bufferDesc, &data, &gVertexBuffer);
	if (FAILED(result))
		MessageBox(NULL, L"Error1", L"Error", MB_OK | MB_ICONERROR);

	//Fullscreen quad
	TriangleVertexUV fsQuad[6] =
	{
		-1.0f, 1.0f, 0.0f,	//v0 pos	L T
		0.0f, 0.0f,			//v0 tex

		1.0, -1.0f, 0.0f,	//v1 pos	R B
		1.0f, 1.0f,			//v1 tex

		-1.0f, -1.0f, 0.0f, //v2 pos	L B
		0.0f, 1.0f,			//v2 tex

		-1.0f, 1.0f, 0.0f,	//v3 pos	L T
		0.0f, 0.0f,			//v3 tex

		1.0f, 1.0f, 0.0f,	//v4 pos	R T
		1.0f, 0.0f,			//v4 tex

		1.0f, -1.0f, 0.0f,	//v5 pos	R B
		1.0f, 1.0f			//v5 tex
	};

	// Describe the Vertex Buffer
	D3D11_BUFFER_DESC bufferDescFSQuad;
	memset(&bufferDescFSQuad, 0, sizeof(bufferDescFSQuad));
	// what type of buffer will this be?
	bufferDescFSQuad.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	// what type of usage (press F1, read the docs)
	bufferDescFSQuad.Usage = D3D11_USAGE_DEFAULT;
	// how big in bytes each element in the buffer is.
	bufferDescFSQuad.ByteWidth = sizeof(fsQuad);

	// this struct is created just to set a pointer to the
	// data containing the vertices.
	D3D11_SUBRESOURCE_DATA dataFSQuad;
	dataFSQuad.pSysMem = fsQuad;

	// create a Vertex Buffer
	result = gDevice->CreateBuffer(&bufferDescFSQuad, &dataFSQuad, &gVertexBufferFSQuad);
	if (FAILED(result))
		MessageBox(NULL, L"Error1", L"Error", MB_OK | MB_ICONERROR);

	// Billboard
	billboardPoint billboardPointInfo
	{
		2.0f, 8.0f, -3.0f,
		1.0f, 1.0f, 1.0f
	};

	bufferDesc.ByteWidth = sizeof(TriangleVertex);
	D3D11_SUBRESOURCE_DATA data2;
	data2.pSysMem = &billboardPointInfo;
	HRESULT hr = gDevice->CreateBuffer(&bufferDesc, &data2, &gBillboardVertexBuffer);
	if (FAILED(hr))
		MessageBox(NULL, L"Error billboardVertexBuffer", L"Error", MB_OK | MB_ICONERROR);


}

void createConstantBuffer()
{
	//WorldViewProj
	// Fill in a buffer description.
	D3D11_BUFFER_DESC cbDesc;
	cbDesc.ByteWidth = sizeof(gMatricesPerFrame);
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	// Fill in the subresource data.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = &gMatricesPerFrame;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	// create a Constant Buffer
	HRESULT result = gDevice->CreateBuffer(&cbDesc, &InitData, &gConstantBuffer);
	if (FAILED(result))
		MessageBox(NULL, L"Error1", L"Error", MB_OK | MB_ICONERROR);

	//Light
	// Fill in a buffer description.
	cbDesc.ByteWidth = sizeof(gLight);
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	// Fill in the subresource data.
	InitData.pSysMem = &gLight;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	// create a Constant Buffer
	gDevice->CreateBuffer(&cbDesc, &InitData, &gConstantBufferLight);

	// CameraData
	// Fill in a buffer description.
	cbDesc.ByteWidth = sizeof(gCameraData);
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	// Fill in the subresource data.
	InitData.pSysMem = &gCameraData;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	// create a Constant Buffer
	gDevice->CreateBuffer(&cbDesc, &InitData, &gConstantBufferCamera);

	// BillboardData
	// Fill in a buffer description.
	cbDesc.ByteWidth = sizeof(gBillboardData);
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	// Fill in the subresource data.
	InitData.pSysMem = &gBillboardData;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	// create a Constant Buffer
	gDevice->CreateBuffer(&cbDesc, &InitData, &gConstantBufferBillboard);
}

void transform(XMFLOAT3 move, XMMATRIX rotation)
{
	XMVECTOR camForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR camRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR camUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMVECTOR LookAt = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	LookAt = XMVector3TransformCoord(camForward, rotation);
	LookAt = XMVector3Normalize(LookAt);

	camRight = XMVector3TransformCoord(camRight, rotation);
	camUp = XMVector3TransformCoord(camUp, rotation);
	camForward = XMVector3TransformCoord(camForward, rotation);

	gCamera.pos += move.x * camRight;
	gCamera.pos += move.y * camUp;
	gCamera.pos += move.z * camForward;
	LookAt = gCamera.pos + LookAt;

	XMMATRIX World = DirectX::XMMatrixRotationY(0.0f);
	XMMATRIX View = XMMatrixLookAtLH(gCamera.pos, LookAt, camUp);
	XMMATRIX Projection = XMMatrixPerspectiveFovLH(0.45f * DirectX::XM_PI, WIDTH / HEIGHT, 0.1, 20.0f);

	View = XMMatrixTranspose(View);
	Projection = XMMatrixTranspose(Projection);

	XMMATRIX WorldView = XMMatrixMultiply(View, World);
	XMMATRIX WorldViewProj = XMMatrixMultiply(Projection, WorldView);
	
	gMatricesPerFrame.WorldViewProj = WorldViewProj;
	gMatricesPerFrame.World = World;

	gCameraData.camPos = gCamera.pos;
}

void createDepthStencil()
{
	//DepthStencil
	ID3D11Texture2D* pDepthStencil = NULL;
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = WIDTH;
	descDepth.Height = HEIGHT;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	HRESULT hr = gDevice->CreateTexture2D(&descDepth, NULL, &pDepthStencil);

	D3D11_DEPTH_STENCIL_DESC dsDesc;

	// Depth test parameters
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

	// Create depth stencil state
	ID3D11DepthStencilState * pDSState;
	gDevice->CreateDepthStencilState(&dsDesc, &pDSState);

	// Bind depth stencil state
	gDeviceContext->OMSetDepthStencilState(pDSState, 1);

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;

	// Create the depth stencil view
	hr = gDevice->CreateDepthStencilView(pDepthStencil, &descDSV, &gDSV);
	if (FAILED(hr))
		MessageBox(NULL, L"Error1", L"Error", MB_OK | MB_ICONERROR);
}

void rockTexture()
{
	//Sampler
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.MaxAnisotropy = 0;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MipLODBias = 0;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	HRESULT hr = gDevice->CreateSamplerState(&sampDesc, &gSamplerState);
	if (FAILED(hr))
		MessageBox(NULL, L"Error", L"Error", MB_OK | MB_ICONERROR);
}

void textureSetUp()
{
	rockTexture();
}

float mousePicking(POINT cursorPos)
{
	float projX = XMVectorGetX(gCamera.projection.r[0]);
	float projY = XMVectorGetY(gCamera.projection.r[1]);

	float viewX = (2.0f * cursorPos.x / WIDTH - 1.0f) / projX;
	float viewY = (2.0f * cursorPos.y / HEIGHT - 1.0f) / projY;

	float currT = -1.0f;

	XMVECTOR rayOrigin = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR rayDirection = XMVectorSet(viewX, viewY, 0.0f, 0.0f);

	// view -> world
	XMMATRIX inverseView = XMMatrixInverse(nullptr, gCamera.view);
	rayDirection = XMVector3Transform(rayDirection, inverseView);
	rayOrigin = inverseView.r[4];
	
	currT = gPillar->getBoundingVolume()->intersectWithRay(rayDirection, rayOrigin);
	return currT;
}

void createRenderTargets()
{
	//TEXTURE FOR FIRST PASS RENDER
	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(texDesc));
	texDesc.Width = WIDTH;
	texDesc.Height = HEIGHT;
	texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = D3D11_USAGE_DEFAULT;
	texDesc.MiscFlags = 0;
	texDesc.SampleDesc.Count = 1;

	//Texture
	HRESULT hr = gDevice->CreateTexture2D(&texDesc, NULL, &gTexDeferredPos);
	if (FAILED(hr))
		MessageBox(NULL, L"Error1", L"Error", MB_OK | MB_ICONERROR);
	hr = gDevice->CreateTexture2D(&texDesc, NULL, &gTexDeferredNor);
	if (FAILED(hr))
		MessageBox(NULL, L"Error1", L"Error", MB_OK | MB_ICONERROR);
	hr = gDevice->CreateTexture2D(&texDesc, NULL, &gTexDeferredCol);
	if (FAILED(hr))
		MessageBox(NULL, L"Error1", L"Error", MB_OK | MB_ICONERROR);

	// Rendertarget FIRST PASS //
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	ZeroMemory(&renderTargetViewDesc, sizeof(renderTargetViewDesc));
	renderTargetViewDesc.Format = texDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	hr = gDevice->CreateRenderTargetView(gTexDeferredPos, &renderTargetViewDesc, &gRenderTargetsDeferred[0]);
	if (FAILED(hr))
		MessageBox(NULL, L"Error1", L"Error", MB_OK | MB_ICONERROR);
	hr = gDevice->CreateRenderTargetView(gTexDeferredNor, &renderTargetViewDesc, &gRenderTargetsDeferred[1]);
	if (FAILED(hr))
		MessageBox(NULL, L"Error1", L"Error", MB_OK | MB_ICONERROR);
	hr = gDevice->CreateRenderTargetView(gTexDeferredCol, &renderTargetViewDesc, &gRenderTargetsDeferred[2]);
	if (FAILED(hr))
		MessageBox(NULL, L"Error1", L"Error", MB_OK | MB_ICONERROR);

	
	// Shaderresourceview SECOND PASS //
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	ZeroMemory(&shaderResourceViewDesc, sizeof(shaderResourceViewDesc));
	shaderResourceViewDesc.Format = texDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = texDesc.MipLevels;

	// Create the shader resource view.
	hr = gDevice->CreateShaderResourceView(gTexDeferredPos, &shaderResourceViewDesc, &gShaderResourceDeferred[0]);
	if (FAILED(hr))
		MessageBox(NULL, L"Error1", L"Error", MB_OK | MB_ICONERROR);
	hr = gDevice->CreateShaderResourceView(gTexDeferredNor, &shaderResourceViewDesc, &gShaderResourceDeferred[1]);
	if (FAILED(hr))
		MessageBox(NULL, L"Error1", L"Error", MB_OK | MB_ICONERROR);
	hr = gDevice->CreateShaderResourceView(gTexDeferredCol, &shaderResourceViewDesc, &gShaderResourceDeferred[2]);
	if (FAILED(hr))
		MessageBox(NULL, L"Error1", L"Error", MB_OK | MB_ICONERROR);
}

void SetViewport()
{
	D3D11_VIEWPORT vp;
	vp.Width = (float)WIDTH;
	vp.Height = (float)HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	gDeviceContext->RSSetViewports(1, &vp);
}

void renderFirstPass()
{
	gDeviceContext->VSSetShader(gVertexShader, nullptr, 0);
	gDeviceContext->HSSetShader(nullptr, nullptr, 0);
	gDeviceContext->DSSetShader(nullptr, nullptr, 0);
	gDeviceContext->GSSetShader(gGeometryShader, nullptr, 0);
	gDeviceContext->PSSetShader(gPixelShader, nullptr, 0);
	gDeviceContext->PSSetShaderResources(0, 1, &gShaderResourceObjTexture);
	UINT32 vertexSize = sizeof(TriangleVertex);
	UINT32 offset = 0;

	gDeviceContext->IASetVertexBuffers(0, 1, &gVertexBuffer, &vertexSize, &offset);
	gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gDeviceContext->IASetInputLayout(gVertexLayout);
	gDeviceContext->PSSetSamplers(0, 1, &gSamplerState);

	gDeviceContext->GSSetConstantBuffers(0, 1, &gConstantBuffer);

	gDeviceContext->Draw(gPillar->getVertCount(), 0);

	// Bounding Volume
	//gDeviceContext->IASetVertexBuffers(0, 1, &gVertexBuffer, &vertexSize, &offset);
	//gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	//gDeviceContext->IASetInputLayout(gBillboardLayoutPosCol);
	//gDeviceContext->Draw(gPillar->getVertCount(), 0);
}

void renderBillboard()
{
	gDeviceContext->VSSetShader(gBillboardVertexShader, nullptr, 0);
	gDeviceContext->HSSetShader(nullptr, nullptr, 0);
	gDeviceContext->DSSetShader(nullptr, nullptr, 0);
	gDeviceContext->GSSetShader(gBillboardGeometryShader, nullptr, 0);
	gDeviceContext->PSSetShader(gPixelShaderBillboard, nullptr, 0);
	
	UINT32 vertexSize = sizeof(billboardPoint);
	UINT32 offset = 0;
	// specify which vertex buffer to use next.
	gDeviceContext->IASetVertexBuffers(0, 1, &gBillboardVertexBuffer, &vertexSize, &offset);

	// specify the topology to use when drawing
	gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	// specify the IA Layout (how is data passed)
	gDeviceContext->IASetInputLayout(gBillboardLayoutPosCol);

	//ConstantBuffer
	gDeviceContext->GSSetConstantBuffers(0, 1, &gConstantBuffer);
	gDeviceContext->GSSetConstantBuffers(1, 1, &gConstantBufferCamera);
	gDeviceContext->GSSetConstantBuffers(2, 1, &gConstantBufferBillboard);
	gDeviceContext->PSSetConstantBuffers(0, 1, &gConstantBufferLight);
	// issue a draw call of 3 vertices (similar to OpenGL)
	gDeviceContext->Draw(1, 0);
}

void renderSecondPass()
{
	gDeviceContext->VSSetShader(gVertexShaderSP, nullptr, 0);
	gDeviceContext->HSSetShader(nullptr, nullptr, 0);
	gDeviceContext->DSSetShader(nullptr, nullptr, 0);
	gDeviceContext->GSSetShader(nullptr, nullptr, 0);
	gDeviceContext->PSSetShader(gPixelShaderSP, nullptr, 0);

	UINT32 vertexSize = sizeof(TriangleVertexUV);
	UINT32 offset = 0;

	gDeviceContext->IASetVertexBuffers(0, 1, &gVertexBufferFSQuad, &vertexSize, &offset);
	gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gDeviceContext->IASetInputLayout(gVertexLayoutFSQuad);

	gDeviceContext->PSSetSamplers(0, 1, &gSamplerState);
	gDeviceContext->PSSetShaderResources(0, 3, gShaderResourceDeferred);

	gDeviceContext->PSSetConstantBuffers(0, 1, &gConstantBufferLight);
	gDeviceContext->PSSetConstantBuffers(1, 1, &gConstantBufferCamera);

	gDeviceContext->Draw(6, 0);

	ID3D11ShaderResourceView* nullRTV = { NULL };
	gDeviceContext->PSSetShaderResources(0, 1, &nullRTV);
}

void update(float lastT)
{
	gDeviceContext->GSSetShader(nullptr, nullptr, 0);

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
	ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
	ImGui::SliderFloat("float", &gFloat, 0.0f, 2 * 3.1415);            // Edit 1 float using a slider from 0.0f to 1.0f    
	ImGui::SliderFloat("dist", &gRotation, 0.0f, 10.0f);
	ImGui::ColorEdit3("clear color", (float*)&gClearColour); // Edit 3 floats representing a color
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Text("Your location: X:%.2f, Y:%.2f, Z:%.2f )", XMVectorGetX(gCamera.pos), XMVectorGetY(gCamera.pos), XMVectorGetZ(gCamera.pos));
	ImGui::Text("lastT:%.2f)", lastT);
	ImGui::End();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	
	D3D11_MAPPED_SUBRESOURCE mappedMemory;
	gDeviceContext->Map(gConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
	memcpy(mappedMemory.pData, &gMatricesPerFrame, sizeof(gMatricesPerFrame));
	gDeviceContext->Unmap(gConstantBuffer, 0);

	gDeviceContext->Map(gConstantBufferCamera, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
	memcpy(mappedMemory.pData, &gCameraData, sizeof(gCameraData));
	gDeviceContext->Unmap(gConstantBufferCamera, 0);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	MSG msg = { 0 };
	HWND wndHandle = InitWindow(hInstance); // Skapa fönster
	
	if (wndHandle)
	{
		CreateDirect3DContext(wndHandle); // Skapa och koppla SwapChain, Device och Device Context

		SetViewport();

		createMeshes(); // test

		createRenderTargets();
		createShaders();
		createShadersSP();

		createTriangleData(); //5. Definiera triangelvertiser, 6. Skapa vertex buffer, 7. Skapa input layout

		textureSetUp();
		createConstantBuffer();

		ShowWindow(wndHandle, nCmdShow);

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); // (void)io;
		ImGui_ImplWin32_Init(wndHandle);
		ImGui_ImplDX11_Init(gDevice, gDeviceContext);
		ImGui::StyleColorsDark();

		std::unique_ptr<Keyboard> keyboard;
		std::unique_ptr<Mouse> mouse;
		Mouse::ButtonStateTracker tracker;

		keyboard = std::make_unique<Keyboard>();
		mouse = std::make_unique<Mouse>();
		mouse->SetWindow(wndHandle);
		POINT cursorPos;
		XMFLOAT3 velocity{ 0.0f, 0.0f, 0.0f };
		float distance = 5.0f;
		float pitch = 0.0f;
		float yaw = 0.0f;
		
		float lastT = 0.0f;
		LARGE_INTEGER clockFreq;
		LARGE_INTEGER startTime;
		LARGE_INTEGER delta;
		LARGE_INTEGER currTime;
		QueryPerformanceFrequency(&clockFreq);
		QueryPerformanceCounter(&startTime);
	
		while (WM_QUIT != msg.message)
		{
			if (PeekMessage(&msg, wndHandle, 0, 0, PM_REMOVE))
			{
				switch (msg.message)
				{
				case WM_KEYDOWN:
					Keyboard::ProcessMessage(msg.message, msg.wParam, msg.lParam);
				case WM_KEYUP:
					Keyboard::ProcessMessage(msg.message, msg.wParam, msg.lParam);
				case WM_MOUSEMOVE:
					Mouse::ProcessMessage(msg.message, msg.wParam, msg.lParam);
				case WM_INPUT:
					Mouse::ProcessMessage(msg.message, msg.wParam, msg.lParam);
					break;
				}
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				//Camera updates
				//
				//

				QueryPerformanceCounter(&currTime);
				delta.QuadPart = currTime.QuadPart - startTime.QuadPart;
				float deltaSeconds = (float)delta.QuadPart / clockFreq.QuadPart;
				startTime = currTime;

				DirectX::Mouse::State ms = mouse->GetState();
				DirectX::Keyboard::State kb = keyboard->GetState();
				mouse->SetMode(Mouse::MODE_RELATIVE);

				yaw += XMConvertToRadians(ms.x);
				pitch += XMConvertToRadians(ms.y);
				pitch = min(XM_PI / 2, max(-XM_PI / 2, pitch));

				XMMATRIX rotation = XMMatrixRotationRollPitchYaw(pitch, yaw, 0.0f);

				GetCursorPos(&cursorPos); // gets current curosr coordinates
				ScreenToClient(wndHandle, &cursorPos); // sets cursor coordinates relative to the program window

				lastT = mousePicking(cursorPos);
				

				velocity.x = 0;
				velocity.y = 0;
				velocity.z = 0;

				if (kb.W)
					velocity.z += distance * deltaSeconds;
				if (kb.S)
					velocity.z -= distance * deltaSeconds;
				if (kb.A)
					velocity.x -= distance * deltaSeconds;
				if (kb.D)
					velocity.x += distance * deltaSeconds;
				if (kb.Space)
					velocity.y += distance * deltaSeconds;
				if (kb.LeftControl)
					velocity.y -= distance * deltaSeconds;
				if (kb.Home)
					velocity = { 0.0f, 0.0f, -2.0f };
				if (kb.Escape)
					msg.message = WM_QUIT;

				transform(velocity, rotation);
				
				//
				//
				// RENDER //
				gDeviceContext->OMSetRenderTargets(3, gRenderTargetsDeferred, gDSV);
				
				gClearColour[3] = 1.0;
				gDeviceContext->ClearDepthStencilView(gDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
				gDeviceContext->ClearRenderTargetView(gRenderTargetsDeferred[0], gClearColour);
				gDeviceContext->ClearRenderTargetView(gRenderTargetsDeferred[1], gClearColour);
				gDeviceContext->ClearRenderTargetView(gRenderTargetsDeferred[2], gClearColour);

				renderFirstPass();
				renderBillboard();

				gDeviceContext->OMSetRenderTargets(1, &gBackbufferRTV, nullptr);
				gDeviceContext->ClearRenderTargetView(gBackbufferRTV, gClearColour);
				
				renderSecondPass();
				
				update(lastT);
				
				gSwapChain->Present(0, 0);
				
				ImGui::Render();
				ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
			}
		}

		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();

		gVertexBuffer->Release();
		gVertexBufferFSQuad->Release();
		gConstantBuffer->Release();
		gShaderResourceDeferred[0]->Release();
		gShaderResourceDeferred[1]->Release();
		gShaderResourceDeferred[2]->Release();
		gRenderTargetsDeferred[0]->Release();
		gRenderTargetsDeferred[1]->Release();
		gRenderTargetsDeferred[2]->Release();
		gSamplerState->Release();

		gVertexLayout->Release();
		gVertexLayoutFSQuad->Release();
		
		gVertexShader->Release();
		gVertexShaderSP->Release();
		gGeometryShader->Release();
		gPixelShader->Release();
		gPixelShaderSP->Release();
		gPixelShaderBillboard->Release();
		gBillboardGeometryShader->Release();

		gDSV->Release();
		gBackbufferRTV->Release();
		gSwapChain->Release();
		gDevice->Release();
		gDeviceContext->Release();
		delete gPillar;
		DestroyWindow(wndHandle);
	}

	return (int)msg.wParam;
}

HWND InitWindow(HINSTANCE hInstance)
{
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;
	wcex.lpszClassName = L"BTH_D3D_DEMO";
	if (!RegisterClassEx(&wcex))
		return false;

	RECT rc = { 0, 0, (int)WIDTH, (int)HEIGHT };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	HWND handle = CreateWindow(
		L"BTH_D3D_DEMO",
		L"BTH Direct3D Demo",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rc.right - rc.left,
		rc.bottom - rc.top,
		nullptr,
		nullptr,
		hInstance,
		nullptr);

	return handle;
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// check if IMGUI can handle the message (when we click INSIDE ImGui
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;

	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

HRESULT CreateDirect3DContext(HWND wndHandle)
{
	// create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC scd;

	// clear out the struct for use
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	// fill the swap chain description struct
	scd.BufferCount = 1;                                    // one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
	scd.OutputWindow = wndHandle;                           // the window to be used
	scd.SampleDesc.Count = 1;                               // how many multisamples
	scd.Windowed = TRUE;                                    // windowed/full-screen mode

	// create a device, device context and swap chain using the information in the scd struct
	HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&gSwapChain,
		&gDevice,
		NULL,
		&gDeviceContext);

	if (SUCCEEDED(hr))
	{
		// get the address of the back buffer
		ID3D11Texture2D* pBackBuffer = nullptr;
		gSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

		// use the back buffer address to create the render target
		gDevice->CreateRenderTargetView(pBackBuffer, NULL, &gBackbufferRTV);
		pBackBuffer->Release();

		//DepthBuffer
		createDepthStencil();
	}
	return hr;
}