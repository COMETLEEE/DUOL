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

		//Todo:: �ٱ����� ������� ���
		DUOLGraphicsLibrary::Buffer* _particleBuffer;

		DUOLGraphicsLibrary::Buffer* _counterBuffer;

		DUOLGraphicsLibrary::Buffer* _oitLayerBuffer;

		DUOLGraphicsLibrary::Buffer* _firstOffsetBuffer;

		DUOLGraphicsLibrary::Buffer* _perFrameBuffer;

		DUOLGraphicsLibrary::Buffer* _perCameraBuffer;

		DUOLGraphicsLibrary::Buffer* _perObjectBuffer;

		//particle system
		DUOLGraphicsLibrary::PipelineState* _particleShader;

		DUOLGraphicsLibrary::PipelineState* _particleTrailShader;

		DUOLGraphicsLibrary::Texture* _particleRandomTexture;

		DUOLGraphicsLibrary::Buffer* _postProcessingRectVertex;

		DUOLGraphicsLibrary::Buffer* _postProcessingRectIndex;

		DUOLGraphicsLibrary::Buffer* _axisVertex;

		DUOLGraphicsLibrary::Buffer* _axisIndex;

		////����ť�� 1�� ������ �����̴�.
		//std::priority_queue<UINT32, std::vector<RenderObject*>, std::less<UINT32>> _renderQueue;

		std::unique_ptr<ByteBuffer> _buffer;
		
		//������ ���������� Resources slot
		DUOLGraphicsLibrary::ResourceViewLayout _currentBindTextures;

		DUOLGraphicsLibrary::ResourceViewLayout _perFrameBufferBinder;

		DUOLGraphicsLibrary::ResourceViewLayout _perCameraBufferBinder;

		DUOLGraphicsLibrary::ResourceViewLayout _perObjectBufferBinder;

		std::vector<DUOLGraphicsEngine::RenderObject*> _debugRenderQueue;

		int _oitDrawCount;
	public:
		//Todo ���������� �ȵǴ� �Լ���
		void PushDebugObject(RenderObject* render_object);

		void CreateAxis(DUOLGraphicsLibrary::Renderer* renderer);

		void SetParticleShader(DUOLGraphicsLibrary::PipelineState* particle, DUOLGraphicsLibrary::PipelineState* trailPipeline, DUOLGraphicsEngine::
		                       RenderingPipeline* oitPipeline);

		void CreateParticleBuffer(DUOLGraphicsLibrary::Renderer* renderer, int width, int height);
		//

		void ReserveResourceLayout();

		void ExecuteRenderingPipeline(RenderingPipeline* renderPipeline, const std::vector<DecomposedRenderData>& renderObjects, void* postProcessingData = nullptr, int dataSize = 0);

		void OnResize(const DUOLMath::Vector2& resolution);

		void CopyTexture(DUOLGraphicsLibrary::Texture* destTexture, DUOLGraphicsLibrary::Texture* srcTexture);

		void Present();

		void Begin();

		void ClearState();

		void End();

		void ClearOITUAVs();

		bool GetPipelineQueryInfo(DUOLGraphicsLibrary::QueryInfo& info);

		void ExecuteDebugRenderPass(RenderingPipeline* renderPipeline);

		void ExecuteDebugRenderTargetPass(RenderingPipeline* renderPipeline);

		void OcclusionCulling(OcclusionCulling* occlusionCulling, const std::vector<DecomposedRenderData>& inObjects, std::vector<DecomposedRenderData>& outObjects);

		void RenderSkyBox(RenderingPipeline* skyBox, DUOLGraphicsLibrary::Buffer* vertices, DUOLGraphicsLibrary::Buffer* indices, const Camera& cameraInfo);

		void RenderCascadeShadow(DUOLGraphicsLibrary::PipelineState* shadowMesh, DUOLGraphicsLibrary::PipelineState* shadowSkinnedMesh, DUOLGraphicsLibrary::RenderTarget* shadowRenderTarget, const ConstantBufferPerFrame& perFrameInfo, const std::vector<RenderObject*>& renderObjects);

		void RenderSpotShadow(DUOLGraphicsLibrary::PipelineState* shadowMesh, DUOLGraphicsLibrary::PipelineState* shadowSkinnedMesh, DUOLGraphicsLibrary::RenderTarget* shadowRenderTarget, const std::vector<RenderObject*>& renderObjects, int
		                      shadowIdx);

		void RenderCanvas(RenderingPipeline* uiRenderer, DUOLGraphicsLibrary::Texture* canvas);

		void SetPerFrameBuffer(const ConstantBufferPerFrame& buffer);

		void BindBackBuffer(DUOLGraphicsLibrary::RenderPass* backbuffer);

		void CreateCubeMapFromPanoramaImage(DUOLGraphicsLibrary::Texture* panorama, DUOLGraphicsLibrary::RenderTarget* cubeMap[6], DUOLGraphicsLibrary::PipelineState* pipelineState, DUOLGraphicsLibrary::Buffer* perObject, DUOLGraphicsLibrary::Sampler* linearSampler);

		void CreatePreFilteredMapFromCubeImage(DUOLGraphicsLibrary::Texture* cubeMap, DUOLGraphicsLibrary::RenderTarget** RadianceMap, DUOLGraphicsLibrary::
		                                       PipelineState* pipelineState, DUOLGraphicsLibrary::RenderTarget* depth, DUOLGraphicsLibrary::Buffer* perObject, DUOLGraphicsLibrary
		                                       ::Sampler* linearSampler, UINT mipmapSize, float width, float height);

		void CreateBRDFLookUpTable(DUOLGraphicsLibrary::RenderTarget* BRDFLookUp, DUOLGraphicsLibrary::PipelineState* pipelineState, DUOLGraphicsLibrary::RenderTarget* depth, DUOLGraphicsLibrary::Buffer* perObject, UINT width, UINT height);

		bool GetRenderData(DUOLGraphicsLibrary::QueryInfo& outData);

		void SetPerCameraBuffer(ConstantBufferPerCamera& perCameraBuffer, const ConstantBufferPerFrame& perFrameBuffer);

	private:
		int GetNumIndicesFromBuffer(DUOLGraphicsLibrary::Buffer* indexBuffer);

		void CreatePostProcessingRect();

		void ExecuteRenderPass(RenderingPipeline* renderPipeline, const std::vector<DecomposedRenderData>& renderObjects);

		void ExecutePostProcessingPass(RenderingPipeline* renderPipeline, void* postProcessingData = nullptr, int dataSize = 0);

		void ExecuteTransparencyPass(RenderingPipeline* renderPipeline); // 0�� �Է����� ���� �ٸ��� ó���Ѵ�.

		void RenderMesh(DecomposedRenderData& renderObject, RenderingPipeline* renderPipeline);

		void RenderParticle(DecomposedRenderData& renderObject, RenderingPipeline* renderPipeline, DUOLGraphicsLibrary::PipelineState* particle, DUOLGraphicsLibrary::PipelineState*
		                    particleTrail);
	};
}
