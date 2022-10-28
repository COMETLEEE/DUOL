#include "RenderManager.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/Mesh.h"
#include "DUOLGraphicsLibrary/Renderer/ResourceViewLayout.h"
#include "DUOLGraphicsLibrary/Renderer/Renderer.h"


void DUOLGraphicsEngine::RenderManager::ExecuteRenderPass(DUOLGraphicsLibrary::RenderPass* renderPass,
	DUOLGraphicsLibrary::ResourceViewLayout* resourceViewLayout, const DUOLGraphicsLibrary::Viewport& viewport)
{
	_commandBuffer->SetRenderPass(renderPass);
	_commandBuffer->SetViewport(viewport);
	_commandBuffer->BindResources(*resourceViewLayout);

	uint32_t renderQueueSize = _renderQueue.size();

	for (uint32_t renderIndex = 0; renderIndex < renderQueueSize; renderIndex++)
	{
		RenderObject renderObject = _renderQueue.front();
		_renderQueue.pop();


		for (unsigned int submeshIndex = 0; submeshIndex < renderObject.mesh->_submeshCount; submeshIndex++)
		{
			_commandBuffer->SetPipelineState(renderObject.mesh->_pipelinestate);



			_commandBuffer->DrawIndexed(GetNumIndicesFromBuffer(renderObject.mesh->_indexBuffer[submeshIndex]), 0, 0);
		}
	}
}

void DUOLGraphicsEngine::RenderManager::ExecutePostProcessingPass(DUOLGraphicsLibrary::RenderPass* renderPass,
	DUOLGraphicsLibrary::ResourceViewLayout* resourceViewLayout, const DUOLGraphicsLibrary::Viewport& viewport)
{
}

void DUOLGraphicsEngine::RenderManager::Present()
{
	_context->Present();

	while (_renderQueue.empty())
	{
		_renderQueue.pop();
	}
}

int DUOLGraphicsEngine::RenderManager::GetNumIndicesFromBuffer(DUOLGraphicsLibrary::Buffer* indexBuffer)
{
	auto bufferDesc = indexBuffer->GetBufferDesc();

	int ret = bufferDesc._size / bufferDesc._stride;

	return ret;
}
