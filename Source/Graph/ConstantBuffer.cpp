#include "DirectxRenderer.h"
#include <assert.h>
#include "Directx.h"
#include "ConstantBuffer.h"

namespace Graph
{

	ConstantBuffer::ConstantBuffer(size_t size) : bufferSize(size), constantBuffer(nullptr)
	{

	}

	void ConstantBuffer::GenerateBuffer(Graph::DirectxRenderer * renderer, unsigned int bindTarget)
	{
		assert(renderer);

		constantBuffer = renderer->GenerateConstantBuffer(bufferSize);

		assert(constantBuffer);
	}

	void ConstantBuffer::GenerateAndBindBuffer(unsigned int vsSlot, unsigned int psSlot, Graph::DirectxRenderer * renderer, unsigned int bindTarget)
	{
		assert(renderer);

		constantBuffer = renderer->GenerateConstantBuffer(bufferSize);

		assert(constantBuffer);

		renderer->BindConstantBufferToShaderStage(vsSlot, psSlot, constantBuffer, bindTarget);
	}

	void ConstantBuffer::UpdateBuffer(const void * data, Graph::DirectxRenderer * renderer)
	{
		renderer->UpdateSubresource(constantBuffer, data);
	}

	void ConstantBuffer::SetBuffer(unsigned int vsSlot, unsigned int psSlot, Graph::DirectxRenderer * renderer, unsigned int bindTarget)
	{
		assert(renderer);
		assert(constantBuffer);

		renderer->BindConstantBufferToShaderStage(vsSlot, psSlot, constantBuffer, bindTarget);
	}

	void ConstantBuffer::ReleaseConstantBuffer()
	{
		assert(constantBuffer);

		constantBuffer->Release();
	}

}
