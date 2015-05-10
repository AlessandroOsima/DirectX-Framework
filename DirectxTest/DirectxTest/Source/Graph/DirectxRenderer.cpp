#include "DirectxRenderer.h"

#include <stdio.h>
#include <tchar.h>
#include <string>
#include "Math.h"

Math::Vertex triangle[] = 
{
	{ Math::Vector3{ 0.f, 0.5f, 0.f }, Math::Color{ 1.f, 0.f, 0.f, 1.f } },
	{ Math::Vector3{ 0.5f, -0.5f, 0.f }, Math::Color{ 0.f,  1.f, 0.f, 1.f } },
	{ Math::Vector3{ -0.5f, -0.5f, 0.f }, Math::Color{ 0.f, 0.f, 1.f, 1.f } },
};


DirectxRenderer::DirectxRenderer()
{
}

void DirectxRenderer::Init(const WindowData & windowData, HWND hWnd)
{
	//Create swap chain and directx device and device context
	DXGI_SWAP_CHAIN_DESC swapChainDesc;

	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.Width = windowData.width;
	swapChainDesc.BufferDesc.Height = windowData.height;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.Windowed = !windowData.fullscreen;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;


	D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&swapchain,
		&dev,
		NULL,
		&devcon);

	//Get the backbuffer from the swapchain and set it as render target view
	ID3D11Texture2D * pBackbuffer;
	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackbuffer);

	dev->CreateRenderTargetView(pBackbuffer, NULL, &backbuffer);
	pBackbuffer->Release();

	//Set the backbuffer resource as the current render target
	devcon->OMSetRenderTargets(1, &backbuffer, NULL);

	//Create and set the current viewport to render to
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float)windowData.width;
	viewport.Height = (float)windowData.height;

	devcon->RSSetViewports(1, &viewport);

	//Get the current app directory and use to find the shader files
	//windows api bullshit//////////////////////////////////////////////////////
	DWORD currentDirectorySize = GetCurrentDirectory(0, nullptr);

	TCHAR * shadersPath = L"\\Data\\Shaders\\shaders.hlsl";

	int currentDirStringLenght = currentDirectorySize + _tcslen(shadersPath);

	TCHAR * currentDirectoryBuffer = new TCHAR[currentDirStringLenght];

	GetCurrentDirectory(currentDirectorySize, currentDirectoryBuffer);

	_tcsncat_s(currentDirectoryBuffer, currentDirStringLenght, shadersPath, _tcslen(shadersPath));

	std::wstring shaderFilePath = std::wstring(currentDirectoryBuffer);
	//////////////////////////////////////////////////////////////////////////

	//Load shader set
	sSet.LoadFromFile(shaderFilePath, "VShader", shaderFilePath, "PShader", dev);
	sSet.SetInDeviceContext(devcon);

	delete[] currentDirectoryBuffer;

	//shader input layout setup
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	sSet.CreateInputLayout(ied, 2, dev);
	//set pipeline inputs
	devcon->IASetInputLayout(sSet.GetInputLayout());

	//Vertex buffer setup
	
	//create a vertex buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(Math::Vertex) * 3;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	dev->CreateBuffer(&bd, nullptr, &vertexBuffer);

	//map and copy vertex data to the created vertex buffer
	D3D11_MAPPED_SUBRESOURCE ms;
	devcon->Map(vertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, triangle, sizeof(triangle));
	devcon->Unmap(vertexBuffer, NULL);


    //Create a buffer to hold shader constant data 
	D3D11_BUFFER_DESC constantBd;
	ZeroMemory(&constantBd, sizeof(constantBd));

	constantBd.Usage = D3D11_USAGE_DEFAULT;
	constantBd.ByteWidth = 16;
	constantBd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	dev->CreateBuffer(&constantBd, 0, &constantBuffer);

	devcon->VSSetConstantBuffers(0, 1, &constantBuffer);

}

void DirectxRenderer::Render()
{
	float color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	devcon->ClearRenderTargetView(backbuffer, color);

	//Update constant buffer
	Math::Vector3 vector = { 0.5f, 0.5f, 0 };
	devcon->UpdateSubresource(constantBuffer, 0, 0, &vector, 0, 0);

	//Set the vertex buffer and primitive topology
	UINT stride = sizeof(Math::Vertex);
	UINT offset = 0;
	devcon->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Draw on the backbuffer with the current viewport
	devcon->Draw(sizeof(triangle)/sizeof(Math::Vertex), 0);

	//Swap the backbuffer to the screen
	swapchain->Present(0, 0);
}

void DirectxRenderer::DeInit()
{
	swapchain->SetFullscreenState(FALSE, NULL);

	backbuffer->Release();
	devcon->Release();
	dev->Release();
	swapchain->Release();
}

DirectxRenderer::~DirectxRenderer()
{
}
