#pragma once


namespace DUOLGraphicsLibrary
{
	class Sampler;
	class PipelineState;
	class RenderTarget;
	class Texture;
}

namespace DUOLGraphicsEngine
{
	class ResourceManager;
	class GraphicsEngine;

 /**

     @class   CascadeShadow
     @brief   캐스케이드 섀도우와 관련된 데이터 홀더.
     @details ~
     @author  KyungMin Oh

 **/
	class CascadeShadow
	{
	public:
		CascadeShadow(DUOLGraphicsEngine::GraphicsEngine* const graphicsEngine, int textureSize, int sliceCount);

		~CascadeShadow();

	public:
		void CreateShadowMap(ResourceManager* resourceManager, int textureSize, int sliceCount);

		//Getter
	public:
		DUOLGraphicsLibrary::Texture* GetShadowMap() const;

		DUOLGraphicsLibrary::RenderTarget* GetShadowMapDepth() const;

		int GetTextureSize() const;

		int GetSliceCount() const;

		DUOLGraphicsLibrary::PipelineState* GetShadowSkinned() const;

		DUOLGraphicsLibrary::PipelineState* GetShadowStatic() const;

		DUOLGraphicsLibrary::Sampler* GetSampler() const;

	private:
		DUOLGraphicsLibrary::Texture* _shadowMap;

		DUOLGraphicsLibrary::Sampler* _samLinear;

		DUOLGraphicsLibrary::RenderTarget* _shadowMapDepth;

		DUOLGraphicsLibrary::PipelineState* _shadowSkinned;

		DUOLGraphicsLibrary::PipelineState* _shadowStatic;

		int _textureSize;

		int _sliceCount;
	};

}
