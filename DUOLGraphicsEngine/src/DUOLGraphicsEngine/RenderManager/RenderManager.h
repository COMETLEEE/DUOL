#pragma once
#include <unordered_map>

#include "DUOLGraphicsEngine/ResourceManager/Resource/RenderObject.h"
#include "DUOLGraphicsLibrary/PipelineStateFlags.h"
#include "DUOLGraphicsLibrary/Renderer/Renderer.h"

namespace DUOLGraphicsLibrary
{
	class Buffer;
	class RenderPass;
	class CommandBuffer;
	class RenderContext;
	class Renderer;
	class ResourceViewLayout;
}

namespace DUOLGraphicsEngine
{
	class RenderPipieline;

	class RenderManager
	{
	public:
		RenderManager(DUOLGraphicsLibrary::Renderer* renderer, DUOLGraphicsLibrary::RenderContext* context) :
			_renderer(renderer)
			,_context(context)
		{
			DUOLGraphicsLibrary::CommandBufferDesc commandBufferDesc;

			_commandBuffer = _renderer->CreateCommandBuffer(0, commandBufferDesc);

			_renderQueue.reserve(60);
		}

	private:
		DUOLGraphicsLibrary::Renderer* _renderer;

		DUOLGraphicsLibrary::RenderContext* _context;

		DUOLGraphicsLibrary::CommandBuffer* _commandBuffer;

		std::vector<RenderObject> _renderQueue;

	public:

		void ExecuteRenderPass(
			RenderPipieline* renderPipeline
			, const ConstantBufferPerFrame& perFrameInfo);

		void ExecutePostProcessingPass(
			DUOLGraphicsLibrary::RenderPass* renderPass
			//, DUOLGraphicsLibrary::PipelineState* pipeline
			, DUOLGraphicsLibrary::ResourceViewLayout* resourceViewLayout
			, const DUOLGraphicsLibrary::Viewport& viewport);

		void OnResize(const DUOLMath::Vector2& resolution);

		void Render(const RenderObject& object);

		void Present();

	private:
		int GetNumIndicesFromBuffer(DUOLGraphicsLibrary::Buffer* indexBuffer);

	};
}
