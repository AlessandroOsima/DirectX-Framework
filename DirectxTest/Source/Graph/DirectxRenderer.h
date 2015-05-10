#pragma once
#include "Directx.h"
#include "ShaderSet.h"
#include "..\Common.h"

class DirectxRenderer
{
public:
	DirectxRenderer();
	void Init(const WindowData & windowData, HWND hWnd);
	void Render();
	void DeInit();

	~DirectxRenderer();

private:
	//DirectX vars
	IDXGISwapChain *swapchain;
	ID3D11Device *dev;
	ID3D11DeviceContext *devcon;
	ID3D11RenderTargetView * backbuffer;

	ID3D11Buffer * vertexBuffer;

	ID3D11Buffer * constantBuffer;

	ShaderSet sSet;
};

