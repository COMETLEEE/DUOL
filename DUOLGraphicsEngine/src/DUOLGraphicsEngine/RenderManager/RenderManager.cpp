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
	ReserveResourceLayout();
	CreateAxis(renderer);
}

void DUOLGraphicsEngine::RenderManager::CreateAxis(DUOLGraphicsLibrary::Renderer* renderer)
{
	struct DebugVertex
	{
		DUOLMath::Vector3 _position;
		int _color;
	};

	DebugVertex vertice[6];

	vertice[0]._position = DUOLMath::Vector3{0.f, 0.f, 0.f};
	vertice[0]._color = 0xff000000;
	vertice[1]._position = DUOLMath::Vector3{100.f, 0.f, 0.f};
	vertice[1]._color = 0xff000000;
	vertice[2]._position = DUOLMath::Vector3{ 0.f, 0.f, 0.f };
	vertice[2]._color = 0x00ff0000;
	vertice[3]._position = DUOLMath::Vector3{0.f, 100.f, 0.f};
	vertice[3]._color = 0x00ff0000;
	vertice[4]._position = DUOLMath::Vector3{ 0.f, 0.f, 0.f };
	vertice[4]._color = 0x0000ff00;
	vertice[5]._position = DUOLMath::Vector3{0.f, 0.f, 100.f};
	vertice[5]._color = 0x0000ff00;

	UINT indice[6] = {0, 1, 2, 3, 4, 5};

	DUOLGraphicsLibrary::BufferDesc vetexBufferDesc;

	vetexBufferDesc._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::VERTEXBUFFER);
	vetexBufferDesc._usage = DUOLGraphicsLibrary::ResourceUsage::USAGE_DYNAMIC;
	vetexBufferDesc._stride = sizeof(DebugVertex);
	vetexBufferDesc._size = vetexBufferDesc._stride * 6;
	vetexBufferDesc._cpuAccessFlags = static_cast<long>(DUOLGraphicsLibrary::CPUAccessFlags::WRITE);

	DUOLGraphicsLibrary::BufferDesc indexBufferDesc;

	indexBufferDesc._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::INDEXBUFFER);
	indexBufferDesc._usage = DUOLGraphicsLibrary::ResourceUsage::USAGE_DYNAMIC;
	indexBufferDesc._stride = sizeof(unsigned int);
	indexBufferDesc._size = indexBufferDesc._stride * 6;
	indexBufferDesc._format = DUOLGraphicsLibrary::ResourceFormat::FORMAT_R32_UINT;
	indexBufferDesc._cpuAccessFlags = static_cast<long>(DUOLGraphicsLibrary::CPUAccessFlags::WRITE);

	_axisVertex = renderer->CreateBuffer(Hash::Hash64(_T("AxisVertex")), vetexBufferDesc, vertice);
	_axisIndex = renderer->CreateBuffer(Hash::Hash64(_T("Axisindex")), indexBufferDesc, indice);
}

void DUOLGraphicsEngine::RenderManager::ReserveResourceLayout()
{
	_currentBindBuffer._resourceViews.reserve(2);

	_currentBindSamplers._resourceViews.reserve(1);
	DUOLGraphicsLibrary::SamplerDesc samplerDesc;
	_currentBindSamplers._resourceViews.emplace_back(_renderer->CreateSampler(1, samplerDesc), 0, static_cast<long>(DUOLGraphicsLibrary::BindFlags::SAMPLER), static_cast<long>(DUOLGraphicsLibrary::StageFlags::VSPS));

	_currentBindTextures._resourceViews.reserve(8);
	_currentBindTextures._resourceViews.emplace_back(nullptr, 0, static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE), static_cast<long>(DUOLGraphicsLibrary::StageFlags::VSPS));
	_currentBindTextures._resourceViews.emplace_back(nullptr, 1, static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE), static_cast<long>(DUOLGraphicsLibrary::StageFlags::VSPS));
	_currentBindTextures._resourceViews.emplace_back(nullptr, 2, static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE), static_cast<long>(DUOLGraphicsLibrary::StageFlags::VSPS));
	_currentBindTextures._resourceViews.emplace_back(nullptr, 3, static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE), static_cast<long>(DUOLGraphicsLibrary::StageFlags::VSPS));
	_currentBindTextures._resourceViews.emplace_back(nullptr, 4, static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE), static_cast<long>(DUOLGraphicsLibrary::StageFlags::VSPS));
	_currentBindTextures._resourceViews.emplace_back(nullptr, 5, static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE), static_cast<long>(DUOLGraphicsLibrary::StageFlags::VSPS));
	_currentBindTextures._resourceViews.emplace_back(nullptr, 6, static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE), static_cast<long>(DUOLGraphicsLibrary::StageFlags::VSPS));
	_currentBindTextures._resourceViews.emplace_back(nullptr, 7, static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE), static_cast<long>(DUOLGraphicsLibrary::StageFlags::VSPS));

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
	test._light[0]._direction = DUOLMath::Vector3{ 0.0f, 0.0f , -1.f };
	test._light[0]._direction.Normalize();

	_commandBuffer->UpdateBuffer(renderPipeline->GetPerFrameBuffer(), 0, &test, sizeof(perFrameInfo));

	const size_t renderQueueSize = _renderQueue.size();

	for (uint32_t renderIndex = 0; renderIndex < renderQueueSize; renderIndex++)
	{
		RenderObject& renderObject = _renderQueue[renderIndex];

		renderObject._renderInfo->BindPipeline(_buffer);
		int renderObjecttBufferSize = renderObject._renderInfo->GetInfoStructureSize();

		for (unsigned int submeshIndex = 0; submeshIndex < renderObject._materials->size(); submeshIndex++)
		{
			_commandBuffer->SetPipelineState(renderObject._materials->at(submeshIndex)->GetPipelineState());

			_commandBuffer->SetVertexBuffer(renderObject._mesh->_subMesh[submeshIndex]._vertexBuffer);
			_commandBuffer->SetIndexBuffer(renderObject._mesh->_subMesh[submeshIndex]._indexBuffer);

			renderObject._materials->at(submeshIndex)->BindPipeline(_buffer + renderObjecttBufferSize, &_currentBindTextures);

			_commandBuffer->UpdateBuffer(renderPipeline->GetPerObjectBuffer(), 0, _buffer, renderObjecttBufferSize + 48);
	
			_commandBuffer->SetResources(_currentBindSamplers);
			_commandBuffer->SetResources(_currentBindBuffer);
			_commandBuffer->SetResources(_currentBindTextures);

			_commandBuffer->DrawIndexed(renderObject._mesh->_subMesh[submeshIndex]._drawIndex, 0, 0);
		}
	}
}

