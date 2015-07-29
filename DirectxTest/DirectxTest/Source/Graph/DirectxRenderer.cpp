#include "DirectxRenderer.h"

#include <stdio.h>
#include <tchar.h>
#include <string>

#include "../Math/Math.h"
#include "../Windows/WinUtils.h"
#include "Lights/Light.h"
#include "../ThirdParty/DDSTextureLoader/DDSTextureLoader.h"
#include <DirectXMath.h>

/*

Math::Vertex triangle[] = 
{
{ Math::Vector3f( 0.f, 50, 0.f ), Math::Color( 1.f, 0.f, 0.f, 1.f ) },
{ Math::Vector3f( 50, -50, 0.f ), Math::Color( 0.f,  1.f, 0.f, 1.f ) },
{ Math::Vector3f( -50, -50, 0.f ), Math::Color( 0.f, 0.f, 1.f, 1.f ) },
};

*/


namespace Graph
{
	struct ConstantBuffer
	{
		Math::Matrix44 finalMatrix;
		Math::Matrix44 modelMatrix;
		Math::Vector4f eye;
		Math::Color ambientLight;
		DirectionalLightProperties  directionalLights[Constants::MAX_DIRECTIONAL_LIGHTS];
		PointLightProperties  pointLights[Constants::MAX_POINT_LIGHTS];
	};

