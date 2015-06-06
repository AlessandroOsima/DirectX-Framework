#include "DirectxRenderer.h"

#include <stdio.h>
#include <tchar.h>
#include <string>

#include "../Math/Math.h"
#include "../Windows/WinUtils.h"
#include <DirectXMath.h>

/*

Math::Vertex triangle[] = 
{
{ Math::Vector3f( 0.f, 50, 0.f ), Math::Color( 1.f, 0.f, 0.f, 1.f ) },
{ Math::Vector3f( 50, -50, 0.f ), Math::Color( 0.f,  1.f, 0.f, 1.f ) },
{ Math::Vector3f( -50, -50, 0.f ), Math::Color( 0.f, 0.f, 1.f, 1.f ) },
};

*/

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

	//Create a z buffer

	ID3D11Texture2D * depthBufferTexture;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	depthBufferDesc.Width = windowData.width;
	depthBufferDesc.Height = windowData.height;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	dev->CreateTexture2D(&depthBufferDesc, NULL, &depthBufferTexture);

	D3D11_DEPTH_STENCIL_VIEW_DESC depthDesc;
	ZeroMemory(&depthDesc, sizeof(depthDesc));

	depthBufferDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;

	dev->CreateDepthStencilView(depthBufferTexture, &depthDesc, &depthBuffer);
	depthBufferTexture->Release();

	//Set the backbuffer resource as the current render target
	devcon->OMSetRenderTargets(1, &backbuffer, depthBuffer);

	//Create and set the current viewport to render to
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float)windowData.width;
	viewport.Height = (float)windowData.height;
	viewport.MaxDepth = 1;
	viewport.MinDepth = 0;

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

    //Create a buffer to hold shader constant data 
	D3D11_BUFFER_DESC constantBd;
	ZeroMemory(&constantBd, sizeof(constantBd));

	constantBd.Usage = D3D11_USAGE_DEFAULT;
	constantBd.ByteWidth = 64;
	constantBd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	dev->CreateBuffer(&constantBd, 0, &constantBuffer);

	devcon->VSSetConstantBuffers(0, 1, &constantBuffer);


    projectionMatrix.ortho(windowData.width, windowData.height, 1.0f, 100);

    Math::Matrix44 trasl;
    trasl.translate(Math::Vector3f(-windowData.width/2, -windowData.height/2,0));

    projectionMatrix = trasl * projectionMatrix;

    //Testing for matrices////////////////

    Math::Matrix44 mtxOrtho;

    mtxOrtho.ortho(windowData.width, windowData.height,10,100);

    Math::Vector3f vectorToTransform = Math::Vector3f( 160, 0, 0.f);

    Math::Vector3f resultOrtho = mtxOrtho * vectorToTransform;

	Math::Vector3f vc = Math::Vector3f(2,1,1);
	float l = vc.lenght();
	Math::Vector3f norm = vc.normal();

    //////////////////////////////////////

    //Testing geometry buffers///////

    Graph::Geometry gm;

    gm.addVertex(Math::Vertex( Math::Vector3f( -40, 40, 2 ), Math::Color( 1.f, 0.f, 0.f, 1.f )));
    gm.addVertex(Math::Vertex( Math::Vector3f( 40, -40, 2 ), Math::Color( 0.f,  1.f, 0.f, 1.f )));
    gm.addVertex(Math::Vertex( Math::Vector3f( -40, -40, 2 ), Math::Color( 0.f, 0.f, 1.f, 1.f )));

    gm.addVertex(Math::Vertex( Math::Vector3f( 40, 40, 2 ), Math::Color( 1.f, 0.f, 0.f, 1.f )));
    gm.addVertex(Math::Vertex( Math::Vector3f( 40, -40, 2 ), Math::Color( 0.f,  1.f, 0.f, 1.f )));
    gm.addVertex(Math::Vertex( Math::Vector3f( -40, 40, 2 ), Math::Color( 1.f, 0.f, 0.f, 1.f )));

    gm.translate(Math::Vector3f(500, 300, 0));
    gm.rotate(Math::Vector3f(0,0,180));
    gm.scale(Math::Vector3f(1,1,1));
    
    //addGeometry(gm);

	Graph::Geometry gm2;

	gm2.addVertex(Math::Vertex(Math::Vector3f(-40, 40, 2), Math::Color(1.f, 0.f, 0.f, 1.f)));
	gm2.addVertex(Math::Vertex(Math::Vector3f(40, -40, 2), Math::Color(0.f, 1.f, 0.f, 1.f)));
	gm2.addVertex(Math::Vertex(Math::Vector3f(-40, -40, 2), Math::Color(0.f, 0.f, 1.f, 1.f)));
	
	gm2.addVertex(Math::Vertex(Math::Vector3f(40, 40, 2), Math::Color(1.f, 0.f, 0.f, 1.f)));
	gm2.addVertex(Math::Vertex(Math::Vector3f(40, -40, 2), Math::Color(0.f, 1.f, 0.f, 1.f)));
	gm2.addVertex(Math::Vertex(Math::Vector3f(-40, 40, 2), Math::Color(1.f, 0.f, 0.f, 1.f)));
	
	gm2.translate(Math::Vector3f(200, 300, 0));
	gm2.rotate(Math::Vector3f(0, 0, 45));
	gm2.scale(Math::Vector3f(2, 2, 1));

	//addGeometry(gm2);

	Graph::Geometry gm3;

	gm3.addVertex(Math::Vertex(Math::Vector3f(-40, 40, 2), Math::Color(1.f, 0.f, 0.f, 1.f)));
	gm3.addVertex(Math::Vertex(Math::Vector3f(40, -40, 2), Math::Color(0.f, 1.f, 0.f, 1.f)));
	gm3.addVertex(Math::Vertex(Math::Vector3f(-40, -40, 2), Math::Color(0.f, 0.f, 1.f, 1.f)));
	
	
	gm3.translate(Math::Vector3f(600, 500, 0));

	//addGeometry(gm3);


	Graph::Geometry gm4;

	gm4.addVertex(Math::Vertex(Math::Vector3f(-40, 40, 0), Math::Color(1.f, 0.f, 0.f, 1.f)));
	gm4.addVertex(Math::Vertex(Math::Vector3f(40, -40, 0), Math::Color(0.f, 1.f, 0.f, 1.f)));
	gm4.addVertex(Math::Vertex(Math::Vector3f(-40, -40, 0), Math::Color(0.f, 0.f, 1.f, 1.f)));

	gm4.addVertex(Math::Vertex(Math::Vector3f(-40, 40, 0), Math::Color(1.f, 0.f, 0.f, 1.f)));
	gm4.addVertex(Math::Vertex(Math::Vector3f(40, 40, 0), Math::Color(0.f, 0.f, 1.f, 1.f)));
	gm4.addVertex(Math::Vertex(Math::Vector3f(40, -40, 0), Math::Color(0.f, 1.f, 0.f, 1.f)));

	gm4.addVertex(Math::Vertex(Math::Vector3f(-40, -40, 40), Math::Color(0.f, 0.f, 1.f, 1.f)));
	gm4.addVertex(Math::Vertex(Math::Vector3f(40, -40, 40), Math::Color(0.f, 1.f, 0.f, 1.f)));
	gm4.addVertex(Math::Vertex(Math::Vector3f(-40, 40, 40), Math::Color(1.f, 0.f, 0.f, 1.f)));
	
	gm4.addVertex(Math::Vertex(Math::Vector3f(40, -40, 40), Math::Color(0.f, 1.f, 0.f, 1.f)));
	gm4.addVertex(Math::Vertex(Math::Vector3f(40, 40, 40), Math::Color(0.f, 0.f, 1.f, 1.f)));
	gm4.addVertex(Math::Vertex(Math::Vector3f(-40, 40, 40), Math::Color(1.f, 0.f, 0.f, 1.f)));

	gm4.addVertex(Math::Vertex(Math::Vector3f(40, 40, 0), Math::Color(0.f, 0.f, 1.f, 1.f)));
	gm4.addVertex(Math::Vertex(Math::Vector3f(40, 40, 40), Math::Color(0.f, 1.f, 0.f, 1.f)));
	gm4.addVertex(Math::Vertex(Math::Vector3f(40, -40, 0), Math::Color(0.f, 1.f, 0.f, 1.f)));

	gm4.addVertex(Math::Vertex(Math::Vector3f(40, -40, 0), Math::Color(0.f, 1.f, 0.f, 1.f)));
	gm4.addVertex(Math::Vertex(Math::Vector3f(40, 40, 40), Math::Color(0.f, 1.f, 0.f, 1.f)));
	gm4.addVertex(Math::Vertex(Math::Vector3f(40, -40, 40), Math::Color(1, 0.f, 0.f, 1.f)));



	gm4.translate(Math::Vector3f(0, 0 , 500));
    gm4.rotate(Math::Vector3f(0,0,0));

	addGeometry(gm4);

    /////////////////////////////////
}

