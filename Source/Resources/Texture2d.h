#pragma  once
#include <string>
#include "../Graph/Directx.h"

namespace Graph
{
	class DirectxRenderer;
}

namespace Resources
{
	class Texture2d
	{
	private:
		std::string filename = ""; //in Data/Textures
		bool needsReload = true;
		ID3D11Resource * texture;
		ID3D11ShaderResourceView * shaderResource;

	public:

		Texture2d(const std::string & filename);
		Texture2d();
		~Texture2d();

		bool Load(Graph::DirectxRenderer * renderer);

		void UnLoad();

		inline void SetFilename(const std::string filename)
		{
			this->filename = filename;
			needsReload = true;
		}

		inline const std::string & GetFilename() const
		{
			return filename;
		}

		inline bool IsLoaded() const 
		{
			return !needsReload;
		}

		inline ID3D11Resource *  GetTexture() const
		{
			return texture;
		}

		inline ID3D11ShaderResourceView * GetShaderResource() const
		{
			return shaderResource;
		}

	};
}