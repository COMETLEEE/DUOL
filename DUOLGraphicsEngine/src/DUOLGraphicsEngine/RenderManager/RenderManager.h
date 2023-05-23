#pragma once
#include <queue>

#include "DUOLGraphicsEngine/ResourceManager/Resource/RenderConstantBuffer.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/RenderObject.h"
#include "DUOLGraphicsEngine/Util/ByteBuffer.h"
#include "DUOLGraphicsLibrary/PipelineStateFlags.h"
#include "DUOLGraphicsLibrary/Renderer/Renderer.h"

namespace DUOLGraphicsLibrary
{
	class IFont;
	class Buffer;
	class CommandBuffer;
	class RenderContext;
	class Renderer;
	class ResourceViewLayout;
}

namespace DUOLGraphicsEngine
{
	class InstancingManager;
	class OrderIndependentTransparencyRenderer;
	class OcclusionCulling;
	class RenderingPipeline;

	class RenderManager
	{
	public:
		RenderManager(DUOLGraphicsLibrary::Renderer* renderer, DUOLGraphicsLibrary::RenderContext* context, DUOLGraphicsLibrary::Buffer* PerFrameBuffer, DUOLGraphicsLibrary::Buffer* PerCameraBuffer, DUOLGraphicsLibrary::Buffer* PerObjectBuffer);

	private:
		DUOLGraphicsLibrary::Renderer* _renderer;

		DUOLGraphicsLibrary::RenderContext* _context;

		DUOLGraphicsLibrary::CommandBuffer* _commandBuffer;

		DUOLGraphicsEngine::OrderIndependentTransparencyRenderer* _oitRenderer;

		DUOLGraphicsEngine::InstancingManager* _instancingManager;

		DUOLGraphicsLibrary::Buffer* _perFrameBuffer;

		DUOLGraphicsLibrary::Buffer* _perCameraBuffer;

		DUOLGraphicsLibrary::Buffer* _perObjectBuffer;

		DUOLGraphicsLibrary::Buffer* _postProcessingRectVertex;

		DUOLGraphicsLibrary::Buffer* _postProcessingRectIndex;

		DUOLGraphicsLibrary::Buffer* _axisVertex;

		DUOLGraphicsLibrary::Buffer* _axisIndex;

		////렌더큐의 1차 정렬을 위함이다.
		//std::priority_queue<UINT32, std::vector<RenderObject*>, std::less<UINT32>> _renderQueue;

		std::unique_ptr<ByteBuffer> _buffer;
		
		//렌더링 파이프라인 Resources slot
		DUOLGraphicsLibrary::ResourceViewLayout _currentBindTextures;

		DUOLGraphicsLibrary::ResourceViewLayout _perFrameBufferBinder;

		DUOLGraphicsLibrary::ResourceViewLayout _perCameraBufferBinder;

		DUOLGraphicsLibrary::ResourceViewLayout _perObjectBufferBinder;

		std::vector<DUOLGraphicsEngine::RenderObject*> _debugRenderQueue;

	public:
		//Todo 여기있으면 안되는 함수들
		void PushDebugObject(RenderObject* render_object);

		void CreateAxis(DUOLGraphicsLibrary::Renderer* renderer);

		void ReserveResourceLayout();

		void ExecuteRenderingPipeline(RenderingPipeline* renderPipeline, const std::vector<DecomposedRenderData>& renderObjects, void* postProcessingData = nullptr, int dataSize = 0);

		void OnResize(const DUOLMath::Vector2& resolution);

		void CopyTexture(DUOLGraphicsLibrary::Texture* destTexture, DUOLGraphicsLibrary::Texture* srcTexture);

		void Present();

		void Begin();

		void ClearState();

		void End();

		void SetOITRenderer(DUOLGraphicsEngine::OrderIndependentTransparencyRenderer* oitRenderer);

		void SetInstancingRenderer(DUOLGraphicsEngine::InstancingManager* instancingManger);

		void ClearOITRTVs();