void DUOLGraphicsEngine::RenderManager::ExecuteDebugRenderPass(RenderingPipeline* renderPipeline,
	const ConstantBufferPerFrame& perFrameInfo)
{
	_commandBuffer->SetRenderPass(renderPipeline->GetRenderPass());

	const size_t renderQueueSize = _renderDebugQueue.size();

	for (uint32_t renderIndex = 0; renderIndex < renderQueueSize; renderIndex++)
	{
		RenderObject& renderObject = _renderDebugQueue[renderIndex];

		renderObject._renderInfo->BindPipeline(_buffer);
		for (unsigned int submeshIndex = 0; submeshIndex < renderObject._materials->size(); submeshIndex++)
		{
			_commandBuffer->SetPipelineState(renderObject._materials->at(submeshIndex)->GetPipelineState());

			_commandBuffer->SetVertexBuffer(renderObject._mesh->_subMesh[submeshIndex]._vertexBuffer);
			_commandBuffer->SetIndexBuffer(renderObject._mesh->_subMesh[submeshIndex]._indexBuffer);

			memcpy(_buffer + renderObject._renderInfo->GetInfoStructureSize(), renderObject._materials->at(submeshIndex), 48);

			_commandBuffer->UpdateBuffer(renderPipeline->GetPerObjectBuffer(), 0, _buffer, sizeof(Transform) + 48);
			_commandBuffer->SetResources(renderPipeline->GetResourceViewLayout());

			_commandBuffer->DrawIndexed(renderObject._mesh->_subMesh[submeshIndex]._drawIndex, 0, 0);
		}
	}

	{
		_commandBuffer->SetVertexBuffer(_axisVertex);
		_commandBuffer->SetIndexBuffer(_axisIndex);

		Transform transform;

		memcpy(_buffer, &transform, sizeof(Transform));

		_commandBuffer->UpdateBuffer(renderPipeline->GetPerObjectBuffer(), 0, _buffer, sizeof(Transform) + 48);
		_commandBuffer->SetResources(renderPipeline->GetResourceViewLayout());

		_commandBuffer->DrawIndexed(6, 0, 0);
	}

	_commandBuffer->Flush();
}

void DUOLGraphicsEngine::RenderManager::ExecuteDebugRenderTargetPass(RenderingPipeline* renderPipeline,
	const ConstantBufferPerFrame& perFrameInfo)
{
	_commandBuffer->SetRenderPass(renderPipeline->GetRenderPass());

	_commandBuffer->SetPipelineState(renderPipeline->GetPipelineState());

	_commandBuffer->SetVertexBuffer(_postProcessingRectVertex);
	_commandBuffer->SetIndexBuffer(_postProcessingRectIndex);

	_commandBuffer->SetResources(renderPipeline->GetResourceViewLayout());

	for (int renderTargets = 0; renderTargets < 4; renderTargets++)
	{
		_commandBuffer->UpdateBuffer(renderPipeline->GetPerObjectBuffer(), 0, &renderTargets, sizeof(renderTargets));

		_commandBuffer->DrawIndexed(GetNumIndicesFromBuffer(_postProcessingRectIndex), 0, 0);
	}

	_commandBuffer->Flush();
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

	UINT32 indices[6] = { 0,3,2, 0,1,3 };

	DUOLGraphicsLibrary::BufferDesc indexbufferDesc;

	indexbufferDesc._usage = DUOLGraphicsLibrary::ResourceUsage::USAGE_DEFAULT;
	indexbufferDesc._stride = sizeof(unsigned int);
	indexbufferDesc._size = indexbufferDesc._stride * 6;
	indexbufferDesc._format = DUOLGraphicsLibrary::ResourceFormat::FORMAT_R32_UINT;
	indexbufferDesc._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::INDEXBUFFER);

	_postProcessingRectIndex = _renderer->CreateBuffer(Hash::Hash64(_T("postProcessingRectIndex")), indexbufferDesc, static_cast<void*>(indices));
}