void DirectxRenderer::Render()
{
	float color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	devcon->ClearRenderTargetView(backbuffer, color);
	devcon->ClearDepthStencilView(depthBuffer, D3D11_CLEAR_DEPTH, 1.0f, 0.f);

	//Update constant buffer
    
    /*
    DirectX::XMVECTOR eye = DirectX::XMVectorSet( 0, 0, 0, 0 );
    DirectX::XMVECTOR focus = DirectX::XMVectorSet( 0, 0, 10, 0 );
    DirectX::XMVECTOR up = DirectX::XMVectorSet( 0, 1, 0, 0 );

    DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(eye, focus, up);
    
    //DirectX::XMMATRIX perspective = DirectX::XMMatrixOrthographicOffCenterLH(0, 800, 0, 600, 1.0f, 100.0f);

    DirectX::XMMATRIX perspective = DirectX::XMMatrixOrthographicLH(800, 600, 1.0f, 100.0f);
    //


    DirectX::XMMATRIX perspectiveView =  DirectX::XMMatrixMultiply(view , perspective);

    */   

	//DirectX::XMMATRIX perspective = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(45), 800 / 600, 1.0f, 100.0f);


	Math::Matrix44 perspective;
	perspective.perspective(Math::Deg2Rad(45), 800 / 600, 1.0f, 2000.0f);

	Math::Vector3f eye = Math::Vector3f(0,0,0);
	Math::Vector3f at = Math::Vector3f(0,0,10);
	Math::Vector3f up = Math::Vector3f(0,1,0);

	Math::Matrix44 lookAt;
	lookAt.lookAt(eye, at, up);

	bool usePerspective = true;

	if (usePerspective)
	{
		if (zDirection && zMove >= 360)
		{
			zDirection = false;
		}

		if (!zDirection && zMove <= 0)
		{
			zDirection = true;
		}

		if (zDirection)
		{
			zMove += 0.001f;
		}
		else
		{
			zMove -= 0.001f;
		}


		geometries[0].rotate(Math::Vector3f(0, zMove, 0));
	}


    for (unsigned int i = 0; i < vertexBuffers.size(); i++)
    {

		Math::Matrix44 finalMatrix;

		finalMatrix.identity();

		if (usePerspective)
		{
			finalMatrix = geometries[i].getWorld() * lookAt * perspective;
		}
		else
		{
			finalMatrix = geometries[i].getWorld() * projectionMatrix;
		}

        devcon->UpdateSubresource(constantBuffer, 0, 0, &finalMatrix, 0, 0);

        ID3D11Buffer * vertexBuffer = vertexBuffers[i];

        UINT stride = sizeof(Math::Vertex);
        UINT offset = 0;

        devcon->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

        devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        //Draw on the backbuffer with the current viewport
        unsigned int numToDraw = geometries[i].getVertices().size();
        devcon->Draw(numToDraw, 0);
    }

	//Swap the backbuffer to the screen
	swapchain->Present(0, 0);
}

