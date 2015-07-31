#include "Texture2d.h"
#include <assert.h>
#include "../Graph/DirectxRenderer.h"
#include <tchar.h>

namespace Resources
{
	Texture2d::Texture2d(const std::string & filename) : filename(filename)
	{}

	Texture2d::Texture2d()
	{}


	bool Texture2d::Load(Graph::DirectxRenderer * renderer)
	{
		assert(renderer);

		if (needsReload)
		{
			if (texture)
			{
				texture->Release();
				texture = nullptr;
			}

			if (shaderResource)
			{
				shaderResource->Release();
				shaderResource = nullptr;
			}



			DWORD currentDirectorySize = GetCurrentDirectory(0, nullptr);

			TCHAR * texturesPath = "\\Data\\Textures\\";

			int currentDirStringLenght = currentDirectorySize + _tcslen(texturesPath);

			TCHAR * currentDirectoryBuffer = new TCHAR[currentDirStringLenght];

			GetCurrentDirectory(currentDirectorySize, currentDirectoryBuffer);

			_tcsncat_s(currentDirectoryBuffer, currentDirStringLenght, texturesPath, _tcslen(texturesPath));

			std::string texturesFilePath = std::string(currentDirectoryBuffer);


			needsReload = !renderer->CreateTextureResources(texturesFilePath + filename, &texture, &shaderResource);
		}

		return false;
	}


	void Texture2d::UnLoad()
	{
		if (texture)
		{
			texture->Release();
			texture = nullptr;
		}

		if (shaderResource)
		{
			shaderResource->Release();
			shaderResource = nullptr;
		}
	}

	Texture2d::~Texture2d()
	{
		UnLoad();
	}
}