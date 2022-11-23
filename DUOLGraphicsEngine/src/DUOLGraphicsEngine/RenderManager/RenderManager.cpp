#include "RenderManager.h"
#include "RenderingPipeline.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/Material.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/Mesh.h"
#include "DUOLGraphicsLibrary/Renderer/ResourceViewLayout.h"
#include "DUOLGraphicsLibrary/Renderer/Renderer.h"
#include "DUOLGraphicsEngine/Util/Hash/Hash.h"

DUOLGraphicsEngine::RenderManager::RenderManager(DUOLGraphicsLibrary::Renderer* renderer,
	DUOLGraphicsLibrary::RenderContext* context) :
	_renderer(renderer)
	, _context(context)
{
	DUOLGraphicsLibrary::CommandBufferDesc commandBufferDesc;

	_commandBuffer = _renderer->CreateCommandBuffer(0, commandBufferDesc);
	_renderQueue.reserve(60);

	CreatePostProcessingRect();
}

void DUOLGraphicsEngine::RenderManager::ExecuteRenderingPipeline(RenderingPipeline* renderPipeline, const ConstantBufferPerFrame& perFrameInfo)
{
	switch (renderPipeline->GetPipelineType())
	{
	case PipelineType::Render:
	{
		ExecuteRenderPass(renderPipeline, perFrameInfo);
		break;
	}
	case PipelineType::PostProcessing:
	{
		ExecutePostProcessingPass(renderPipeline, perFrameInfo);
		break;
	}
	default:;
	}

	_commandBuffer->Flush();
}

void DUOLGraphicsEngine::RenderManager::ExecuteRenderPass(RenderingPipeline* renderPipeline, const ConstantBufferPerFrame& perFrameInfo)
{
	_commandBuffer->SetRenderPass(renderPipeline->GetRenderPass());

	ConstantBufferPerFrame test = perFrameInfo;
	test._lightCount = 1;

	_commandBuffer->UpdateBuffer(renderPipeline->GetPerFrameBuffer(), 0, &test, sizeof(perFrameInfo));

	uint32_t renderQueueSize = _renderQueue.size();

	for (uint32_t renderIndex = 0; renderIndex < renderQueueSize; renderIndex++)
	{
		RenderObject& renderObject = _renderQueue[renderIndex];

		for (unsigned int submeshIndex = 0; submeshIndex < renderObject.PerObjectData._material->size(); submeshIndex++)
		{
			_commandBuffer->SetPipelineState(renderObject.PerObjectData._material->at(submeshIndex)->_shaders);

			_commandBuffer->SetVertexBuffer(renderObject.mesh->_subMesh[submeshIndex]._vertexBuffer);
			_commandBuffer->SetIndexBuffer(renderObject.mesh->_subMesh[submeshIndex]._indexBuffer);

			memcpy(_buffer, renderObject.PerObjectData._transform, sizeof(Transform));
			memcpy(_buffer + sizeof(Transform), renderObject.PerObjectData._material->at(submeshIndex), 48);

			_commandBuffer->UpdateBuffer(renderPipeline->GetPerObjectBuffer(), 0, _buffer, sizeof(Transform)+48);

			renderPipeline->ChangeTexture(renderObject.PerObjectData._material->at(submeshIndex)->_albedoMap, 0);
			renderPipeline->ChangeTexture(renderObject.PerObjectData._material->at(submeshIndex)->_normalMap, 1);
			renderPipeline->ChangeTexture(renderObject.PerObjectData._material->at(submeshIndex)->_metalicSmoothnessMap, 2);

			_commandBuffer->SetResources(renderPipeline->GetResourceViewLayout());

			//_commandBuffer->DrawIndexed(GetNumIndicesFromBuffer(renderObject.mesh->_subMesh[submeshIndex]._indexBuffer), 0, 0);
			_commandBuffer->DrawIndexed(renderObject.mesh->_subMesh[submeshIndex]._drawIndex, 0, 0);
		}
	}
}

void DUOLGraphicsEngine::RenderManager::ExecuteDebugRenderPass(RenderingPipeline* renderPipeline,
	const ConstantBufferPerFrame& perFrameInfo)
{
	_commandBuffer->SetRenderPass(renderPipeline->GetRenderPass());

	ConstantBufferPerFrame test = perFrameInfo;
	test._lightCount = 1;

	_commandBuffer->UpdateBuffer(renderPipeline->GetPerFrameBuffer(), 0, &test, sizeof(perFrameInfo));

	uint32_t renderQueueSize = _renderDebugQueue.size();

	for (uint32_t renderIndex = 0; renderIndex < renderQueueSize; renderIndex++)
	{
		RenderObject& renderObject = _renderDebugQueue[renderIndex];

		for (unsigned int submeshIndex = 0; submeshIndex < renderObject.PerObjectData._material->size(); submeshIndex++)
		{
			_commandBuffer->SetPipelineState(renderObject.PerObjectData._material->at(submeshIndex)->_shaders);

			_commandBuffer->SetVertexBuffer(renderObject.mesh->_subMesh[submeshIndex]._vertexBuffer);
			_commandBuffer->SetIndexBuffer(renderObject.mesh->_subMesh[submeshIndex]._indexBuffer);

			memcpy(_buffer, renderObject.PerObjectData._transform, sizeof(Transform));
			memcpy(_buffer + sizeof(Transform), renderObject.PerObjectData._material->at(submeshIndex), 48);

			_commandBuffer->UpdateBuffer(renderPipeline->GetPerObjectBuffer(), 0, _buffer, sizeof(Transform) + 48);
			_commandBuffer->SetResources(renderPipeline->GetResourceViewLayout());

			//_commandBuffer->DrawIndexed(GetNumIndicesFromBuffer(renderObject.mesh->_subMesh[submeshIndex]._indexBuffer), 0, 0);
			_commandBuffer->DrawIndexed(renderObject.mesh->_subMesh[submeshIndex]._drawIndex, 0, 0);
		}
	}
}

