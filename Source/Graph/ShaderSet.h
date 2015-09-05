#pragma once
#include <string>
#include <map>
#include "Directx.h"
#include "ConstantBuffer.h"
#include <memory>
#include <vector>
#include "Graph.h"
#include "../Resources/Resources.h"

namespace Graph
{
	class DirectxRenderer;

	class ShaderSet
	{
	public:
		ShaderSet();

		bool LoadFromFile(const Resources::Shaders::ShadersInfo & shadersInfo, ID3D11Device * dev);
		void CreateInputLayout(D3D11_INPUT_ELEMENT_DESC desc[], int numElements, DirectxRenderer * dev);
		void SetInDeviceContext(ID3D11DeviceContext * devcon, unsigned int shadersToBind);
		void GenerateConstantBuffer(DirectxRenderer * renderer);
		void BindConstantBuffer(DirectxRenderer * renderer);

		std::vector<ConstantBuffer> & GetBuffersForShader(Resources::Shaders::SHADER_TYPES shaderToBind);

		inline ID3D11InputLayout * GetInputLayout() { return inputLayout; };

		void AddConstantBuffer(Resources::Shaders::SHADER_TYPES target, const ConstantBuffer & constantBuffer);

		void Unload();

		~ShaderSet();
	private:
		
		ID3DBlob * LoadShaderFromFile(const Resources::Shaders::ShaderTypeInfo & info, ID3D11Device * dev);
		std::map<unsigned int, std::vector<ConstantBuffer>> constantBuffers;

		ID3D10Blob * VSBlob;
		ID3D10Blob * PSBlob;

		ID3D11VertexShader * vertexShader;    // the vertex shader
		ID3D11HullShader * hullShader;     // the hull shader
		ID3D11DomainShader * domainShader;     // the domain shader
		ID3D11PixelShader * pixelShader;     // the pixel shader

		ID3D11InputLayout * inputLayout;
	};
}

