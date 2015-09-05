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
    DirectxRenderer::DirectxRenderer() :usePerspective(true)
    {
    }

    void DirectxRenderer::Init(const WindowData & windowData, HWND hWnd)
    {
		currentWindowData = windowData;

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


        /////////////////////////////////
    }

    void DirectxRenderer::OnPreRender()
    {
	    float color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	    devcon->ClearRenderTargetView(backbuffer, color);
	    devcon->ClearDepthStencilView(depthBuffer, D3D11_CLEAR_DEPTH, 1.0f, 0.f);
    }

	void DirectxRenderer::RenderGeometry(const Graph::Geometry & geometry, unsigned int geometryIndex)
    {

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
		//We should not be able to submit a geometry with 0 vertices its useless (also since the single vertex and index size is found by doing "sizeof(geometry.getIndices()[0])" if my array has size 0 I'd be going out of array bounds)
		if (!geometry.getVertices().size())
		{
			assert(0);
		}

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
				assert(0);
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
		bd.ByteWidth = sizeof(geometry.getVertices()[0]) * geometry.getVertices().size();
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
		bd.ByteWidth = sizeof(geometry.getIndices()[0]) * geometry.getIndices().size();
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

	ID3D11Buffer * DirectxRenderer::GenerateConstantBuffer(size_t size)
	{
		ID3D11Buffer * buffer;

		//Create a buffer to hold shader constant data 
		D3D11_BUFFER_DESC constantBd;
		ZeroMemory(&constantBd, sizeof(constantBd));

		constantBd.Usage = D3D11_USAGE_DEFAULT;
		constantBd.ByteWidth = size;
		constantBd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		dev->CreateBuffer(&constantBd, 0, &buffer);

		return buffer;
	}

	void DirectxRenderer::BindConstantBufferToShaderStage(UINT vsSlot, UINT psSlot, ID3D11Buffer * constantBuffer, unsigned int bindTarget)
	{

		if (bindTarget & Resources::Shaders::SHADER_TYPES::PIXEL_SHADER)
		{
			devcon->PSSetConstantBuffers(psSlot, 1, &constantBuffer);
		}

		if (bindTarget & Resources::Shaders::SHADER_TYPES::VERTEX_SHADER)
		{
			devcon->VSSetConstantBuffers(psSlot, 1, &constantBuffer);
		}
		
	}


	void DirectxRenderer::UpdateSubresource(ID3D11Buffer * resourceToUpdate, const void * data)
	{
		assert(resourceToUpdate);
		assert(data);

		devcon->UpdateSubresource(resourceToUpdate, 0, 0, data, 0, 0);
	}
	 
	void DirectxRenderer::UseShader(ShaderSet * shaderSet)
	{
		assert(shaderSet);

		currentShaderSet = shaderSet;

		currentShaderSet->SetInDeviceContext(devcon, Resources::Shaders::SHADER_TYPES::VERTEX_SHADER | Resources::Shaders::SHADER_TYPES::PIXEL_SHADER);

		devcon->IASetInputLayout(currentShaderSet->GetInputLayout());
	}

	std::unique_ptr<Graph::ShaderSet> DirectxRenderer::GenerateShaderSetFromFile(const Resources::Shaders::ShadersInfo & info)
	{
		std::unique_ptr<Graph::ShaderSet> sSet = std::make_unique<Graph::ShaderSet>();

		sSet->LoadFromFile(info, dev);

		return sSet;
	}

	void DirectxRenderer::CreateInputLayout(D3D11_INPUT_ELEMENT_DESC * ied, size_t elementCount, const void * shaderByteCode, size_t bytecodeSize, ID3D11InputLayout ** inputLayout)
	{
		assert(ied);

		dev->CreateInputLayout(ied, elementCount, shaderByteCode, bytecodeSize, inputLayout);
	}

	void DirectxRenderer::SetResources(ID3D11ShaderResourceView ** resources , unsigned int resourcesCount, unsigned int startSlot)
	{
		assert(resources);
		devcon->PSSetShaderResources(startSlot, resourcesCount, resources);
	}

}