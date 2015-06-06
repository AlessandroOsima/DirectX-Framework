#pragma once
#include "Directx.h"
#include "ShaderSet.h"
#include "..\Common.h"
#include "Geometry.h"
#include "../Math/Matrix44.h"

#include <vector>

class DirectxRenderer
{
public:
	DirectxRenderer();
	void Init(const WindowData & windowData, HWND hWnd);
	void Render();
	void DeInit();

    void addGeometry(const Graph::Geometry & geometryData);

	~DirectxRenderer();

private:

    void buildBufferForGeometry(unsigned int geometryId);

	//DirectX vars
	IDXGISwapChain *swapchain;
	ID3D11Device *dev;
	ID3D11DeviceContext *devcon;


	
	ID3D11DepthStencilView * depthBuffer;
	ID3D11RenderTargetView * backbuffer;

	ID3D11Buffer * constantBuffer;

	ShaderSet sSet;


    std::vector<Graph::Geometry> geometries;
    std::vector<ID3D11Buffer *> vertexBuffers;

    Math::Matrix44 projectionMatrix;

	float zMove = 0;
	bool zDirection = false;

};

