#pragma once
#include <windows.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

// Extra
#include <algorithm>
#include <wchar.h>
// Own classes
#include "Mesh.h"
#include "Vertex_Pos_UV_Normal.h"
#include "Heightmap.h"
#include "QuadtreeNode.h"

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

ID3D11RasterizerState* gRasterizerState = nullptr;

// VIEWS //
ID3D11DepthStencilView* gDSV = nullptr;

ID3D11RenderTargetView* gBackbufferRTV = nullptr;
ID3D11RenderTargetView* gRenderTargetsDeferred[3] = {};
ID3D11RenderTargetView* gRenderTargetShadowMap = nullptr;

ID3D11ShaderResourceView* gSRV_Texture = nullptr;
ID3D11ShaderResourceView *gShaderResourceDeferred[3] = {};
ID3D11ShaderResourceView* gShaderResourceShadowMap = nullptr;

// SAMPLERS //
ID3D11SamplerState *gSamplerState = nullptr;

// BUFFERS //
ID3D11Buffer* gVertexBufferFSQuad = nullptr;
ID3D11Buffer* gVertexBufferBillboard = nullptr;
ID3D11Buffer* gVertexBufferBoundingVolume = nullptr;

ID3D11Buffer* gConstantBuffer = nullptr;
ID3D11Buffer* gConstantBufferLight = nullptr;
ID3D11Buffer* gConstantBufferCamera = nullptr;
ID3D11Buffer* gConstantBufferBillboard = nullptr;
ID3D11Buffer* gConstantBufferShadowMap = nullptr;

ID3D11InputLayout* gVertexLayout = nullptr;
ID3D11InputLayout* gVertexLayoutFSQuad = nullptr;
ID3D11InputLayout* gVertexLayoutPosCol = nullptr;
ID3D11InputLayout* gVertexLayout_Pos_UV_Normal_Tan = nullptr;


//TEXTURES
ID3D11Texture2D *gTexDeferredPos = nullptr;
ID3D11Texture2D *gTexDeferredNor = nullptr;
ID3D11Texture2D *gTexDeferredCol = nullptr;

ID3D11Texture2D* gTexShadowMap = nullptr;

// SHADERS //
ID3D11VertexShader* gVertexShader = nullptr;
ID3D11VertexShader* gVertexShaderBoundingVolume = nullptr;
ID3D11VertexShader* gVertexShaderNormalMap = nullptr;
ID3D11VertexShader* gVertexShaderSP = nullptr;
ID3D11VertexShader* gVertexShaderBillboard = nullptr;
ID3D11VertexShader* gVertexShaderShadowMap = nullptr;
ID3D11GeometryShader* gGeometryShader = nullptr;
ID3D11GeometryShader* gGeometryShaderBillboard = nullptr;
ID3D11GeometryShader* gGeometryShaderNormalMap = nullptr;
ID3D11GeometryShader* gGeometryShaderShadowMap = nullptr;
ID3D11PixelShader* gPixelShader = nullptr;
ID3D11PixelShader* gPixelShaderSP = nullptr;
ID3D11PixelShader* gPixelShaderBoundingVolume = nullptr;
ID3D11PixelShader* gPixelShaderBillboard = nullptr;
ID3D11PixelShader* gPixelShaderNormalMap = nullptr;
ID3D11PixelShader* gPixelShaderShadowMap = nullptr;


// GLOBALS //
float gHeight = 0.0f;
bool gCameraWalkMode = true;
float gFloat = 1.0f;
float gDist = 0.0f;
float gRotation = 0.0f;
float gIncrement = 0;
float gClearColour[3] = {};
//Meshes
Mesh* gPillar = nullptr;
Mesh* gBrickWall = nullptr;
Mesh* gBillboard = nullptr;
Mesh* gBoundingVolume = nullptr;
Mesh* gPlane = nullptr;
Heightmap* gHeightmap = nullptr;
std::vector<Mesh*> gPillars;
// Quadtree //
QuadtreeNode* gRoot;
int gQuadtreeLevels = 4;
int gNrOfrenderedMeshes;

struct PerFrameMatrices {
	XMMATRIX World, WorldViewProj;
};
PerFrameMatrices gMatricesPerFrame;
ID3D11Buffer* gMatrixPerFrameBuffer = nullptr;

struct Lights
{
	XMFLOAT3 lightPos = { 0.0f, 10.0f, -50.0f };
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

struct ShadowMapData {
	XMMATRIX worldView;
};
ShadowMapData gLightView;

struct TriangleVertexUV
{
	float x, y, z;
	float u, v;
};

struct Camera
{
	XMVECTOR forward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR right = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR lookAt = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR pos = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX projection;

