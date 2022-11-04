#pragma once
#include <unordered_map>
#include <queue>

#include "DUOLGraphicsEngine/ResourceManager/Resource/RenderObject.h"
#include "DUOLGraphicsLibrary/PipelineStateFlags.h"

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

	class RenderManager
	{
	public:
		RenderManager(DUOLGraphicsLibrary::Renderer* renderer) :
			_renderer(renderer)
		{

		}

	private:
		DUOLGraphicsLibrary::Renderer* _renderer;

		DUOLGraphicsLibrary::RenderContext* _context;

		DUOLGraphicsLibrary::CommandBuffer* _commandBuffer;

		std::queue<RenderObject> _renderQueue;

	public:
		void ExecuteRenderPass(
			DUOLGraphicsLibrary::RenderPass* renderPass
			//, DUOLGraphicsLibrary::PipelineState* pipeline
			, DUOLGraphicsLibrary::ResourceViewLayout* resourceViewLayout
			, const DUOLGraphicsLibrary::Viewport& viewport);

		void ExecutePostProcessingPass(
			DUOLGraphicsLibrary::RenderPass* renderPass
			//, DUOLGraphicsLibrary::PipelineState* pipeline
			, DUOLGraphicsLibrary::ResourceViewLayout* resourceViewLayout
			, const DUOLGraphicsLibrary::Viewport& viewport);

		void Present();

	private:
		int GetNumIndicesFromBuffer(DUOLGraphicsLibrary::Buffer* indexBuffer);

	};
}
