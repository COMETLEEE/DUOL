#pragma once
#include <unordered_map>
#include "DUOLGraphicsEngine/ResourceManager/Resource/RenderObject.h"
#include "DUOLGraphicsLibrary/PipelineStateFlags.h"
#include "DUOLGraphicsLibrary/Renderer/Renderer.h"

namespace DUOLGraphicsLibrary
{
	class Buffer;
	class CommandBuffer;
	class RenderContext;
	class Renderer;
	class ResourceViewLayout;
}

namespace DUOLGraphicsEngine
{
	class RenderingPipeline;

	class RenderManager
	{
	public:
		RenderManager(DUOLGraphicsLibrary::Renderer* renderer, DUOLGraphicsLibrary::RenderContext* context);

	private:
		DUOLGraphicsLibrary::Renderer* _renderer;

		DUOLGraphicsLibrary::RenderContext* _context;

		DUOLGraphicsLibrary::CommandBuffer* _commandBuffer;

		DUOLGraphicsLibrary::Buffer* _postProcessingRectVertex;

		DUOLGraphicsLibrary::Buffer* _postProcessingRectIndex;

		std::vector<RenderObject> _renderQueue;

	public:
		void ExecuteRenderingPipeline(RenderingPipeline* renderPipeline
			, const ConstantBufferPerFrame& perFrameInfo);

		void OnResize(const DUOLMath::Vector2& resolution);

		void Render(const RenderObject& object);

		void Present();

	private:
		int GetNumIndicesFromBuffer(DUOLGraphicsLibrary::Buffer* indexBuffer);

		void CreatePostProcessingRect();

		void ExecuteRenderPass(
			RenderingPipeline* renderPipeline
			, const ConstantBufferPerFrame& perFrameInfo);

		void ExecutePostProcessingPass(
			RenderingPipeline* renderPipeline
			, const ConstantBufferPerFrame& perFrameInfo);

	};
}
