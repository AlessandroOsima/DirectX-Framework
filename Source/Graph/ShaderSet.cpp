#include "ShaderSet.h"

#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

#include <cassert>
#include "..\Windows\WinUtils.h"

namespace Graph
{
	ShaderSet::ShaderSet()
	{
		//Allowed map types
		constantBuffers.emplace(ConstantBufferBindTarget::BIND_PS, std::vector<ConstantBuffer>());
		constantBuffers.emplace(ConstantBufferBindTarget::BIND_VS, std::vector<ConstantBuffer>());
		constantBuffers.emplace(ConstantBufferBindTarget::BIND_PS | ConstantBufferBindTarget::BIND_VS, std::vector<ConstantBuffer>());
	}

	bool ShaderSet::LoadFromFile(const std::string & vertextShaderPath, const std::string & vertexShaderMainFunction, const std::string & pixelShaderPath, const std::string & pixelShaderMainFunction, ID3D11Device * dev)
	{
		assert(dev);

		ID3D10Blob  * Errors;

		HRESULT res = D3DCompileFromFile(std::wstring(vertextShaderPath.begin(), vertextShaderPath.end()).c_str(), 0, 0, vertexShaderMainFunction.c_str(), "vs_4_0", D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_PREFER_FLOW_CONTROL, 0, &VSBlob, &Errors);
		if (res != S_OK || Errors != nullptr)
		{
			WinUtils::PrintErrorMessage(res, "Error loading VS from file : ", "Error");

			if (Errors != nullptr)
			{
				char * compileError = static_cast<char*>(Errors->GetBufferPointer());
				WinUtils::PrintErrorMessageString(compileError, "Error loading VS from file : ", "Error");

				Errors->Release();
			}

			return false;
		}

		res = D3DCompileFromFile(std::wstring(pixelShaderPath.begin(), pixelShaderPath.end()).c_str(), 0, 0, pixelShaderMainFunction.c_str(), "ps_4_0", D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_PREFER_FLOW_CONTROL, 0, &PSBlob, &Errors);
		if (res != S_OK || Errors != nullptr)
		{
			WinUtils::PrintErrorMessage(res, "Error loading PS from file : ", "Error");

			if (Errors != nullptr)
			{
				char * compileError = static_cast<char*>(Errors->GetBufferPointer());
				WinUtils::PrintErrorMessageString(compileError, "Error loading VS from file : ", "Error");

				Errors->Release();
			}

			return false;
		}

		res = dev->CreateVertexShader(VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), NULL, &vertexShader);
		if (res != S_OK)
		{
			WinUtils::PrintErrorMessage(res, "Error creating VS : ", "Error");
			return false;
		}

		res = dev->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), NULL, &pixelShader);
		if (res != S_OK)
		{
			WinUtils::PrintErrorMessage(res, "Error creating PS : ", "Error");
			return false;
		}

		return true;
	}

	void ShaderSet::CreateInputLayout(D3D11_INPUT_ELEMENT_DESC desc[], int numElements, ID3D11Device * dev)
	{
		assert(dev);

		dev->CreateInputLayout(desc, numElements, VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), &inputLayout);
	}

	void ShaderSet::SetInDeviceContext(ID3D11DeviceContext * devcon)
	{
		assert(devcon);

		devcon->VSSetShader(vertexShader, 0, 0);
		devcon->PSSetShader(pixelShader, 0, 0);
	}

	void ShaderSet::SetConstantBuffers(DirectxRenderer * renderer)
	{
		assert(renderer);

		std::vector<ConstantBuffer> & vertexShaderCSB = constantBuffers[ConstantBufferBindTarget::BIND_VS];
		std::vector<ConstantBuffer> & pixelShaderCSB = constantBuffers[ConstantBufferBindTarget::BIND_PS];
		std::vector<ConstantBuffer> & pixelVertexShaderCSB = constantBuffers[ConstantBufferBindTarget::BIND_PS | ConstantBufferBindTarget::BIND_VS];

		int pos = 0;
		for (auto & buffer : vertexShaderCSB)
		{
			buffer.GenerateAndBindBuffer(pos, 0, renderer, ConstantBufferBindTarget::BIND_VS);
			pos++;
		}

		pos = 0;
		for (auto & buffer : pixelShaderCSB)
		{
			buffer.GenerateAndBindBuffer(0, pos, renderer, ConstantBufferBindTarget::BIND_PS);
			pos++;
		}

		pos = 0;
		for (auto & buffer : pixelVertexShaderCSB)
		{
			//for now they are in the same ps/vs slot, change if needed 
			buffer.GenerateAndBindBuffer(pos, pos, renderer, (unsigned int)(ConstantBufferBindTarget::BIND_PS | ConstantBufferBindTarget::BIND_VS));
			pos++;
		}
	}

	std::vector<ConstantBuffer> & ShaderSet::GetBuffersForShader(unsigned int shaderBindFlags)
	{
		return constantBuffers[shaderBindFlags];
	}

	void ShaderSet::AddConstantBuffer(ConstantBufferBindTarget target, const ConstantBuffer & constantBuffer)
	{
		constantBuffers[target].push_back(constantBuffer);
	}

	ShaderSet::~ShaderSet()
	{
		vertexShader->Release();
		pixelShader->Release();

		std::vector<ConstantBuffer> vertexShaderCSB = constantBuffers[ConstantBufferBindTarget::BIND_VS];
		std::vector<ConstantBuffer> pixelShaderCSB = constantBuffers[ConstantBufferBindTarget::BIND_PS];
		std::vector<ConstantBuffer> pixelVertexShaderCSB = constantBuffers[ConstantBufferBindTarget::BIND_PS | ConstantBufferBindTarget::BIND_VS];

		for (auto & buffer : vertexShaderCSB)
		{
			buffer.ReleaseConstantBuffer();
		}

		for (auto & buffer : pixelShaderCSB)
		{
			buffer.ReleaseConstantBuffer();
		}

		for (auto & buffer : pixelVertexShaderCSB)
		{
			//for now they are in the same ps/vs slot, change if needed 
			buffer.ReleaseConstantBuffer();
		}
	}
}