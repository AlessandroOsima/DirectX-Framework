#pragma once
#include "Directx.h"
#include "ShaderSet.h"
#include "../Common.h"
#include "Graph.h"
#include "Geometry.h"
#include "Lights/Light.h"
#include "../Math/Matrix44.h"
#include <vector>

namespace Graph
{

    class DirectxRenderer
    {
    public:
        DirectxRenderer();
        void Init(const WindowData & windowData, HWND hWnd);

        void OnPreRender();
		void RenderGeometry(const Graph::Geometry & geometry, unsigned int geometryIndex);
        void OnPostRender();

        void DeInit();

        void BuildBuffersForGeometry(const Graph::Geometry & geometry, unsigned int indexToBuffer);
        void SetPrimitiveTopology(PrimitiveTopology primitiveTopology);

		//Constant buffer
		bool CreateTextureResources(const std::string & filename, ID3D11Resource ** texture, ID3D11ShaderResourceView ** shaderResource);
		ID3D11Buffer * GenerateConstantBuffer(size_t size);
		void BindConstantBufferToShaderStage(UINT vsSlot, UINT psSlot, ID3D11Buffer * constantBuffer, unsigned int bindTarget);
		void UpdateSubresource(ID3D11Buffer * resourceToUpdate, const void * data);
		////////////////

		//Shader
		void UseShader(ShaderSet * shaderSet);
		std::unique_ptr<Graph::ShaderSet> GenerateShaderSetFromFile(const std::string & vertextShaderPath, const std::string & vertexShaderMainFunction, const std::string & pixelShaderPath, const std::string & pixelShaderMainFunction);
		void CreateInputLayout(ShaderSet & inputLayoutShader, D3D11_INPUT_ELEMENT_DESC * ied, size_t elementSize);
		////////////////

		//Resources
		void SetResources(ID3D11ShaderResourceView ** resources, unsigned int resourcesCount, unsigned int startSlot);
		//

		inline const WindowData & GetCurrentWindowData()
		{
			return currentWindowData;
		}

        ~DirectxRenderer();

    private:

        void buildVertexBufferForGeometry(const Graph::Geometry & geometry, unsigned int geometryId);
        void buildIndexBufferForGeometry(const Graph::Geometry & geometry, unsigned int geometryId);

        //DirectX vars
        IDXGISwapChain *swapchain;
        ID3D11Device *dev;
        ID3D11DeviceContext *devcon;
		ShaderSet * currentShaderSet;


        ID3D11DepthStencilView * depthBuffer;
        ID3D11RenderTargetView * backbuffer;

        ID3D11Buffer * constantBuffer;
		ID3D11Buffer * psConstantBuffer;
		ID3D11Buffer * perFramePSConstantBuffer;

		WindowData currentWindowData;

        //ShaderSet sSet;


        std::vector<ID3D11Buffer *> vertexBuffers;
        std::vector<ID3D11Buffer *> indexBuffers;
        bool usePerspective;
    };
}

