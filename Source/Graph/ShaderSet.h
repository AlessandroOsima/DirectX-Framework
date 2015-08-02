#pragma once
#include <string>
#include <map>
#include "Directx.h"
#include "ConstantBuffer.h"
#include <memory>
#include <vector>
#include "Graph.h"

namespace Graph
{
	class DirectxRenderer;

	class ShaderSet
	{
	public:
		ShaderSet();

		bool LoadFromFile(const std::string & vertextShaderPath, const std::string & vertexShaderMainFunction, const std::string & pixelShaderPath, const std::string & pixelShaderMainFunction, ID3D11Device * dev);
		void CreateInputLayout(D3D11_INPUT_ELEMENT_DESC desc[], int numElements, ID3D11Device * dev);
		void SetInDeviceContext(ID3D11DeviceContext * devcon);
		void GenerateConstantBuffer(DirectxRenderer * renderer);
		void BindConstantBuffer(DirectxRenderer * renderer);


		std::vector<ConstantBuffer> & GetBuffersForShader(unsigned int shaderBindFlags);

		inline ID3D11InputLayout * GetInputLayout() { return inputLayout; };

		void AddConstantBuffer(ConstantBufferBindTarget target, const ConstantBuffer & constantBuffer);

		~ShaderSet();
	private:
		
		std::map<unsigned int, std::vector<ConstantBuffer>> constantBuffers;

		ID3D10Blob * VSBlob;
		ID3D10Blob * PSBlob;

		ID3D11VertexShader * vertexShader;    // the vertex shader
		ID3D11PixelShader * pixelShader;     // the pixel shader
		ID3D11InputLayout * inputLayout;
	};
}