	float distance = 5.0f;
	XMFLOAT3 velocity{ 0.0f, 0.0f, 0.0f };
};
Camera gCamera;

void createMeshes()
{
	DirectX::XMMATRIX identityMatrix = DirectX::XMMatrixIdentity();
	gBrickWall = new Mesh("Resources\\OBJ files\\brick.obj", false, true, gDevice, ORIENTED_BOUNDING_BOX, identityMatrix);
	gPillar = new Mesh("Resources\\OBJ files\\LP_Pillar_Textured.obj", true, true, gDevice, ORIENTED_BOUNDING_BOX, identityMatrix);
	gPlane = new Mesh("Resources\\OBJ files\\plane.obj", false, false, gDevice, ORIENTED_BOUNDING_BOX, identityMatrix);

	DirectX::XMMATRIX modelMatrix = DirectX::XMMatrixTranslation(-5.0f, 0.0f, 0.0f);

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			modelMatrix = DirectX::XMMatrixTranslation(-35.0f + i * 10.0f, 0.0f, -35.0f + j * 10.0f);
			gPillars.push_back(new Mesh("Resources\\OBJ files\\LP_Pillar_Textured.obj", true, true, gDevice, ORIENTED_BOUNDING_BOX, modelMatrix));
		}
	}

	std::vector <Vertex_Pos_Col> arr;
	arr.push_back(Vertex_Pos_Col(XMFLOAT3(2.0f, 8.0f, -3.0f), XMFLOAT3(1.0f, 1.0f, 1.0f)));
	gBillboard = new Mesh(arr, gDevice);

	gHeightmap = new Heightmap("Resources\\Assets_Project\\heightmaps\\heightmap.pgm", 15.0f, 5000.0f, 15.0f, gDevice);

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

	gDevice->CreateVertexShader(pVS->GetBufferPointer(), pVS->GetBufferSize(), nullptr, &gVertexShader);

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
		L"VertexShaderBillboard.hlsl", // filename
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
	gDevice->CreateVertexShader(pVS->GetBufferPointer(), pVS->GetBufferSize(), nullptr, &gVertexShaderBillboard);

	// BillboardLayout
	D3D11_INPUT_ELEMENT_DESC inputDescPosCol[] = {
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

	result = gDevice->CreateInputLayout(inputDescPosCol, ARRAYSIZE(inputDescPosCol), pVS->GetBufferPointer(), pVS->GetBufferSize(), &gVertexLayoutPosCol);
	if (FAILED(result))
		MessageBox(NULL, L"Error", L"Error", MB_OK | MB_ICONERROR);
	// we do not need anymore this COM object, so we release it.
	pVS->Release();
	pVS = nullptr;

	//// Vertex shader bounding volume
	result = D3DCompileFromFile(L"VertexShaderBoundingVolume.hlsl", nullptr, nullptr, "VS_main", "vs_5_0", D3DCOMPILE_DEBUG, 0, &pVS, &errorBlob);
	// compilation failed?
	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}
		if (pVS)
			pVS->Release();
		return result;
	}

	gDevice->CreateVertexShader(pVS->GetBufferPointer(), pVS->GetBufferSize(), nullptr, &gVertexShaderBoundingVolume);
	pVS->Release();
	pVS = nullptr;

	//
	//// Vertex shader normal map
	if (errorBlob) errorBlob->Release();
	errorBlob = nullptr;
	result = D3DCompileFromFile(L"VertexShaderNormalMap.hlsl", nullptr, nullptr, "VS_main", "vs_5_0", D3DCOMPILE_DEBUG, 0, &pVS, &errorBlob);
	// compilation failed?
	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}
		if (pVS)
			pVS->Release();
		return result;
	}

	result = gDevice->CreateVertexShader(pVS->GetBufferPointer(), pVS->GetBufferSize(), nullptr, &gVertexShaderNormalMap);
	if (FAILED(result))
		MessageBox(NULL, L"Error VertexShadowMap", L"Error", MB_OK | MB_ICONERROR);

	D3D11_INPUT_ELEMENT_DESC inputDesc_Pos_UV_Col_Tan[] =
	{
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
			0,
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			12,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		{
			"NORMAL",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			20,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		{
			"TANGENT",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			32,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
	};
	result = gDevice->CreateInputLayout(inputDesc_Pos_UV_Col_Tan, ARRAYSIZE(inputDesc_Pos_UV_Col_Tan), pVS->GetBufferPointer(), pVS->GetBufferSize(), &gVertexLayout_Pos_UV_Normal_Tan);
	if (FAILED(result))
		MessageBox(NULL, L"ErrorVertexLayoutShadowMap", L"Error", MB_OK | MB_ICONERROR);

	pVS->Release();
	pVS = nullptr;

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

	result = gDevice->CreateGeometryShader(pGS->GetBufferPointer(), pGS->GetBufferSize(), nullptr, &gGeometryShader);
	if (FAILED(result))
		MessageBox(NULL, L"Error", L"Error", MB_OK | MB_ICONERROR);
	// we do not need anymore this COM object, so we release it.
	pGS->Release();
	pGS = nullptr;

	////GeometryShader for billboarding
	if (errorBlob) errorBlob->Release();
	errorBlob = nullptr;

	result = D3DCompileFromFile(
		L"GeometryShaderBillboard.hlsl", // filename
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
	gDevice->CreateGeometryShader(pGS->GetBufferPointer(), pGS->GetBufferSize(), nullptr, &gGeometryShaderBillboard);
	pGS->Release();
	pGS = nullptr;

	////GeometryShader for normal mapping
	if (errorBlob) errorBlob->Release();
	errorBlob = nullptr;

	result = D3DCompileFromFile(
		L"GeometryShaderNormalMap.hlsl", // filename
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
	gDevice->CreateGeometryShader(pGS->GetBufferPointer(), pGS->GetBufferSize(), nullptr, &gGeometryShaderNormalMap);
	pGS->Release();

	////create pixel shader
	ID3DBlob* pPS = nullptr;
	if (errorBlob) errorBlob->Release();
	errorBlob = nullptr;

	result = D3DCompileFromFile(
		L"PixelShader.hlsl", // filename
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
	pPS = nullptr;

	////create pixel shader billboard
	if (errorBlob) errorBlob->Release();
	errorBlob = nullptr;

	result = D3DCompileFromFile(
		L"PixelShaderBillboard.hlsl", // filename
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
	result = gDevice->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &gPixelShaderBillboard);
	if (FAILED(result))
		MessageBox(NULL, L"Error PixelShaderBillboard", L"Error", MB_OK | MB_ICONERROR);
	// we do not need anymore this COM object, so we release it.
	pPS->Release();
	pPS = nullptr;

	////create pixel shader for normal mapping
	if (errorBlob) errorBlob->Release();
	errorBlob = nullptr;

	result = D3DCompileFromFile(
		L"PixelShaderNormalMap.hlsl", // filename
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

	result = gDevice->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &gPixelShaderNormalMap);
	if (FAILED(result))
		MessageBox(NULL, L"Error PixelShaderNormalMap", L"Error", MB_OK | MB_ICONERROR);
	// we do not need anymore this COM object, so we release it.
	pPS->Release();
	pPS = nullptr;

	//// pixel shader bounding volume
	pPS = nullptr;
	if (errorBlob) errorBlob->Release();
	errorBlob = nullptr;

	result = D3DCompileFromFile(
		L"PixelShaderBoundingVolume.hlsl", // filename
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

	result = gDevice->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &gPixelShaderBoundingVolume);
	if (FAILED(result))
		MessageBox(NULL, L"Error gPixelShaderBoundingVolume", L"Error", MB_OK | MB_ICONERROR);
	// we do not need anymore this COM object, so we release it.
	pPS->Release();

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
		L"PixelShaderSP.hlsl", // filename
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

HRESULT createShadersShadowMap()
{
	// Binary Large OBject (BLOB), for compiled shader, and errors.
	ID3DBlob* pVS = nullptr;
	ID3DBlob* errorBlob = nullptr;

	// Vertexshader //
	HRESULT result = D3DCompileFromFile(
		L"VertexShaderShadowMap.hlsl",	// filename
		nullptr,						// optional macros
		nullptr,						// optional include files
		"VS_main",						// entry point
		"vs_5_0",						// shader model (target)
		D3DCOMPILE_DEBUG,				// shader compile options (DEBUGGING)
		0,								// IGNORE...DEPRECATED.
		&pVS,							// double pointer to ID3DBlob		
		&errorBlob						// pointer for Error Blob messages.
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

	result = gDevice->CreateVertexShader(pVS->GetBufferPointer(), pVS->GetBufferSize(), nullptr, &gVertexShaderShadowMap);
	if (FAILED(result))
		MessageBox(NULL, L"gVertexShaderShadowMap", L"Error", MB_OK | MB_ICONERROR);

	pVS->Release();

	//////GeometryShader
	//ID3DBlob* pGS = nullptr;
	//if (errorBlob) errorBlob->Release();
	//errorBlob = nullptr;

	//result = D3DCompileFromFile(
	//	L"GeometryShaderShadowMap.hlsl", // filename
	//	nullptr,		// optional macros
	//	nullptr,		// optional include files
	//	"GS_main",		// entry point
	//	"gs_5_0",		// shader model (target)
	//	D3DCOMPILE_DEBUG,	// shader compile options (DEBUGGING)
	//	0,				// IGNORE...DEPRECATED.
	//	&pGS,			// double pointer to ID3DBlob		
	//	&errorBlob		// pointer for Error Blob messages.
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
	//	if (pGS)
	//		pGS->Release();
	//	return result;
	//}

	//result = gDevice->CreateGeometryShader(pGS->GetBufferPointer(), pGS->GetBufferSize(), nullptr, &gGeometryShaderShadowMap);
	//if (FAILED(result))
	//	MessageBox(NULL, L"gGeometryShaderShadowMap", L"Error", MB_OK | MB_ICONERROR);

	//// we do not need anymore this COM object, so we release it.
	//pGS->Release();


	// Pixelshader //
	ID3DBlob* pPS = nullptr;
	if (errorBlob) errorBlob->Release();
	errorBlob = nullptr;

	result = D3DCompileFromFile(
		L"PixelShaderShadowMap.hlsl",	// filename
		nullptr,						// optional macros
		nullptr,						// optional include files
		"PS_main",						// entry point
		"ps_5_0",						// shader model (target)
		D3DCOMPILE_DEBUG,				// shader compile options
		0,								// effect compile options
		&pPS,							// double pointer to ID3DBlob		
		&errorBlob						// pointer for Error Blob messages.
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

	result = gDevice->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &gPixelShaderShadowMap);
	if (FAILED(result))
		MessageBox(NULL, L"gPixelShaderShadowMap", L"Error", MB_OK | MB_ICONERROR);

	return S_OK;
}

void createTriangleData()
{
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
	HRESULT result = gDevice->CreateBuffer(&bufferDescFSQuad, &dataFSQuad, &gVertexBufferFSQuad);
	if (FAILED(result))
		MessageBox(NULL, L"gVertexBufferFSQuad", L"Error", MB_OK | MB_ICONERROR);
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
	result = gDevice->CreateBuffer(&cbDesc, &InitData, &gConstantBufferLight);
	if (FAILED(result))
		MessageBox(NULL, L"Error1", L"Error", MB_OK | MB_ICONERROR);
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
	result = gDevice->CreateBuffer(&cbDesc, &InitData, &gConstantBufferCamera);
	if (FAILED(result))
		MessageBox(NULL, L"Error1", L"Error", MB_OK | MB_ICONERROR);

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
	result = gDevice->CreateBuffer(&cbDesc, &InitData, &gConstantBufferBillboard);
	if (FAILED(result))
		MessageBox(NULL, L"Error1", L"Error", MB_OK | MB_ICONERROR);

	// Fill in the subresource data.
	InitData.pSysMem = &gLightView;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	//Shadow map
	cbDesc.ByteWidth = sizeof(gLightView);
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	// create a Constant Buffer
	result = gDevice->CreateBuffer(&cbDesc, &InitData, &gConstantBufferShadowMap);
	if (FAILED(result))
		MessageBox(NULL, L"gConstantBufferShadowMap", L"Error", MB_OK | MB_ICONERROR);
}

void setupTextures()
{
	//ShadowMap
	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(texDesc));
	texDesc.Width = WIDTH;
	texDesc.Height = HEIGHT;
	texDesc.Format = DXGI_FORMAT_R32_FLOAT;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = D3D11_USAGE_DEFAULT;
	texDesc.MiscFlags = 0;
	texDesc.SampleDesc.Count = 1;

	HRESULT hr = gDevice->CreateTexture2D(&texDesc, NULL, &gTexShadowMap);
	if (FAILED(hr))
		MessageBox(NULL, L"TexShadowMap", L"Error", MB_OK | MB_ICONERROR);
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
	if (FAILED(hr))
		MessageBox(NULL, L"Error1", L"Error", MB_OK | MB_ICONERROR);

	D3D11_DEPTH_STENCIL_DESC dsDesc;

	// Depth test parameters
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

	// Create depth stencil state
	ID3D11DepthStencilState * pDSState;
	hr = gDevice->CreateDepthStencilState(&dsDesc, &pDSState);
	if (FAILED(hr))
		MessageBox(NULL, L"Error1", L"Error", MB_OK | MB_ICONERROR);

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

void samplerSetUp()
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

float mousePicking(POINT cursorPos)
{
	float currT = -1.0f;
	float projX = XMVectorGetX(gCamera.projection.r[0]);
	float projY = XMVectorGetY(gCamera.projection.r[1]);

	// ABBREVIATIONS ////
	// VS = view space //
	// WS = World space//
	// MS = Model space//
	/////////////////////

	// screen -> view
	float viewX = (2.0f * (float)cursorPos.x / WIDTH - 1.0f) / projX;
	float viewY = (-2.0f * (float)cursorPos.y / HEIGHT + 1.0f) / projY;

	XMVECTOR rayOriginVS = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0);
	XMVECTOR rayDirVS = XMVectorSet(viewX, viewY, 1.0f, 0.0f);

	// view -> world
	XMMATRIX inverseView = XMMatrixInverse(nullptr, gCamera.view);
	XMVECTOR rayOriginWS = inverseView.r[3];
	XMVECTOR rayDirWS = XMVector4Transform(rayDirVS, inverseView);
	rayDirWS = XMVector4Normalize(rayDirWS);

	// world -> model
	std::vector<Mesh*> intersectedMeshes = gRoot->getIntersectedMeshes(gCamera.pos, gCamera.lookAt, gCamera.up, gCamera.view, gCamera.projection, 0.1f, 200.0f, 0.45f * DirectX::XM_PI, HEIGHT / WIDTH);
	gNrOfrenderedMeshes = intersectedMeshes.size();
	for (int i = 0; i < gNrOfrenderedMeshes; i++)
	{
		XMMATRIX inverseWorld = XMMatrixInverse(nullptr, intersectedMeshes[i]->getModelMatrix());
		XMVECTOR rayOriginMS = XMVector4Transform(rayOriginWS, inverseWorld);
		XMVECTOR rayDirMS = XMVector4Transform(rayDirWS, inverseWorld);
		rayDirMS = XMVector4Normalize(rayDirWS);

		currT = intersectedMeshes[i]->getBoundingVolume()->intersectWithRay(rayDirMS, rayOriginMS);
	}

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

	//Shadow map
	D3D11_TEXTURE2D_DESC texDescSM;
	ZeroMemory(&texDescSM, sizeof(texDescSM));
	texDescSM.Width = WIDTH;
	texDescSM.Height = HEIGHT;
	texDescSM.Format = DXGI_FORMAT_R32_FLOAT;
	texDescSM.MipLevels = 1;
	texDescSM.ArraySize = 1;
	texDescSM.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDescSM.CPUAccessFlags = D3D11_USAGE_DEFAULT;
	texDescSM.MiscFlags = 0;
	texDescSM.SampleDesc.Count = 1;

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDescSM;
	ZeroMemory(&renderTargetViewDescSM, sizeof(renderTargetViewDescSM));
	renderTargetViewDescSM.Format = texDescSM.Format;
	renderTargetViewDescSM.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDescSM.Texture2D.MipSlice = 0;

	hr = gDevice->CreateRenderTargetView(gTexShadowMap, &renderTargetViewDescSM, &gRenderTargetShadowMap);
	if (FAILED(hr))
		MessageBox(NULL, L"gRenderTargetShadowMap", L"Error", MB_OK | MB_ICONERROR);

	//Shadow map
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDescSM;
	ZeroMemory(&shaderResourceViewDescSM, sizeof(shaderResourceViewDescSM));
	shaderResourceViewDescSM.Format = texDescSM.Format;
	shaderResourceViewDescSM.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDescSM.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDescSM.Texture2D.MipLevels = texDescSM.MipLevels;

	hr = gDevice->CreateShaderResourceView(gTexShadowMap, &shaderResourceViewDescSM, &gShaderResourceShadowMap);
	if (FAILED(hr))
		MessageBox(NULL, L"gShaderResourceShadowMap", L"Error", MB_OK | MB_ICONERROR);
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

	//Rasterizer state setup
	D3D11_RASTERIZER_DESC rasterizerDesc;
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	rasterizerDesc.FrontCounterClockwise = true;
	rasterizerDesc.DepthBias = false;
	rasterizerDesc.DepthBiasClamp = 0;
	rasterizerDesc.SlopeScaledDepthBias = 0;
	rasterizerDesc.DepthClipEnable = true;
	rasterizerDesc.ScissorEnable = false;
	rasterizerDesc.MultisampleEnable = false;
	rasterizerDesc.AntialiasedLineEnable = false;
	
	HRESULT hr = gDevice->CreateRasterizerState(&rasterizerDesc, &gRasterizerState);
	if (FAILED(hr))
		MessageBox(NULL, L"Error1", L"Error", MB_OK | MB_ICONERROR);

}

void transform(XMMATRIX rotation, XMMATRIX rotationYPos)
{
	gCamera.forward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	gCamera.right = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	gCamera.up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	gCamera.lookAt = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	gCamera.lookAt = XMVector3TransformCoord(gCamera.forward, rotation);
	gCamera.lookAt = XMVector3Normalize(gCamera.lookAt);

	if(gHeightmap != nullptr)
	{
		gHeight = gHeightmap->getHeight(XMVectorGetX(gCamera.pos), XMVectorGetZ(gCamera.pos));
	}

	if (gCameraWalkMode)
	{
		gCamera.right = XMVector3TransformCoord(gCamera.right, rotationYPos);
		gCamera.up = XMVector3TransformCoord(gCamera.up, rotationYPos);
		gCamera.forward = XMVector3TransformCoord(gCamera.forward, rotationYPos);
		gCamera.pos += gCamera.velocity.x * gCamera.right;
		gCamera.pos = XMVectorSetY(gCamera.pos, gHeight + 1.0f);
		gCamera.pos += gCamera.velocity.z * gCamera.forward;
		gCamera.lookAt += gCamera.pos;
		gCamera.distance = 5.0f;
	}
	else
	{
		gCamera.right = XMVector3TransformCoord(gCamera.right, rotation);
		gCamera.up = XMVector3TransformCoord(gCamera.up, rotation);
		gCamera.forward = XMVector3TransformCoord(gCamera.forward, rotation);
		gCamera.pos += gCamera.velocity.x * gCamera.right;
		gCamera.pos += gCamera.velocity.y * gCamera.up;
		gCamera.pos += gCamera.velocity.z * gCamera.forward;
		gCamera.lookAt += gCamera.pos;
		gCamera.distance = 10.0f;
	}

	XMMATRIX World = DirectX::XMMatrixRotationY(0.0f);
	XMMATRIX View = XMMatrixLookAtLH(gCamera.pos, gCamera.lookAt, gCamera.up);
	XMMATRIX Projection = XMMatrixPerspectiveFovLH(0.45f * DirectX::XM_PI, WIDTH / HEIGHT, 0.1, 200.0f);

	gCamera.world = World;
	gCamera.view = View;
	gCamera.projection = Projection;

	View = XMMatrixTranspose(View);
	Projection = XMMatrixTranspose(Projection);

	XMMATRIX WorldView = XMMatrixMultiply(View, World);
	XMMATRIX WorldViewProj = XMMatrixMultiply(Projection, WorldView);

	gMatricesPerFrame.WorldViewProj = WorldViewProj;
	gMatricesPerFrame.World = World;

	gCameraData.camPos = gCamera.pos;
}

void update(float lastT, POINT cursorPos)
{
	//Update LightView
	XMVECTOR camUp = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f); //Dåligt att göra detta i update
	XMVECTOR LookAt = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 lightPos = XMFLOAT4(gLight.lightPos.x, gLight.lightPos.y, gLight.lightPos.z, 1.0f);
	XMMATRIX View = XMMatrixLookAtLH(DirectX::XMLoadFloat4(&lightPos), LookAt, camUp);
	//XMMATRIX Projection = XMMatrixPerspectiveFovLH(0.45f * DirectX::XM_PI, WIDTH / HEIGHT, 0.1, 200.0f);
	XMMATRIX Projection = XMMatrixOrthographicLH(200, 200, 0.1, 200.0f);
	View = XMMatrixTranspose(View);
	Projection = XMMatrixTranspose(Projection);
	XMMATRIX worldView = XMMatrixMultiply(Projection, XMMatrixMultiply(View, gMatricesPerFrame.World));

	gLightView.worldView = worldView;

	gDeviceContext->GSSetShader(nullptr, nullptr, 0);

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
	ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
	ImGui::SliderFloat("float", &gFloat, 0.0f, 2 * 3.1415);            // Edit 1 float using a slider from 0.0f to 1.0f    
	ImGui::SliderFloat("dist", &gRotation, 0.0f, 10.0f);
	ImGui::SliderFloat("lightPosY", &gLight.lightPos.y, -20.0f, 100.0f);
	ImGui::ColorEdit3("clear color", (float*)&gClearColour); // Edit 3 floats representing a color
	ImGui::Checkbox("Walk Mode", &gCameraWalkMode);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Text("Your location: X:%.2f, Y:%.2f, Z:%.2f )", XMVectorGetX(gCamera.pos), XMVectorGetY(gCamera.pos), XMVectorGetZ(gCamera.pos));
	ImGui::Text("Cursor location: X:%.2ld, Y:%.2ld )", cursorPos.x, cursorPos.y);
	ImGui::Text("Nr of rendered meshes: %d", gNrOfrenderedMeshes);
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

	gDeviceContext->Map(gConstantBufferLight, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
	memcpy(mappedMemory.pData, &gLight, sizeof(gLight));
	gDeviceContext->Unmap(gConstantBufferLight, 0);

	gDeviceContext->Map(*gPillar->getBoundingVolume()->getVertexBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
	memcpy(mappedMemory.pData, gPillar->getBoundingVolume()->getVertices().data(), sizeof(Vertex_Pos_Col) * gPillar->getBoundingVolume()->getVertCount());
	gDeviceContext->Unmap(*gPillar->getBoundingVolume()->getVertexBuffer(), 0);

	// bounding volumes pillars
	std::vector<Mesh*> intersectedMeshes = gRoot->getIntersectedMeshes(gCamera.pos, gCamera.lookAt, gCamera.up, gCamera.view, gCamera.projection, 0.1f, 200.0f, 0.45f * DirectX::XM_PI, HEIGHT / WIDTH);
	gNrOfrenderedMeshes = intersectedMeshes.size();
	for (int i = 0; i < intersectedMeshes.size(); i++)
	{
		gDeviceContext->Map(*intersectedMeshes[i]->getBoundingVolume()->getVertexBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
		memcpy(mappedMemory.pData, intersectedMeshes[i]->getBoundingVolume()->getVertices().data(), sizeof(Vertex_Pos_Col) * intersectedMeshes[i]->getBoundingVolume()->getVertCount());
		gDeviceContext->Unmap(*intersectedMeshes[i]->getBoundingVolume()->getVertexBuffer(), 0);
	}



	gDeviceContext->Map(gConstantBufferShadowMap, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory); //Shadow map
	memcpy(mappedMemory.pData, &gLightView, sizeof(gLightView));
	gDeviceContext->Unmap(gConstantBufferShadowMap, 0);
}

void renderShadowMap()
{
	gDeviceContext->OMSetRenderTargets(1, &gRenderTargetShadowMap, gDSV); //Set render target

	gDeviceContext->ClearDepthStencilView(gDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0); //Clear
	gDeviceContext->ClearRenderTargetView(gRenderTargetShadowMap, gClearColour);

	gDeviceContext->VSSetShader(gVertexShaderShadowMap, nullptr, 0);
	gDeviceContext->HSSetShader(nullptr, nullptr, 0);
	gDeviceContext->DSSetShader(nullptr, nullptr, 0);
	gDeviceContext->GSSetShader(nullptr, nullptr, 0);
	gDeviceContext->PSSetShader(gPixelShaderShadowMap, nullptr, 0);
	gDeviceContext->VSSetConstantBuffers(0, 1, &gConstantBufferShadowMap);
	gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	UINT32 vertexSize = sizeof(Vertex_Pos_UV_Normal);
	UINT32 offset = 0;
	gDeviceContext->IASetInputLayout(gVertexLayout);

	//Plane
	gDeviceContext->IASetVertexBuffers(0, 1, gPlane->getVertexBuffer(), &vertexSize, &offset);
	gDeviceContext->Draw(gPlane->getVertCount(), 0);

	// PILLAR
	//gDeviceContext->IASetVertexBuffers(0, 1, gPillar->getVertexBuffer(), &vertexSize, &offset);
	//gDeviceContext->Draw(gPillar->getVertCount(), 0);

	// Heightmap
	gDeviceContext->IASetVertexBuffers(0, 1, gHeightmap->getVertexBuffer(), &vertexSize, &offset);
	gDeviceContext->Draw(gHeightmap->getVertCount(), 0);

	std::vector<Mesh*> intersectedMeshes = gRoot->getIntersectedMeshes(gCamera.pos, gCamera.lookAt, gCamera.up, gCamera.view, gCamera.projection, 0.1f, 200.0f, 0.45f * DirectX::XM_PI, HEIGHT / WIDTH);
	gNrOfrenderedMeshes = intersectedMeshes.size();
	for (int i = 0; i < intersectedMeshes.size(); i++)
	{
		gDeviceContext->IASetVertexBuffers(0, 1, intersectedMeshes[i]->getVertexBuffer(), &vertexSize, &offset);
		gDeviceContext->Draw(intersectedMeshes[i]->getVertCount(), 0);
	}
}

void renderFirstPass()
{
	gDeviceContext->VSSetShader(gVertexShader, nullptr, 0);
	gDeviceContext->HSSetShader(nullptr, nullptr, 0);
	gDeviceContext->DSSetShader(nullptr, nullptr, 0);
	gDeviceContext->GSSetShader(gGeometryShader, nullptr, 0);
	gDeviceContext->PSSetShader(gPixelShader, nullptr, 0);

	gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gDeviceContext->IASetInputLayout(gVertexLayout);
	gDeviceContext->PSSetSamplers(0, 1, &gSamplerState);

	gDeviceContext->GSSetConstantBuffers(0, 1, &gConstantBuffer);

	UINT32 vertexSize = sizeof(Vertex_Pos_UV_Normal);
	UINT32 offset = 0;

	//Plane
	gDeviceContext->PSSetShaderResources(0, 1, gPlane->getSRV_Texture());
	gDeviceContext->IASetVertexBuffers(0, 1, gPlane->getVertexBuffer(), &vertexSize, &offset);
	gDeviceContext->Draw(gPlane->getVertCount(), 0);

	//// PILLAR
	//gDeviceContext->PSSetShaderResources(0, 1, gPillar->getSRV_Texture());
	//gDeviceContext->IASetVertexBuffers(0, 1, gPillar->getVertexBuffer(), &vertexSize, &offset);
	//gDeviceContext->Draw(gPillar->getVertCount(), 0);

	// HEIGHTMAP
	gDeviceContext->PSSetShaderResources(0, 1, gBrickWall->getSRV_Texture());
	gDeviceContext->IASetVertexBuffers(0, 1, gHeightmap->getVertexBuffer(), &vertexSize, &offset);
	gDeviceContext->Draw(gHeightmap->getVertCount(), 0);
}

void renderNormalMap()
{
	UINT32 vertexSize = sizeof(Vertex_Pos_UV_Normal_Tangent);
	UINT32 offset = 0;

	gDeviceContext->VSSetShader(gVertexShaderNormalMap, nullptr, 0);
	gDeviceContext->HSSetShader(nullptr, nullptr, 0);
	gDeviceContext->DSSetShader(nullptr, nullptr, 0);
	gDeviceContext->GSSetShader(nullptr, nullptr, 0);
	gDeviceContext->PSSetShader(gPixelShaderNormalMap, nullptr, 0);

	gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gDeviceContext->IASetInputLayout(gVertexLayout_Pos_UV_Normal_Tan);
	gDeviceContext->PSSetSamplers(0, 1, &gSamplerState);

	gDeviceContext->VSSetConstantBuffers(0, 1, &gConstantBuffer);

	// BRICK WALL
	gDeviceContext->PSSetShaderResources(0, 1, gBrickWall->getSRV_Texture());
	gDeviceContext->PSSetShaderResources(1, 1, gBrickWall->getSRV_Normal());
	gDeviceContext->IASetVertexBuffers(0, 1, gBrickWall->getVertexBufferNormalMap(), &vertexSize, &offset);
	gDeviceContext->Draw(gBrickWall->getVertCount(), 0);

	// PILLAR
	gDeviceContext->PSSetShaderResources(0, 1, gPillar->getSRV_Texture());
	gDeviceContext->PSSetShaderResources(1, 1, gPillar->getSRV_Normal());
	gDeviceContext->IASetVertexBuffers(0, 1, gPillar->getVertexBufferNormalMap(), &vertexSize, &offset);
	gDeviceContext->Draw(gPillar->getVertCount(), 0);
	
	// PILLARS
	std::vector<Mesh*> intersectedMeshes = gRoot->getIntersectedMeshes(gCamera.pos, gCamera.lookAt, gCamera.up, gCamera.view, gCamera.projection, 0.1f, 200.0f, 0.45f * DirectX::XM_PI, HEIGHT / WIDTH);
	gNrOfrenderedMeshes = intersectedMeshes.size();
	for (int i = 0; i < intersectedMeshes.size(); i++)
	{
		gDeviceContext->PSSetShaderResources(0, 1, intersectedMeshes[i]->getSRV_Texture());
		gDeviceContext->PSSetShaderResources(1, 1, intersectedMeshes[i]->getSRV_Normal());
		gDeviceContext->IASetVertexBuffers(0, 1, intersectedMeshes[i]->getVertexBufferNormalMap(), &vertexSize, &offset);
		gDeviceContext->Draw(intersectedMeshes[i]->getVertCount(), 0);
	}
}

void renderBoundingVolume()
{
	gDeviceContext->VSSetShader(gVertexShaderBoundingVolume, nullptr, 0);
	gDeviceContext->HSSetShader(nullptr, nullptr, 0);
	gDeviceContext->DSSetShader(nullptr, nullptr, 0);
	gDeviceContext->GSSetShader(nullptr, nullptr, 0);
	gDeviceContext->PSSetShader(gPixelShaderBoundingVolume, nullptr, 0);

	UINT32 vertexSize = sizeof(Vertex_Pos_Col);
	UINT32 offset = 0;
	

	// specify the topology to use when drawing
	gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	// specify the IA Layout (how is data passed)
	gDeviceContext->IASetInputLayout(gVertexLayoutPosCol);

	//ConstantBuffer
	gDeviceContext->VSSetConstantBuffers(0, 1, &gConstantBuffer);

	for (int i = 0; i < gPillars.size(); i++)
	{
		gDeviceContext->IASetVertexBuffers(0, 1, gPillars[i]->getBoundingVolume()->getVertexBuffer(), &vertexSize, &offset);
		gDeviceContext->Draw(gPillars[i]->getBoundingVolume()->getVertCount(), 0);
	}

	gDeviceContext->IASetVertexBuffers(0, 1, gPillar->getBoundingVolume()->getVertexBuffer(), &vertexSize, &offset);
	gDeviceContext->Draw(gPillar->getBoundingVolume()->getVertCount(), 0);
}

void renderBillboard()
{
	gDeviceContext->VSSetShader(gVertexShaderBillboard, nullptr, 0);
	gDeviceContext->HSSetShader(nullptr, nullptr, 0);
	gDeviceContext->DSSetShader(nullptr, nullptr, 0);
	gDeviceContext->GSSetShader(gGeometryShaderBillboard, nullptr, 0);
	gDeviceContext->PSSetShader(gPixelShaderBillboard, nullptr, 0);

	UINT32 vertexSize = sizeof(Vertex_Pos_Col);
	UINT32 offset = 0;
	// specify which vertex buffer to use next.
	gDeviceContext->IASetVertexBuffers(0, 1, gBillboard->getVertexBuffer(), &vertexSize, &offset);

	// specify the topology to use when drawing
	gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	// specify the IA Layout (how is data passed)
	gDeviceContext->IASetInputLayout(gVertexLayoutPosCol);

	//ConstantBuffer
	gDeviceContext->GSSetConstantBuffers(0, 1, &gConstantBuffer);
	gDeviceContext->GSSetConstantBuffers(1, 1, &gConstantBufferCamera);
	gDeviceContext->GSSetConstantBuffers(2, 1, &gConstantBufferBillboard);
	gDeviceContext->PSSetConstantBuffers(0, 1, &gConstantBufferLight);

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
	gDeviceContext->PSSetShaderResources(3, 1, &gShaderResourceShadowMap);

	gDeviceContext->PSSetConstantBuffers(0, 1, &gConstantBufferLight);
	gDeviceContext->PSSetConstantBuffers(1, 1, &gConstantBufferCamera);
	gDeviceContext->PSSetConstantBuffers(2, 1, &gConstantBufferShadowMap);

	gDeviceContext->Draw(6, 0);

	ID3D11ShaderResourceView* nullRTV = { NULL };
	gDeviceContext->PSSetShaderResources(0, 1, &nullRTV);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	MSG msg = { 0 };
	HWND wndHandle = InitWindow(hInstance); // Skapa fönster

	if (wndHandle)
	{
		CreateDirect3DContext(wndHandle); // Skapa och koppla SwapChain, Device och Device Context

		SetViewport(); // And rasterizer state

		createMeshes(); // test
		gRoot = new QuadtreeNode(40.0f, { 0.0f, 0.0f, 0.0f }, gPillars, gQuadtreeLevels, 1);

		setupTextures();

		createRenderTargets();
		createShaders();
		createShadersSP();
		createShadersShadowMap();

		createTriangleData(); //5. Definiera triangelvertiser, 6. Skapa vertex buffer, 7. Skapa input layout

		samplerSetUp();
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
		
		float pitch = 0.0f;
		float yaw = 0.0f;
		float lastT = -1.0f;
		LARGE_INTEGER clockFreq;
		LARGE_INTEGER startTime;
		LARGE_INTEGER delta;
		LARGE_INTEGER currTime;
		QueryPerformanceFrequency(&clockFreq);
		QueryPerformanceCounter(&startTime);

		///////////////
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
				case WM_RBUTTONDOWN:
					Mouse::ProcessMessage(msg.message, msg.wParam, msg.lParam);
				case WM_RBUTTONUP:
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


				mouse->SetMode(ms.rightButton ? Mouse::MODE_RELATIVE : Mouse::MODE_ABSOLUTE);
				if (ms.positionMode == Mouse::MODE_RELATIVE)
				{
					yaw += XMConvertToRadians(ms.x);
					pitch += XMConvertToRadians(ms.y);
					pitch = min(XM_PI / 2 - 0.0001, max(-XM_PI / 2 + 0.0001, pitch));
				}

				XMMATRIX rotation = XMMatrixRotationRollPitchYaw(pitch, yaw, 0.0f);
				XMMATRIX rotationYPos = XMMatrixRotationRollPitchYaw(0.0f, yaw, 0.0f);
			
				gCamera.velocity.x = 0;
				gCamera.velocity.y = 0;
				gCamera.velocity.z = 0;

				if (kb.W)
					gCamera.velocity.z += gCamera.distance * deltaSeconds;
				if (kb.S)
					gCamera.velocity.z -= gCamera.distance * deltaSeconds;
				if (kb.A)
					gCamera.velocity.x -= gCamera.distance * deltaSeconds;
				if (kb.D)
					gCamera.velocity.x += gCamera.distance * deltaSeconds;
				if (kb.Space)
					gCamera.velocity.y += gCamera.distance * deltaSeconds;
				if (kb.LeftControl)
					gCamera.velocity.y -= gCamera.distance * deltaSeconds;
				if (kb.Home)
					gCamera.pos = { 0.0f, 0.0f, -2.0f };
				if (kb.Escape)
					msg.message = WM_QUIT;

				transform(rotation, rotationYPos);
				
				GetCursorPos(&cursorPos); // gets current cursor coordinates
				ScreenToClient(wndHandle, &cursorPos); // sets cursor coordinates relative to the program window. upper left corner of the screen = (0,0)
				float tempT = mousePicking(cursorPos);
				lastT = tempT;

				//
				//
				// RENDER //
				gDeviceContext->RSSetState(gRasterizerState);
				gClearColour[3] = 1.0;
				renderShadowMap();
				
				gDeviceContext->OMSetRenderTargets(3, gRenderTargetsDeferred, gDSV);
				gDeviceContext->ClearDepthStencilView(gDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
				gDeviceContext->ClearRenderTargetView(gRenderTargetsDeferred[0], gClearColour);
				gDeviceContext->ClearRenderTargetView(gRenderTargetsDeferred[1], gClearColour);
				gDeviceContext->ClearRenderTargetView(gRenderTargetsDeferred[2], gClearColour);

				renderFirstPass();
				renderNormalMap();
				renderBoundingVolume();
				renderBillboard();

				gDeviceContext->OMSetRenderTargets(1, &gBackbufferRTV, nullptr);
				gDeviceContext->ClearRenderTargetView(gBackbufferRTV, gClearColour);

				renderSecondPass();

				update(lastT, cursorPos);

				gSwapChain->Present(0, 0);

				ImGui::Render();
				ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
			}
		}

		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();

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
		gGeometryShaderBillboard->Release();

		gDSV->Release();
		gBackbufferRTV->Release();
		gSwapChain->Release();
		gDevice->Release();
		gDeviceContext->Release();
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