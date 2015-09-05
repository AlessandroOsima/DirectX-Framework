#include "ShaderSet.h"

#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

#include <cassert>
#include <string>

#include "..\Windows\WinUtils.h"
#include "../Graph/DirectxRenderer.h"

namespace Graph
{
	ShaderSet::ShaderSet()
	{
		//Allowed map types
		constantBuffers.emplace(Resources::Shaders::SHADER_TYPES::VERTEX_SHADER, std::vector<ConstantBuffer>());
		constantBuffers.emplace(Resources::Shaders::SHADER_TYPES::PIXEL_SHADER, std::vector<ConstantBuffer>());
		constantBuffers.emplace(Resources::Shaders::SHADER_TYPES::VERTEX_SHADER | Resources::Shaders::SHADER_TYPES::PIXEL_SHADER, std::vector<ConstantBuffer>());
	}

	/*
	bool ShaderSet::LoadFromFile(const std::string & vertextShaderPath, const std::string & vertexShaderMainFunction, const std::string & pixelShaderPath, const std::string & pixelShaderMainFunction, ID3D11Device * dev)
	{
		assert(dev);


		DWORD currentDirectorySize = GetCurrentDirectory(0, nullptr);

		TCHAR * shadersPath = "\\Data\\Shaders\\";

		int currentDirStringLenght = currentDirectorySize + _tcslen(shadersPath);

		TCHAR * currentDirectoryBuffer = new TCHAR[currentDirStringLenght];

		GetCurrentDirectory(currentDirectorySize, currentDirectoryBuffer);

		_tcsncat_s(currentDirectoryBuffer, currentDirStringLenght, shadersPath, _tcslen(shadersPath));

		std::string shaderFilePath = std::string(currentDirectoryBuffer);

		std::string completeVertexShaderFilePath = shaderFilePath + vertextShaderPath;
		std::string completePixelShaderFilePath = shaderFilePath + pixelShaderPath;

		delete[] currentDirectoryBuffer;


		ID3D10Blob  * Errors;

		HRESULT res = D3DCompileFromFile(std::wstring(completeVertexShaderFilePath.begin(), completeVertexShaderFilePath.end()).c_str(), 0, 0, vertexShaderMainFunction.c_str(), "vs_4_0", D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_PREFER_FLOW_CONTROL, 0, &VSBlob, &Errors);
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

		res = D3DCompileFromFile(std::wstring(completePixelShaderFilePath.begin(), completePixelShaderFilePath.end()).c_str(), 0, 0, pixelShaderMainFunction.c_str(), "ps_4_0", D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_PREFER_FLOW_CONTROL, 0, &PSBlob, &Errors);
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
	*/

	bool ShaderSet::LoadFromFile(const Resources::Shaders::ShadersInfo & shadersInfo, ID3D11Device * dev)
	{
		assert(dev);

		bool shaderLoadResult = true;

		if (shadersInfo.vertexShaderInfo)
		{
			ID3DBlob * shaderCode = LoadShaderFromFile(*shadersInfo.vertexShaderInfo, dev);

			if (!shaderCode)
			{
				return false;
			}

			VSBlob = shaderCode;

			HRESULT res = dev->CreateVertexShader(shaderCode->GetBufferPointer(), shaderCode->GetBufferSize(), NULL, &vertexShader);
			if (FAILED(res))
			{
				WinUtils::PrintErrorMessage(res, "Error creating VS : ", "Error");

				VSBlob->Release();

				return false;
			}

		}

		if (shadersInfo.pixelShaderInfo)
		{
			ID3DBlob * shaderCode = LoadShaderFromFile(*shadersInfo.pixelShaderInfo, dev);

			if (!shaderCode)
			{
				vertexShader->Release();
				return false;
			}

			PSBlob = shaderCode;

			HRESULT res = dev->CreatePixelShader(shaderCode->GetBufferPointer(), shaderCode->GetBufferSize(), NULL, &pixelShader);
			if (FAILED(res))
			{
				WinUtils::PrintErrorMessage(res, "Error creating PS : ", "Error");

				//clean up previously loaded shaders
				VSBlob->Release();
				vertexShader->Release();
				shaderCode->Release();

				return false;
			}

		}

		return true;
	}


	ID3DBlob * ShaderSet::LoadShaderFromFile(const Resources::Shaders::ShaderTypeInfo & info, ID3D11Device * dev)
	{
		assert(dev);

		ID3DBlob  * Errors;
		ID3DBlob * shaderCode;

		auto fileLocation = std::wstring(info.fileName.begin(), info.fileName.end());
		HRESULT res = D3DCompileFromFile(fileLocation.c_str(), 0, 0, info.mainFunctionName.c_str(), info.profile.c_str(), info.compilationFlags, 0, &shaderCode, &Errors);
		if (FAILED(res) || Errors != nullptr)
		{

			if (Errors != nullptr)
			{
				char * compileError = static_cast<char*>(Errors->GetBufferPointer());
				WinUtils::PrintErrorMessageString(compileError, "Error loading shader from file : ", "Error");

				Errors->Release();
			}
			else
			{
				WinUtils::PrintErrorMessage(res, "Error loading shader from file : ", "Error");
			}

			return nullptr;
		}

		return shaderCode;
	}


