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

		//Todo:: �ٱ����� ������� ���
		DUOLGraphicsLibrary::Buffer* _streamOutBuffer;

		DUOLGraphicsLibrary::PipelineState* _particleShader;

		DUOLGraphicsLibrary::Buffer* _postProcessingRectVertex;

		DUOLGraphicsLibrary::Buffer* _postProcessingRectIndex;

		DUOLGraphicsLibrary::Buffer* _axisVertex;

		DUOLGraphicsLibrary::Buffer* _axisIndex;

		std::vector<RenderObject> _renderQueue;

		std::vector<RenderObject> _renderDebugQueue;

		char _buffer[100000];
		//������ ���������� Resources slot
		DUOLGraphicsLibrary::ResourceViewLayout _currentBindSamplers;

		DUOLGraphicsLibrary::ResourceViewLayout _currentBindTextures;

		DUOLGraphicsLibrary::ResourceViewLayout _currentBindBuffer;

	public:
		//Todo ���������� �ȵǴ� �Լ���
		void CreateAxis(DUOLGraphicsLibrary::Renderer* renderer);

		void SetParticleShader(DUOLGraphicsLibrary::PipelineState* pipelineState);
		//
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

		void RenderMesh(RenderObject& renderObject, RenderingPipeline* renderPipeline);

		void RenderParticle(RenderObject& renderObject, RenderingPipeline* renderPipeline);

	};
}
