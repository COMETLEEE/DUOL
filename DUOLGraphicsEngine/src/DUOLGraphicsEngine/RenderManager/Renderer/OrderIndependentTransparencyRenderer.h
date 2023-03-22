#pragma once
#include "DUOLGraphicsLibrary/Renderer/ResourceViewLayout.h"
#include "DUOLMath/DUOLMath.h"

namespace DUOLGraphicsLibrary
{
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

	void SetOITLayout();

	void CreateOITBuffer(DUOLGraphicsEngine::ResourceManager* resourceManager, const DUOLMath::Vector2& screenSize);

	void CreateParticleRandomTexture(DUOLGraphicsEngine::ResourceManager* resourceManager);

public:
	DUOLGraphicsLibrary::Buffer* GetOITLayerBuffer() const;

	DUOLGraphicsLibrary::Buffer* GetFirstOffsetBuffer() const;

	DUOLGraphicsEngine::RenderingPipeline* GetOITPipeline() const;

	DUOLGraphicsLibrary::Texture* GetParticleRandomTexture() const;

	DUOLGraphicsLibrary::PipelineState* GetParticleShader() const;

	DUOLGraphicsLibrary::PipelineState* GetParticleTrailShader() const;

	DUOLGraphicsLibrary::ResourceViewLayout& GetParticleUpdateLayout() ;

	DUOLGraphicsLibrary::ResourceViewLayout& GetParticleDrawLayout();

private:
	DUOLGraphicsLibrary::Buffer* _oitLayerBuffer;

	DUOLGraphicsLibrary::Buffer* _firstOffsetBuffer;

	DUOLGraphicsEngine::RenderingPipeline* _oitPipeline;

	int _layerCount;

	/**
	 * \brief for particle system
	 */
	DUOLGraphicsEngine::RenderingPipeline* _particlePipeline;

	DUOLGraphicsLibrary::Texture* _particleRandomTexture;

	DUOLGraphicsLibrary::PipelineState* _particleShader;

	DUOLGraphicsLibrary::PipelineState* _particleTrailShader;

	DUOLGraphicsLibrary::ResourceViewLayout _particleUpdateLayout;

private:
	DUOLGraphicsLibrary::ResourceViewLayout _particleDrawLayout;


};
}