	void ShaderSet::CreateInputLayout(D3D11_INPUT_ELEMENT_DESC desc[], int numElements, DirectxRenderer * renderer)
	{
		assert(renderer);


		renderer->CreateInputLayout(desc, numElements, VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), &inputLayout);
	}

	void ShaderSet::SetInDeviceContext(ID3D11DeviceContext * devcon, unsigned int shadersToBind)
	{
		assert(devcon);

		if (shadersToBind & Resources::Shaders::SHADER_TYPES::VERTEX_SHADER)
		{
			devcon->VSSetShader(vertexShader, 0, 0);
		}

		if (shadersToBind & Resources::Shaders::SHADER_TYPES::PIXEL_SHADER)
		{
			devcon->PSSetShader(pixelShader, 0, 0);
		}
	}

	void ShaderSet::GenerateConstantBuffer(DirectxRenderer * renderer)
	{
		assert(renderer);

		std::vector<ConstantBuffer> & vertexShaderCSB = constantBuffers[Resources::Shaders::SHADER_TYPES::VERTEX_SHADER];
		std::vector<ConstantBuffer> & pixelShaderCSB = constantBuffers[Resources::Shaders::SHADER_TYPES::PIXEL_SHADER];
		std::vector<ConstantBuffer> & pixelVertexShaderCSB = constantBuffers[Resources::Shaders::SHADER_TYPES::PIXEL_SHADER | Resources::Shaders::SHADER_TYPES::VERTEX_SHADER];

		int pos = 0;
		for (auto & buffer : vertexShaderCSB)
		{
			buffer.GenerateBuffer(renderer, Resources::Shaders::SHADER_TYPES::VERTEX_SHADER);
			pos++;
		}

		pos = 0;
		for (auto & buffer : pixelShaderCSB)
		{
			buffer.GenerateBuffer(renderer, Resources::Shaders::SHADER_TYPES::PIXEL_SHADER);
			pos++;
		}

		pos = 0;
		for (auto & buffer : pixelVertexShaderCSB)
		{
			//for now they are in the same ps/vs slot, change if needed 
			buffer.GenerateBuffer(renderer, (unsigned int)(Resources::Shaders::SHADER_TYPES::PIXEL_SHADER | Resources::Shaders::SHADER_TYPES::VERTEX_SHADER));
			pos++;
		}
	}

	void ShaderSet::BindConstantBuffer(DirectxRenderer * renderer)
	{
		assert(renderer);

		std::vector<ConstantBuffer> & vertexShaderCSB = constantBuffers[Resources::Shaders::SHADER_TYPES::VERTEX_SHADER];
		std::vector<ConstantBuffer> & pixelShaderCSB = constantBuffers[Resources::Shaders::SHADER_TYPES::PIXEL_SHADER];
		std::vector<ConstantBuffer> & pixelVertexShaderCSB = constantBuffers[Resources::Shaders::SHADER_TYPES::PIXEL_SHADER | Resources::Shaders::SHADER_TYPES::VERTEX_SHADER];

		int pos = 0;
		for (auto & buffer : vertexShaderCSB)
		{
			buffer.SetBuffer(pos, 0, renderer, Resources::Shaders::SHADER_TYPES::VERTEX_SHADER);
			pos++;
		}

		pos = 0;
		for (auto & buffer : pixelShaderCSB)
		{
			buffer.SetBuffer(0, pos, renderer, Resources::Shaders::SHADER_TYPES::PIXEL_SHADER);
			pos++;
		}

		pos = 0;
		for (auto & buffer : pixelVertexShaderCSB)
		{
			//for now they are in the same ps/vs slot, change if needed 
			buffer.SetBuffer(pos, pos, renderer, (unsigned int)(Resources::Shaders::SHADER_TYPES::PIXEL_SHADER | Resources::Shaders::SHADER_TYPES::VERTEX_SHADER));
			pos++;
		}
	}

	std::vector<ConstantBuffer> & ShaderSet::GetBuffersForShader(Resources::Shaders::SHADER_TYPES shaderToBind)
	{
		return constantBuffers[shaderToBind];
	}

	void ShaderSet::AddConstantBuffer(Resources::Shaders::SHADER_TYPES target, const ConstantBuffer & constantBuffer)
	{
		constantBuffers[target].push_back(constantBuffer);
	}

	void ShaderSet::Unload()
	{
		if (vertexShader)
			vertexShader->Release();

		if (pixelShader)
			pixelShader->Release();

		if (inputLayout)
			inputLayout->Release();

		std::vector<ConstantBuffer> vertexShaderCSB = constantBuffers[Resources::Shaders::SHADER_TYPES::VERTEX_SHADER];
		std::vector<ConstantBuffer> pixelShaderCSB = constantBuffers[Resources::Shaders::SHADER_TYPES::PIXEL_SHADER];
		std::vector<ConstantBuffer> pixelVertexShaderCSB = constantBuffers[Resources::Shaders::SHADER_TYPES::PIXEL_SHADER | Resources::Shaders::SHADER_TYPES::VERTEX_SHADER];

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

	ShaderSet::~ShaderSet()
	{
	}
}