#include "RenderManager.h"

#include "RenderPipieline.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/Material.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/Mesh.h"
#include "DUOLGraphicsLibrary/Renderer/ResourceViewLayout.h"
#include "DUOLGraphicsLibrary/Renderer/Renderer.h"


void DUOLGraphicsEngine::RenderManager::ExecuteRenderPass(DUOLGraphicsLibrary::RenderPass* renderPass,
	DUOLGraphicsLibrary::ResourceViewLayout* resourceViewLayout, const DUOLGraphicsLibrary::Viewport& viewport)
{
	_commandBuffer->SetRenderPass(renderPass);
	_commandBuffer->SetViewport(viewport);
	_commandBuffer->SetResources(*resourceViewLayout);

	uint32_t renderQueueSize = _renderQueue.size();

	for (uint32_t renderIndex = 0; renderIndex < renderQueueSize; renderIndex++)
	{
		RenderObject& renderObject = _renderQueue[renderIndex];

		for (unsigned int submeshIndex = 0; submeshIndex < renderObject.mesh->_submeshCount; submeshIndex++)
		{
			_commandBuffer->SetPipelineState(renderObject.PerObjectData._material->_shaders);

			//_commandBuffer->DrawIndexed(GetNumIndicesFromBuffer(renderObject.mesh->_indexBuffer[submeshIndex]), 0, 0);
		}
	}
}

void DUOLGraphicsEngine::RenderManager::ExecuteRenderPass(RenderPipieline* renderPipeline,
	const DUOLGraphicsLibrary::Viewport& viewport, const ConstantBufferPerFrame& perFrameInfo)
{
	_commandBuffer->SetRenderPass(renderPipeline->GetRenderPass());
	_commandBuffer->SetViewport(viewport);

	_commandBuffer->UpdateBuffer(renderPipeline->GetPerFrameBuffer(), 0, &perFrameInfo, sizeof(perFrameInfo));

	uint32_t renderQueueSize = _renderQueue.size();

	for (uint32_t renderIndex = 0; renderIndex < renderQueueSize; renderIndex++)
	{
		RenderObject& renderObject = _renderQueue[renderIndex];

		for (unsigned int submeshIndex = 0; submeshIndex < renderObject.mesh->_submeshCount; submeshIndex++)
		{
			//_commandBuffer->SetPipelineState(renderPipeline->GetPipielineState());
			_commandBuffer->SetPipelineState(renderObject.PerObjectData._material->_shaders);

			_commandBuffer->SetVertexBuffer(renderObject.mesh->_subMesh[submeshIndex]._vertexBuffer);
			_commandBuffer->SetIndexBuffer(renderObject.mesh->_subMesh[submeshIndex]._indexBuffer);

			_commandBuffer->UpdateBuffer(renderPipeline->GetPerObjectBuffer(), sizeof(Transfrom), renderObject.PerObjectData._material,24);
			_commandBuffer->UpdateBuffer(renderPipeline->GetPerObjectBuffer(), 0, renderObject.PerObjectData._transform,sizeof(Transfrom));

			renderPipeline->ChangeTexture(renderObject.PerObjectData._material->_albedoMap, 0);
			renderPipeline->ChangeTexture(renderObject.PerObjectData._material->_metalicSmoothnessMap, 1);
			renderPipeline->ChangeTexture(renderObject.PerObjectData._material->_normalMap, 2);

			_commandBuffer->SetResources(renderPipeline->GetResourceViewLayout());

			_commandBuffer->DrawIndexed(GetNumIndicesFromBuffer(renderObject.mesh->_subMesh[submeshIndex]._indexBuffer), 0, 0);
		}
	}
}

void DUOLGraphicsEngine::RenderManager::ExecutePostProcessingPass(DUOLGraphicsLibrary::RenderPass* renderPass,
                                                                  DUOLGraphicsLibrary::ResourceViewLayout* resourceViewLayout, const DUOLGraphicsLibrary::Viewport& viewport)
{
}

void DUOLGraphicsEngine::RenderManager::Render(const RenderObject& object)
{
	_renderQueue.emplace_back(object);
}

void DUOLGraphicsEngine::RenderManager::Present()
{
	_context->Present();

	_renderQueue.clear();
}

int DUOLGraphicsEngine::RenderManager::GetNumIndicesFromBuffer(DUOLGraphicsLibrary::Buffer* indexBuffer)
{
	auto bufferDesc = indexBuffer->GetBufferDesc();

	int ret = bufferDesc._size / bufferDesc._stride;

	return ret;
}
