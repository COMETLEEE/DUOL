#pragma once
#include "DUOLGraphicsLibrary/Renderer/ResourceViewLayout.h"
#include "DUOLMath/DUOLMath.h"

namespace DUOLGraphicsLibrary
{
	class Renderer;
	class RenderTarget;
	class PipelineState;
	class Texture;
	class Buffer;
}

//struct ClosestTransparencyStruct
//{
//	float _depth;
//	DUOLMath::Vector3 _world;
//	unsigned int _mRAS;
//	DUOLMath::Vector3 _normal;
//	DUOLMath::Vector4 _objectID;
//	DUOLMath::Vector4 _effect;
//};

namespace DUOLGraphicsEngine
{
	class RenderingPipeline;
	class ResourceManager;

/**

    @class   OrderIndependentTransparencyRenderer
    @brief   OIT를 관리하고, 파티클 렌더링에 필요한 셰이더코드와 리소스들을 들고있는 홀더입니다.
    @details ~
    @author  KyungMin Oh

**/
class OrderIndependentTransparencyRenderer
{
public:
	OrderIndependentTransparencyRenderer(DUOLGraphicsEngine::ResourceManager* resourceManager, const DUOLMath::Vector2& screenSize, int layerCount);

public:
	void OnResize(DUOLGraphicsEngine::ResourceManager* resourceManager, const DUOLMath::Vector2& screenSize );

private:
	void SetParticleResourceLayout();

	void CreateParticleRandomTexture(DUOLGraphicsEngine::ResourceManager* resourceManager);

	//void CreateOITClosestPixelInfoBuffer(DUOLGraphicsEngine::ResourceManager* resourceManager, const DUOLMath::Vector2& screenSize);

public:
	void ClearOITRTVs(DUOLGraphicsLibrary::Renderer* renderer);

	DUOLGraphicsEngine::RenderingPipeline* GetOITPipeline() const;

	DUOLGraphicsEngine::RenderingPipeline* GetOITLayerCreateRenderingPipeline() const;

	DUOLGraphicsEngine::RenderingPipeline* GetSecondPassPipeline() const;

	DUOLGraphicsLibrary::Texture* GetParticleRandomTexture() const;

	DUOLGraphicsLibrary::PipelineState* GetParticleShader() const;

	DUOLGraphicsLibrary::PipelineState* GetParticleTrailShader() const;

	DUOLGraphicsLibrary::ResourceViewLayout& GetParticleUpdateLayout() ;

	DUOLGraphicsLibrary::ResourceViewLayout& GetParticleDrawLayout();

	DUOLGraphicsLibrary::ResourceViewLayout& GetTransparencyDrawLayout();

	DUOLGraphicsLibrary::RenderTarget* GetDefaultDepth() const;

private:
	DUOLGraphicsEngine::RenderingPipeline* _oitPipeline;

	DUOLGraphicsEngine::RenderingPipeline* _secondPassPipeline;

	int _layerCount;

	/**
	 * \brief for particle system
	 */
	DUOLGraphicsEngine::RenderingPipeline* _oitLayerCreateRenderingPipeline;

	DUOLGraphicsLibrary::Texture* _particleRandomTexture;

	//DUOLGraphicsLibrary::Buffer* _orderIndependentClosestPixelInfo;

	DUOLGraphicsLibrary::PipelineState* _particleShader;

	DUOLGraphicsLibrary::PipelineState* _particleTrailShader;

	DUOLGraphicsLibrary::ResourceViewLayout _particleUpdateLayout;

	DUOLGraphicsLibrary::RenderTarget* _defaultDepth;

	DUOLGraphicsLibrary::ResourceViewLayout _particleDrawLayout;

	DUOLGraphicsLibrary::ResourceViewLayout _transparencyDrawLayout;

	
};
}