		bool GetPipelineQueryInfo(DUOLGraphicsLibrary::QueryInfo& info);

		void ExecuteDebugRenderPass(RenderingPipeline* renderPipeline);

		void ExecuteDebugRenderTargetPass(RenderingPipeline* renderPipeline);

		void OcclusionCulling(OcclusionCulling* occlusionCulling, const std::vector<RenderObject*>& inObjects, std::vector<RenderObject*>& outObjects);

		void RenderSkyBox(RenderingPipeline* skyBox, DUOLGraphicsLibrary::Buffer* vertices, DUOLGraphicsLibrary::Buffer* indices, const Camera& cameraInfo);

		void RenderCascadeShadow(DUOLGraphicsLibrary::PipelineState* shadowMesh, DUOLGraphicsLibrary::PipelineState* shadowSkinnedMesh, DUOLGraphicsLibrary::RenderTarget* shadowRenderTarget, const
		                         ConstantBufferPerCamera& perCameraInfo, const std::vector<RenderObject*>& renderObjects);

		void RenderShadow(DUOLGraphicsLibrary::PipelineState* shadowMesh, DUOLGraphicsLibrary::PipelineState* shadowSkinnedMesh, DUOLGraphicsLibrary::RenderTarget* shadowRenderTarget, const RenderObject* renderObject, int
		                  shadowIdx, bool isStatic);

		void RenderCanvas(RenderingPipeline* uiRenderer, DUOLGraphicsLibrary::Texture* canvas);

		void SetPerFrameBuffer(const ConstantBufferPerFrame& buffer);

		void BindBackBuffer(DUOLGraphicsLibrary::RenderPass* backbuffer);

		void CreateCubeMapFromPanoramaImage(DUOLGraphicsLibrary::Texture* panorama, DUOLGraphicsLibrary::RenderTarget* cubeMap[6], DUOLGraphicsLibrary::PipelineState* pipelineState, DUOLGraphicsLibrary::Buffer* perObject, DUOLGraphicsLibrary::Sampler* linearSampler);

		void CreatePreFilteredMapFromCubeImage(DUOLGraphicsLibrary::Texture* cubeMap, DUOLGraphicsLibrary::RenderTarget** RadianceMap, DUOLGraphicsLibrary::
		                                       PipelineState* pipelineState, DUOLGraphicsLibrary::RenderTarget* depth, DUOLGraphicsLibrary::Buffer* perObject, DUOLGraphicsLibrary
		                                       ::Sampler* linearSampler, UINT mipmapSize, float width, float height);

		void CreateBRDFLookUpTable(DUOLGraphicsLibrary::RenderTarget* BRDFLookUp, DUOLGraphicsLibrary::PipelineState* pipelineState, DUOLGraphicsLibrary::RenderTarget* depth, DUOLGraphicsLibrary::Buffer* perObject, UINT width, UINT height);

		bool GetRenderData(DUOLGraphicsLibrary::QueryInfo& outData);

		void SetPerCameraBuffer(const ConstantBufferPerCamera& perCameraBuffer);

		void BakeShadows(const ConstantBufferPerCamera& perCameraBuffer);

	private:
		int GetNumIndicesFromBuffer(DUOLGraphicsLibrary::Buffer* indexBuffer);

		void CreatePostProcessingRect();

		void ExecuteRenderPass(RenderingPipeline* renderPipeline, const std::vector<DecomposedRenderData>& renderObjects);

		void ExecutePostProcessingPass(RenderingPipeline* renderPipeline, void* postProcessingData = nullptr, int dataSize = 0);

		void ExecuteTransparencyPass(RenderingPipeline* renderPipeline); // 0을 입력했을 때만 다르게 처리한다.

		void RenderMesh(DecomposedRenderData& renderObject, RenderingPipeline* renderPipeline);

		void RenderTransparencyMesh(DecomposedRenderData& renderObject, RenderingPipeline* renderPipeline);

		void RenderParticle(DecomposedRenderData& renderObject, RenderingPipeline* renderPipeline);
	};
}
