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
		void RenderGeometry(const Math::Color & ambientLight, const DirectionalLightProperties * directionalLights, const PointLightProperties * pointLights, const Graph::Geometry & geometry, unsigned int geometryIndex, unsigned int activeDirectionalLights, unsigned int activePointLights);
        void OnPostRender();

        void DeInit();

        void BuildBuffersForGeometry(const Graph::Geometry & geometry, unsigned int indexToBuffer);
        void SetPrimitiveTopology(PrimitiveTopology primitiveTopology);

		bool CreateTextureResources(const std::string & filename, ID3D11Resource ** texture, ID3D11ShaderResourceView ** shaderResource);

        ~DirectxRenderer();

    private:

        void buildVertexBufferForGeometry(const Graph::Geometry & geometry, unsigned int geometryId);
        void buildIndexBufferForGeometry(const Graph::Geometry & geometry, unsigned int geometryId);

        //DirectX vars
        IDXGISwapChain *swapchain;
        ID3D11Device *dev;
        ID3D11DeviceContext *devcon;



        ID3D11DepthStencilView * depthBuffer;
        ID3D11RenderTargetView * backbuffer;

        ID3D11Buffer * constantBuffer;
		ID3D11Buffer * psConstantBuffer;
		ID3D11Buffer * perFramePSConstantBuffer;

        ShaderSet sSet;


        std::vector<ID3D11Buffer *> vertexBuffers;
        std::vector<ID3D11Buffer *> indexBuffers;

        Math::Matrix44 projectionMatrix;
        Math::Matrix44 lookAtMatrix;
        Math::Vector4f eyeLocation;
        bool usePerspective;
    };
}

