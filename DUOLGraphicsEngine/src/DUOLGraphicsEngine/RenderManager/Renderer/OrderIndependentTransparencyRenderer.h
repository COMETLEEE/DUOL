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

namespace DUOLGraphicsEngine
{
	class RenderingPipeline;
	class ResourceManager;

/**

    @class   OrderIndependentTransparencyRenderer
    @brief   OIT�� �����ϰ�, ��ƼŬ �������� �ʿ��� ���̴��ڵ�� ���ҽ����� ����ִ� Ȧ���Դϴ�.
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

public:
	void ClearOITRTVs(DUOLGraphicsLibrary::Renderer* renderer);

	DUOLGraphicsEngine::RenderingPipeline* GetOITPipeline() const;

	DUOLGraphicsEngine::RenderingPipeline* GetOITLayerCreateRenderingPipeline() const;

	DUOLGraphicsLibrary::Texture* GetParticleRandomTexture() const;

	DUOLGraphicsLibrary::PipelineState* GetParticleShader() const;

	DUOLGraphicsLibrary::PipelineState* GetParticleTrailShader() const;

	DUOLGraphicsLibrary::ResourceViewLayout& GetParticleUpdateLayout() ;

	DUOLGraphicsLibrary::ResourceViewLayout& GetParticleDrawLayout();

	DUOLGraphicsLibrary::ResourceViewLayout& GetTransparencyDrawLayout();

	DUOLGraphicsLibrary::RenderTarget* GetDefaultDepth() const;

private:
	DUOLGraphicsEngine::RenderingPipeline* _oitPipeline;

	int _layerCount;

	/**
	 * \brief for particle system
	 */
	DUOLGraphicsEngine::RenderingPipeline* _oitLayerCreateRenderingPipeline;

	DUOLGraphicsLibrary::Texture* _particleRandomTexture;

	DUOLGraphicsLibrary::PipelineState* _particleShader;

	DUOLGraphicsLibrary::PipelineState* _particleTrailShader;

	DUOLGraphicsLibrary::ResourceViewLayout _particleUpdateLayout;

	DUOLGraphicsLibrary::RenderTarget* _defaultDepth;

	DUOLGraphicsLibrary::ResourceViewLayout _particleDrawLayout;

	DUOLGraphicsLibrary::ResourceViewLayout _transparencyDrawLayout;
	
};
}