void DirectxRenderer::DeInit()
{
	swapchain->SetFullscreenState(FALSE, NULL);

    for(auto vertexBuffer : vertexBuffers)
    {
        vertexBuffer->Release();
    }

	backbuffer->Release();
	depthBuffer->Release();
	devcon->Release();
	dev->Release();
	swapchain->Release();
}


void DirectxRenderer::addGeometry(const Graph::Geometry & geometryData)
{
	if (geometryData.getVertices().size() == 0)
	{
		return;
	}

    geometries.push_back(geometryData);
    buildBufferForGeometry(geometries.size() - 1);
}

DirectxRenderer::~DirectxRenderer()
{
}

void DirectxRenderer::buildBufferForGeometry(unsigned int geometryId)
{

    if(geometryId < 0 || geometryId >= geometries.size())
    {
        WinUtils::PrintErrorMessageString("[Renderer]Geometry ID is outside geometries array boundaries","","Error");
        return;
    }

    ID3D11Buffer * vertexBuffer;

    //create a vertex buffer
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof(Math::Vertex) * geometries[geometryId].getVertices().size();
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    dev->CreateBuffer(&bd, nullptr, &vertexBuffer);

    //map and copy vertex data to the created vertex buffer
    D3D11_MAPPED_SUBRESOURCE ms;
    devcon->Map(vertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
    memcpy(ms.pData, geometries[geometryId].getVertices().data(), sizeof(Math::Vertex) *geometries[geometryId].getVertices().size());
    devcon->Unmap(vertexBuffer, NULL);

    vertexBuffers.push_back(vertexBuffer);
}