    DirectxRenderer::DirectxRenderer() :usePerspective(true)
    {
	    projectionMatrix.identity();
	    lookAtMatrix.identity();
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

	    //Create a depth buffer texture (32 bit float texture to store depth data)
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


	    //Create a depth stencil view and bind the texture to it.
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

	    TCHAR * shadersPath = "\\Data\\Shaders\\shaders.hlsl";

	    int currentDirStringLenght = currentDirectorySize + _tcslen(shadersPath);

	    TCHAR * currentDirectoryBuffer = new TCHAR[currentDirStringLenght];

	    GetCurrentDirectory(currentDirectorySize, currentDirectoryBuffer);

	    _tcsncat_s(currentDirectoryBuffer, currentDirStringLenght, shadersPath, _tcslen(shadersPath));

	    std::string shaderFilePath = std::string(currentDirectoryBuffer);
	    //////////////////////////////////////////////////////////////////////////

	    //Load shader set
	    sSet.LoadFromFile(shaderFilePath, "VShader", shaderFilePath, "PShader", dev);
	    sSet.SetInDeviceContext(devcon);

	    delete[] currentDirectoryBuffer;

	    //shader input layout setup
	    D3D11_INPUT_ELEMENT_DESC ied[] =
	    {
		    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		    { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	    };

	    sSet.CreateInputLayout(ied, 4, dev);

	    //set pipeline inputs
	    devcon->IASetInputLayout(sSet.GetInputLayout());

        //Create a buffer to hold shader constant data 
	    D3D11_BUFFER_DESC constantBd;
	    ZeroMemory(&constantBd, sizeof(constantBd));

	    constantBd.Usage = D3D11_USAGE_DEFAULT;
		constantBd.ByteWidth = 144 + sizeof(Math::Vector4f) + sizeof(DirectionalLightProperties) * Constants::MAX_DIRECTIONAL_LIGHTS + sizeof(PointLightProperties) * Constants::MAX_POINT_LIGHTS;
	    constantBd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	    dev->CreateBuffer(&constantBd, 0, &constantBuffer);

	    devcon->VSSetConstantBuffers(0, 1, &constantBuffer);
		devcon->PSSetConstantBuffers(0, 1, &constantBuffer);

	    if (usePerspective)
	    {
		    projectionMatrix.perspective(Math::Deg2Rad(45), 800 / 600, 1.0f, 2000.0f);

            eyeLocation = Math::Vector4f(0,0,0,0);

		    Math::Vector3f eye = Math::Vector3f(eyeLocation.x, eyeLocation.y, eyeLocation.z);
		    Math::Vector3f at = Math::Vector3f(0, 0, 10);
		    Math::Vector3f up = Math::Vector3f(0, 1, 0);

		    lookAtMatrix.lookAt(eye, at, up);

	    }
	    else
	    {
		    projectionMatrix.ortho(windowData.width, windowData.height, 1.0f, 100);

		    Math::Matrix44 trasl;
		    trasl.translate(Math::Vector3f(-windowData.width / 2, -windowData.height / 2, 0));

		    projectionMatrix = trasl * projectionMatrix;
	    }


        /////////////////////////////////
    }

    void DirectxRenderer::OnPreRender()
    {
	    float color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	    devcon->ClearRenderTargetView(backbuffer, color);
	    devcon->ClearDepthStencilView(depthBuffer, D3D11_CLEAR_DEPTH, 1.0f, 0.f);
    }

	void DirectxRenderer::RenderGeometry(const Math::Color & ambientLight, const DirectionalLightProperties * directionalLights, const PointLightProperties * pointLights, const Graph::Geometry & geometry, unsigned int geometryIndex)
    {
		assert(directionalLights);
		assert(pointLights);

	    Math::Matrix44 finalMatrix;

	    finalMatrix.identity();

	    if (usePerspective)
	    {
		    finalMatrix = geometry.getWorld() * lookAtMatrix * projectionMatrix;
	    }
	    else
	    {
		    finalMatrix = geometry.getWorld() * projectionMatrix;
	    }

		ID3D11ShaderResourceView * resources[1] = { geometry.GetDiffuseTexture().GetShaderResource()};

		devcon->PSSetShaderResources(0, 1, resources);

		ConstantBuffer cBuffer; 
        cBuffer.finalMatrix = finalMatrix; 
        cBuffer.modelMatrix = geometry.getRotation();
        cBuffer.eye = eyeLocation;
		cBuffer.ambientLight = ambientLight;
		
		for (int i = 0; i < Constants::MAX_DIRECTIONAL_LIGHTS; i++)
		{
			cBuffer.directionalLights[i] = directionalLights[i];
		}

		for (int i = 0; i < Constants::MAX_POINT_LIGHTS; i++)
		{
			cBuffer.pointLights[i] = pointLights[i];
		}

	    devcon->UpdateSubresource(constantBuffer, 0, 0, &cBuffer, 0, 0);

	    ID3D11Buffer * vertexBuffer = vertexBuffers[geometryIndex];

	    UINT stride = sizeof(Math::Vertex);
	    UINT offset = 0;

	    devcon->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	    devcon->IASetIndexBuffer(indexBuffers[geometryIndex], DXGI_FORMAT_R32_UINT, 0);
	

	    //Draw on the backbuffer with the current viewport
	    //unsigned int numToDraw = geometry.getVertices().size();
	    //devcon->Draw(numToDraw, 0);

	    devcon->DrawIndexed(geometry.getIndices().size(), 0, 0);
    }

    void DirectxRenderer::OnPostRender()
    {
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


    void DirectxRenderer::BuildBuffersForGeometry(const Graph::Geometry & geometry, unsigned int indexToBuffer)
    {

	    buildVertexBufferForGeometry(geometry, indexToBuffer);
	    buildIndexBufferForGeometry(geometry, indexToBuffer);

    }

    void DirectxRenderer::SetPrimitiveTopology(PrimitiveTopology primitiveTopology)
    {
        D3D11_PRIMITIVE_TOPOLOGY dxPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

        switch (primitiveTopology)
        {
        case Graph::PrimitiveTopology::PointList:
            {
                dxPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
                break;
            }

        case Graph::PrimitiveTopology::LineList:
            {
                dxPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
                break;
            }
            

        case Graph::PrimitiveTopology::TriangleList:
            {
                dxPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
                break;
            }

        default:
            {

                break;
            }
        }

        devcon->IASetPrimitiveTopology(dxPrimitiveTopology);
    }

    DirectxRenderer::~DirectxRenderer()
    {
    }

    void DirectxRenderer::buildVertexBufferForGeometry(const Graph::Geometry & geometry, unsigned int geometryId)
    {

        ID3D11Buffer * vertexBuffer;

        //create a vertex buffer
        D3D11_BUFFER_DESC bd;
        ZeroMemory(&bd, sizeof(bd));

        bd.Usage = D3D11_USAGE_DYNAMIC;
	    bd.ByteWidth = sizeof(Math::Vertex) * geometry.getVertices().size();
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

        dev->CreateBuffer(&bd, nullptr, &vertexBuffer);

        //map and copy vertex data to the created vertex buffer
        D3D11_MAPPED_SUBRESOURCE ms;
        devcon->Map(vertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	    memcpy(ms.pData, geometry.getVertices().data(), sizeof(Math::Vertex) * geometry.getVertices().size());
        devcon->Unmap(vertexBuffer, NULL);


	    if (geometryId < vertexBuffers.size())
	    {
		    vertexBuffers[geometryId]->Release();
		    vertexBuffers[geometryId] = vertexBuffer;
	    }
	    else
	    {
		    assert(geometryId == vertexBuffers.size());

		    vertexBuffers.push_back(vertexBuffer);
	    }
    
    }

    void DirectxRenderer::buildIndexBufferForGeometry(const Graph::Geometry & geometry, unsigned int geometryId)
    {

	    ID3D11Buffer * indexBuffer;

	    //create a vertex buffer
	    D3D11_BUFFER_DESC bd;
	    ZeroMemory(&bd, sizeof(bd));

	    bd.Usage = D3D11_USAGE_DYNAMIC;
	    bd.ByteWidth = sizeof(int) * geometry.getIndices().size();
	    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	    dev->CreateBuffer(&bd, nullptr, &indexBuffer);

	    //map and copy vertex data to the created vertex buffer
	    D3D11_MAPPED_SUBRESOURCE ms;
	    devcon->Map(indexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	    memcpy(ms.pData, geometry.getIndices().data(), sizeof(int) * geometry.getIndices().size());
	    devcon->Unmap(indexBuffer, NULL);


	    if (geometryId < indexBuffers.size())
	    {
		    indexBuffers[geometryId]->Release();
		    indexBuffers[geometryId] = indexBuffer;
	    }
	    else
	    {
		    assert(geometryId == indexBuffers.size());

		    indexBuffers.push_back(indexBuffer);
	    }

    }

	bool DirectxRenderer::CreateTextureResources(const std::string & filename, ID3D11Resource ** texture, ID3D11ShaderResourceView ** shaderResource)
	{
		HRESULT hr = DirectX::CreateDDSTextureFromFile(dev, std::wstring(filename.begin(), filename.end()).c_str(), texture, shaderResource);
		
		if (hr != S_OK)
		{
			return false;
		}

		return true;
	}

}