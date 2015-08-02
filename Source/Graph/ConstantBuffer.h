#pragma once
#include "Graph.h"

struct ID3D11Buffer;

namespace Graph
{
	class DirectxRenderer;
	class ShaderSet;

	class ConstantBuffer
	{
	public:
		ConstantBuffer(size_t size);
		void GenerateBuffer(Graph::DirectxRenderer * renderer, unsigned int bindTarget);
		void GenerateAndBindBuffer(unsigned int vsSlot, unsigned int psSlot, Graph::DirectxRenderer * renderer, unsigned int target);
		void UpdateBuffer(const void * data, Graph::DirectxRenderer * renderer);
		void SetBuffer(unsigned int vsSlot, unsigned int psSlot, Graph::DirectxRenderer * renderer, unsigned int bindTarget);
		void ReleaseConstantBuffer();
		inline ID3D11Buffer * GetBuffer()
		{
			return constantBuffer;
		}
	private:
		ID3D11Buffer * constantBuffer;
		size_t bufferSize = -1;
	};
}
