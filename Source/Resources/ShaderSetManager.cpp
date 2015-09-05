#include "ShaderSetManager.h"
#include <assert.h>
#include "../Graph/DirectxRenderer.h"
#include <tchar.h>
#include <fstream>
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

namespace Resources
{

	ShaderSetManager::ShaderSetManager()
	{
		shaders.resize(static_cast<unsigned int>(Shaders::SHADERS_IDX::SIZE));

		DWORD currentDirectorySize = GetCurrentDirectory(0, nullptr);

		TCHAR * shadersPath = "\\Data\\Shaders\\";

		int currentDirStringLenght = currentDirectorySize + _tcslen(shadersPath);

		TCHAR * currentDirectoryBuffer = new TCHAR[currentDirStringLenght];

		GetCurrentDirectory(currentDirectorySize, currentDirectoryBuffer);

		_tcsncat_s(currentDirectoryBuffer, currentDirStringLenght, shadersPath, _tcslen(shadersPath));

		fullPath = std::string(currentDirectoryBuffer);

		delete[] currentDirectoryBuffer;
	}


	void ShaderSetManager::RegisterShaders(Graph::DirectxRenderer * renderer)
	{

		assert(renderer);

		std::unique_ptr<Shaders::ShadersInfo> blinnPhongInfo = GenerateShadersInfoFromFile(fullPath + "Blinn_Phong_Text.sif");

		if (blinnPhongInfo)
		{
			//Load shader set
			std::unique_ptr<Graph::ShaderSet> sSet = renderer->GenerateShaderSetFromFile(*blinnPhongInfo);

			if (!sSet)
			{
				abort();
			}

			//shader input layout setup
			D3D11_INPUT_ELEMENT_DESC ied[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};

			sSet->CreateInputLayout(ied, 4, renderer);

			//set pipeline inputs
			Graph::ConstantBuffer perVertexConstantBuffer(sizeof(Shaders::BLINN_PHONG_TEXTURE::vsConstantBuffer));
			sSet->AddConstantBuffer(Resources::Shaders::SHADER_TYPES::VERTEX_SHADER, perVertexConstantBuffer);

			Graph::ConstantBuffer perFramePSConstantBuffer(sizeof(Shaders::BLINN_PHONG_TEXTURE::PerFramePSConstantBuffer));
			sSet->AddConstantBuffer(Resources::Shaders::SHADER_TYPES::PIXEL_SHADER, perFramePSConstantBuffer);


			Graph::ConstantBuffer perGeometryPSConstantBuffer(sizeof(Shaders::BLINN_PHONG_TEXTURE::PerGeometryPSConstantBuffer));
			sSet->AddConstantBuffer(Resources::Shaders::SHADER_TYPES::PIXEL_SHADER, perGeometryPSConstantBuffer);
			//

			unsigned int idx = static_cast<unsigned int>(Shaders::SHADERS_IDX::BLINN_PHONG_TEXTURE);
			shaders[idx] = std::move(sSet);
		}

		
	}

	std::unique_ptr<Shaders::ShadersInfo> ShaderSetManager::GenerateShadersInfoFromFile(const std::string & filename)
	{

		std::unique_ptr<Shaders::ShadersInfo> info = std::make_unique<Shaders::ShadersInfo>();

		bool success = true;

		std::ifstream fileStream(filename);

		if (fileStream.is_open())
		{
			std::string data;
			fileStream >> data;


			if (data == "stages")
			{
				std::streamsize size = fileStream.rdbuf()->in_avail();

				while (!fileStream.eof())
				{
					fileStream >> data;
					if (data == "v->")
					{
						info->vertexShaderInfo = std::make_unique<Shaders::ShaderTypeInfo>();
						fileStream >> info->vertexShaderInfo->fileName;
						info->vertexShaderInfo ->fileName= fullPath + info->vertexShaderInfo->fileName;

						fileStream >> info->vertexShaderInfo->mainFunctionName;
						fileStream >> info->vertexShaderInfo->profile;
						fileStream >> info->vertexShaderInfo->compilationFlags;
					}

					if (data == "p->")
					{
						info->pixelShaderInfo = std::make_unique<Shaders::ShaderTypeInfo>();
						fileStream >> info->pixelShaderInfo->fileName;
						info->pixelShaderInfo->fileName = fullPath + info->pixelShaderInfo->fileName;

						fileStream >> info->pixelShaderInfo->mainFunctionName;
						fileStream >> info->pixelShaderInfo->profile;
						fileStream >> info->pixelShaderInfo->compilationFlags;
					}
					data = "";

				}

				if (!info->pixelShaderInfo || !info->vertexShaderInfo)
				{
					success = false;
				}

			}
			else
			{
				success = false;
			}
		}

		fileStream.close();

		if (success)
		{
			return info;
		}
		else
		{
			return nullptr;
		}
	}

	void ShaderSetManager::Load(Graph::DirectxRenderer * renderer)
	{
		assert(renderer);

		RegisterShaders(renderer);
	}

	void ShaderSetManager::Unload()
	{
		for (auto & shaderSet : shaders)
		{
			shaderSet->Unload();
		}
	}

	void ShaderSetManager::Init(Graph::DirectxRenderer * renderer)
	{
		assert(renderer);

		Load(renderer);
	}

	void ShaderSetManager::DeInit()
	{
		Unload();
	}

	Graph::ShaderSet * ShaderSetManager::Get(Shaders::SHADERS_IDX idx)
	{
		return shaders[static_cast<unsigned int>(idx)].get();
	}

	ShaderSetManager::~ShaderSetManager()
	{
	}
}