void DUOLGraphicsEngine::RenderManager::ExecutePostProcessingPass(RenderingPipeline* renderPipeline,
                                                                  const ConstantBufferPerFrame& perFrameInfo)
{
	_commandBuffer->SetRenderPass(renderPipeline->GetRenderPass());

	_commandBuffer->SetPipelineState(renderPipeline->GetPipelineState());

	_commandBuffer->SetVertexBuffer(_postProcessingRectVertex);
	_commandBuffer->SetIndexBuffer(_postProcessingRectIndex);

	_commandBuffer->SetResources(renderPipeline->GetResourceViewLayout());

	_commandBuffer->DrawIndexed(GetNumIndicesFromBuffer(_postProcessingRectIndex), 0, 0);
}

void DUOLGraphicsEngine::RenderManager::OnResize(const DUOLMath::Vector2& resolution)
{
	_commandBuffer->SetViewport(resolution);
}

void DUOLGraphicsEngine::RenderManager::Render(const RenderObject& object)
{
	_renderQueue.emplace_back(object);
}

void DUOLGraphicsEngine::RenderManager::RenderDebug(const RenderObject& object)
{
	_renderDebugQueue.emplace_back(object);
}

void DUOLGraphicsEngine::RenderManager::Present()
{
	_context->Present();

	_renderQueue.clear();
	_renderDebugQueue.clear();
}

int DUOLGraphicsEngine::RenderManager::GetNumIndicesFromBuffer(DUOLGraphicsLibrary::Buffer* indexBuffer)
{
	auto bufferDesc = indexBuffer->GetBufferDesc();

	int ret = bufferDesc._size / bufferDesc._stride;

	return ret;
}

void DUOLGraphicsEngine::RenderManager::CreatePostProcessingRect()
{
	//TODO:: 나중에는 RESOUCEMANAGER를 경유할것

	struct Rect
	{
		DUOLMath::Vector3 _position;
		DUOLMath::Vector2 _texcoord;
	};

	Rect rect[4];

	rect[0]._position = DUOLMath::Vector3{ -1.f, +1.f, +0.f };
	rect[1]._position = DUOLMath::Vector3{ +1.f, +1.f, +0.f };
	rect[2]._position = DUOLMath::Vector3{ -1.f, -1.f, +0.f };
	rect[3]._position = DUOLMath::Vector3{ +1.f, -1.f, +0.f };

	rect[0]._texcoord = DUOLMath::Vector2{ +0.f, +0.f };
	rect[1]._texcoord = DUOLMath::Vector2{ +1.f, +0.f };
	rect[2]._texcoord = DUOLMath::Vector2{ +0.f, +1.f };
	rect[3]._texcoord = DUOLMath::Vector2{ +1.f, +1.f };

	DUOLGraphicsLibrary::BufferDesc vertexbufferDesc;
	vertexbufferDesc._usage = DUOLGraphicsLibrary::ResourceUsage::USAGE_DEFAULT;
	vertexbufferDesc._stride = sizeof(Rect);
	vertexbufferDesc._size = vertexbufferDesc._stride * 4;
	vertexbufferDesc._format = DUOLGraphicsLibrary::ResourceFormat::FORMAT_UNKNOWN;
	vertexbufferDesc._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::VERTEXBUFFER);

	_postProcessingRectVertex = _renderer->CreateBuffer(Hash::Hash64(_T("postProcessingRectVertex")), vertexbufferDesc, static_cast<void*>(rect));

	UINT32 indices[6] = {0,3,2, 0,1,3};

	DUOLGraphicsLibrary::BufferDesc indexbufferDesc;

	indexbufferDesc._usage = DUOLGraphicsLibrary::ResourceUsage::USAGE_DEFAULT;
	indexbufferDesc._stride = sizeof(unsigned int);
	indexbufferDesc._size = indexbufferDesc._stride * 6;
	indexbufferDesc._format = DUOLGraphicsLibrary::ResourceFormat::FORMAT_R32_UINT;
	indexbufferDesc._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::INDEXBUFFER);

	_postProcessingRectIndex = _renderer->CreateBuffer(Hash::Hash64(_T("postProcessingRectIndex")), indexbufferDesc, static_cast<void*>(indices));
}
