#include "Texture2d.h"
#include <assert.h>
#include "../Graph/DirectxRenderer.h"


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

			needsReload = !renderer->CreateTextureResources(filename, &texture, &shaderResource);
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