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

		DUOLGraphicsLibrary::Buffer* _streamOutBuffer;

		DUOLGraphicsLibrary::Buffer* _postProcessingRectVertex;

		DUOLGraphicsLibrary::Buffer* _postProcessingRectIndex;

		DUOLGraphicsLibrary::Buffer* _axisVertex;

		DUOLGraphicsLibrary::Buffer* _axisIndex;

		std::vector<RenderObject> _renderQueue;

		std::vector<RenderObject> _renderDebugQueue;

		char _buffer[4096];
		//렌더링 파이프라인 Resources slot
		DUOLGraphicsLibrary::ResourceViewLayout _currentBindSamplers;

		DUOLGraphicsLibrary::ResourceViewLayout _currentBindTextures;

		DUOLGraphicsLibrary::ResourceViewLayout _currentBindBuffer;

	public:
		void CreateAxis(DUOLGraphicsLibrary::Renderer* renderer);

		void CreateStreamOutBuffer(DUOLGraphicsLibrary::Renderer* renderer);

		void ReserveResourceLayout();

		void ExecuteRenderingPipeline(RenderingPipeline* renderPipeline
			, const ConstantBufferPerFrame& perFrameInfo);

		void OnResize(const DUOLMath::Vector2& resolution);

		void Render(const RenderObject& object);

		void RenderDebug(const RenderObject& object);

		void Present();

		void ExecuteDebugRenderPass(
			RenderingPipeline* renderPipeline
			, const ConstantBufferPerFrame& perFrameInfo);

		void ExecuteDebugRenderTargetPass(
			RenderingPipeline* renderPipeline
			, const ConstantBufferPerFrame& perFrameInfo);
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
