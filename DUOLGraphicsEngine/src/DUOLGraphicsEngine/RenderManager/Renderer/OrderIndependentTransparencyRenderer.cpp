#include "DUOLGraphicsEngine/RenderManager/Renderer/OrderIndependentTransparencyRenderer.h"

#include "DUOLGraphicsEngine/ResourceManager/ResourceManager.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/Particle.h"
#include "DUOLGraphicsEngine/Util/Hash/Hash.h"

#include "DUOLGraphicsLibrary/Renderer/Buffer.h"
#include "DUOLGraphicsLibrary/Renderer/Texture.h"
#include "DUOLGraphicsLibrary/ResourceFlags.h"
#include "DUOLGraphicsLibrary/ResourceFormat.h"
#include "DUOLGraphicsLibrary/TextureFlags.h"
namespace DUOLGraphicsEngine
{
	OrderIndependentTransparencyRenderer::OrderIndependentTransparencyRenderer(
		DUOLGraphicsEngine::ResourceManager* resourceManager,
		const DUOLMath::Vector2& screenSize,
		int layerCount) :
		_layerCount(layerCount)
	{
		_oitPipeline = resourceManager->GetRenderingPipeline(_T("OIT"));
		_particlePipeline = resourceManager->GetRenderingPipeline(_T("Particle"));
		_particleShader = resourceManager->GetPipelineState(DUOLGraphicsEngine::Hash::Hash64(_T("BasicParticle_Particle")));
		_particleTrailShader = resourceManager->GetPipelineState(DUOLGraphicsEngine::Hash::Hash64(_T("BasicParticle_Trail")));

		_defaultDepth = resourceManager->GetRenderTarget(Hash::Hash64(_T("DefaultDepth")));

		CreateOITBuffer(resourceManager, screenSize);
		CreateParticleRandomTexture(resourceManager);
		SetParticleResourceLayout();
		SetOITLayout();

	}

	void OrderIndependentTransparencyRenderer::OnResize(DUOLGraphicsEngine::ResourceManager* resourceManager,
		const DUOLMath::Vector2& screenSize)
	{
		CreateOITBuffer(resourceManager, screenSize);
		SetParticleResourceLayout();
		SetOITLayout();
	}

	void OrderIndependentTransparencyRenderer::SetParticleResourceLayout()
	{
		///////// 파티클 업데이트 레이아웃
		_particleUpdateLayout._resourceViews.resize(4);
		_particleUpdateLayout._resourceViews[0]._resource = _particleRandomTexture;
		_particleUpdateLayout._resourceViews[0]._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE);
		_particleUpdateLayout._resourceViews[0]._slot = 0;
		_particleUpdateLayout._resourceViews[0]._stageFlags = static_cast<long>(DUOLGraphicsLibrary::StageFlags::COMPUTESTAGE);

		//노이즈맵
		_particleUpdateLayout._resourceViews[1]._resource = nullptr;
		_particleUpdateLayout._resourceViews[1]._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE);
		_particleUpdateLayout._resourceViews[1]._slot = 1;
		_particleUpdateLayout._resourceViews[1]._stageFlags = static_cast<long>(DUOLGraphicsLibrary::StageFlags::COMPUTESTAGE);

		//파티클 버퍼
		_particleUpdateLayout._resourceViews[2]._resource = nullptr;
		_particleUpdateLayout._resourceViews[2]._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::UNORDEREDACCESS);
		_particleUpdateLayout._resourceViews[2]._slot = 0;
		_particleUpdateLayout._resourceViews[2]._stageFlags = static_cast<long>(DUOLGraphicsLibrary::StageFlags::COMPUTESTAGE);

		//파티클의 카운터버퍼
		_particleUpdateLayout._resourceViews[3]._resource = nullptr;
		_particleUpdateLayout._resourceViews[3]._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::UNORDEREDACCESS);
		_particleUpdateLayout._resourceViews[3]._slot = 1;
		_particleUpdateLayout._resourceViews[3]._stageFlags = static_cast<long>(DUOLGraphicsLibrary::StageFlags::COMPUTESTAGE);

		//////// 파티클 그리기 레이아웃
		_particleDrawLayout._resourceViews.resize(7);
		_particleDrawLayout._resourceViews[0]._resource = _particleRandomTexture;
		_particleDrawLayout._resourceViews[0]._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE);
		_particleDrawLayout._resourceViews[0]._slot = 0;
		_particleDrawLayout._resourceViews[0]._stageFlags = static_cast<long>(DUOLGraphicsLibrary::StageFlags::GEOMETRYSTAGE);

		//노이즈맵
		_particleDrawLayout._resourceViews[1]._resource = nullptr;
		_particleDrawLayout._resourceViews[1]._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE);
		_particleDrawLayout._resourceViews[1]._slot = 1;
		_particleDrawLayout._resourceViews[1]._stageFlags = static_cast<long>(DUOLGraphicsLibrary::StageFlags::GEOMETRYSTAGE);

		//파티클 버퍼
		_particleDrawLayout._resourceViews[2]._resource = nullptr;
		_particleDrawLayout._resourceViews[2]._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE);
		_particleDrawLayout._resourceViews[2]._slot = 2;
		_particleDrawLayout._resourceViews[2]._stageFlags = static_cast<long>(DUOLGraphicsLibrary::StageFlags::GEOMETRYSTAGE);

		//파티클의 Albedo Texture
		_particleDrawLayout._resourceViews[3]._resource = nullptr;
		_particleDrawLayout._resourceViews[3]._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE);
		_particleDrawLayout._resourceViews[3]._stageFlags = static_cast<long>(DUOLGraphicsLibrary::StageFlags::PIXELSTAGE);
		_particleDrawLayout._resourceViews[3]._slot = 0;

		////뎁스버퍼
		//_particleDrawLayout._resourceViews[4]._resource = _particlePipeline->GetTextureResourceViewLayout()._resourceViews[0]._resource;
		//_particleDrawLayout._resourceViews[4]._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE);
		//_particleDrawLayout._resourceViews[4]._stageFlags = static_cast<long>(DUOLGraphicsLibrary::StageFlags::PIXELSTAGE);
		//_particleDrawLayout._resourceViews[4]._slot = 1;

		//픽셀정보
		_particleDrawLayout._resourceViews[5]._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::UNORDEREDACCESS);
		_particleDrawLayout._resourceViews[5]._resource = _oitLayerBuffer;
		_particleDrawLayout._resourceViews[5]._stageFlags = static_cast<long>(DUOLGraphicsLibrary::StageFlags::PIXELSTAGE);
		_particleDrawLayout._resourceViews[5]._slot = 0;

		//픽셀 위치 데이터
		_particleDrawLayout._resourceViews[6]._resource = _firstOffsetBuffer;
		_particleDrawLayout._resourceViews[6]._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::UNORDEREDACCESS);
		_particleDrawLayout._resourceViews[6]._stageFlags = static_cast<long>(DUOLGraphicsLibrary::StageFlags::PIXELSTAGE);
		_particleDrawLayout._resourceViews[6]._slot = 1;

		_transparencyDrawLayout._resourceViews.resize(8);

		//픽셀정보
		_transparencyDrawLayout._resourceViews[0]._resource = _oitLayerBuffer;
		_transparencyDrawLayout._resourceViews[0]._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::UNORDEREDACCESS);
		_transparencyDrawLayout._resourceViews[0]._stageFlags = static_cast<long>(DUOLGraphicsLibrary::StageFlags::PIXELSTAGE);
		_transparencyDrawLayout._resourceViews[0]._slot = 0;

		//픽셀 위치 데이터
		_transparencyDrawLayout._resourceViews[1]._resource = _firstOffsetBuffer;
		_transparencyDrawLayout._resourceViews[1]._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::UNORDEREDACCESS);
		_transparencyDrawLayout._resourceViews[1]._stageFlags = static_cast<long>(DUOLGraphicsLibrary::StageFlags::PIXELSTAGE);
		_transparencyDrawLayout._resourceViews[1]._slot = 1;

	}

	void OrderIndependentTransparencyRenderer::SetOITLayout()
	{
		auto& textureLayout = _oitPipeline->GetTextureResourceViewLayout();

		textureLayout._resourceViews[2]._resource = _oitLayerBuffer;
		textureLayout._resourceViews[2]._stageFlags = static_cast<long>(DUOLGraphicsLibrary::StageFlags::PIXELSTAGE);
		textureLayout._resourceViews[3]._resource = _firstOffsetBuffer;
		textureLayout._resourceViews[3]._stageFlags = static_cast<long>(DUOLGraphicsLibrary::StageFlags::PIXELSTAGE);
	}

	void OrderIndependentTransparencyRenderer::CreateOITBuffer(DUOLGraphicsEngine::ResourceManager* resourceManager,
	                                                           const DUOLMath::Vector2& screenSize)
	{
		//기존에 있을 데이터 삭제
		resourceManager->DeleteBuffer(_T("oitLayerBuffer"));
		resourceManager->DeleteBuffer(_T("firstOffsetBuffer"));

		_oitLayerBuffer = nullptr;
		_firstOffsetBuffer = nullptr;

		UINT elementsCount = screenSize.x * screenSize.y * _layerCount;

		DUOLGraphicsLibrary::BufferDesc oitLayerTexture;
		std::vector<PixelNode> initVertex(elementsCount);

		oitLayerTexture._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::UNORDEREDACCESS) | static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE);
		oitLayerTexture._usage = DUOLGraphicsLibrary::ResourceUsage::USAGE_DEFAULT;
		oitLayerTexture._stride = sizeof(DUOLGraphicsEngine::PixelNode);
		oitLayerTexture._size = sizeof(DUOLGraphicsEngine::PixelNode) * elementsCount;
		oitLayerTexture._format = DUOLGraphicsLibrary::ResourceFormat::FORMAT_UNKNOWN;
		oitLayerTexture._cpuAccessFlags = 0;
		oitLayerTexture._miscFlags = static_cast<long>(DUOLGraphicsLibrary::MiscFlags::RESOURCE_MISC_BUFFER_STRUCTURED) | static_cast<long>(DUOLGraphicsLibrary::MiscFlags::RESOURCE_MISC_COUNTER);

		_oitLayerBuffer = resourceManager->CreateBuffer(DUOLGraphicsEngine::Hash::Hash64(_T("oitLayerBuffer")), oitLayerTexture, initVertex.data());

		// -------------------------------------------------------------------------------------------------------
		DUOLGraphicsLibrary::BufferDesc firstOffsetDesc;

		std::vector<unsigned int> initData(screenSize.x * screenSize.y);

		firstOffsetDesc._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::UNORDEREDACCESS) | static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE);
		firstOffsetDesc._usage = DUOLGraphicsLibrary::ResourceUsage::USAGE_DEFAULT;
		firstOffsetDesc._stride = 0;
		firstOffsetDesc._size = sizeof(unsigned int) * screenSize.x * screenSize.y;
		firstOffsetDesc._format = DUOLGraphicsLibrary::ResourceFormat::FORMAT_UNKNOWN;
		firstOffsetDesc._cpuAccessFlags = 0;
		firstOffsetDesc._miscFlags = static_cast<long>(DUOLGraphicsLibrary::MiscFlags::RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS);

		_firstOffsetBuffer = resourceManager->CreateBuffer(DUOLGraphicsEngine::Hash::Hash64(_T("firstOffsetBuffer")), firstOffsetDesc, initData.data());

	}

	void OrderIndependentTransparencyRenderer::CreateParticleRandomTexture(DUOLGraphicsEngine::ResourceManager* resourceManager)
	{
		DUOLMath::Vector4 randomValues[1024];

		for (int i = 0; i < 1024; ++i)
		{
			randomValues[i].x = DUOLMath::MathHelper::RandF(-1.0f, 1.0f);
			randomValues[i].y = DUOLMath::MathHelper::RandF(-1.0f, 1.0f);
			randomValues[i].z = DUOLMath::MathHelper::RandF(-1.0f, 1.0f);
			randomValues[i].w = DUOLMath::MathHelper::RandF(-1.0f, 1.0f);
		}

		DUOLGraphicsLibrary::TextureDesc textureDesc;

		textureDesc._initData = randomValues;
		textureDesc._type = DUOLGraphicsLibrary::TextureType::TEXTURE1D;
		textureDesc._size = 1024 * sizeof(DUOLMath::Vector4);
		textureDesc._mipLevels = 1;
		textureDesc._textureExtent = DUOLMath::Vector3{ 1024.f, 0.f, 0.f };
		textureDesc._usage = DUOLGraphicsLibrary::ResourceUsage::USAGE_IMMUTABLE;
		textureDesc._format = DUOLGraphicsLibrary::ResourceFormat::FORMAT_R32G32B32A32_FLOAT;
		textureDesc._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE);
		textureDesc._cpuAccessFlags = 0;
		textureDesc._arraySize = 1;

		_particleRandomTexture = resourceManager->CreateTexture(DUOLGraphicsEngine::Hash::Hash64(_T("ParticleRandomTexture")), textureDesc);
	}

	DUOLGraphicsLibrary::Buffer* OrderIndependentTransparencyRenderer::GetOITLayerBuffer() const
	{
		return _oitLayerBuffer;
	}

	DUOLGraphicsLibrary::Buffer* OrderIndependentTransparencyRenderer::GetFirstOffsetBuffer() const
	{
		return _firstOffsetBuffer;
	}

	DUOLGraphicsEngine::RenderingPipeline* OrderIndependentTransparencyRenderer::GetOITPipeline() const
	{
		return _oitPipeline;
	}

	DUOLGraphicsLibrary::Texture* OrderIndependentTransparencyRenderer::GetParticleRandomTexture() const
	{
		return _particleRandomTexture;
	}

	DUOLGraphicsLibrary::PipelineState* OrderIndependentTransparencyRenderer::GetParticleShader() const
	{
		return _particleShader;
	}

	DUOLGraphicsLibrary::PipelineState* OrderIndependentTransparencyRenderer::GetParticleTrailShader() const
	{
		return _particleTrailShader;
	}

	DUOLGraphicsLibrary::ResourceViewLayout& OrderIndependentTransparencyRenderer::GetParticleUpdateLayout()
	{
		return _particleUpdateLayout;
	}

	DUOLGraphicsLibrary::ResourceViewLayout& OrderIndependentTransparencyRenderer::GetParticleDrawLayout()
	{
		return _particleDrawLayout;
	}

	DUOLGraphicsLibrary::ResourceViewLayout& OrderIndependentTransparencyRenderer::GetTransparencyDrawLayout()
	{
		return _transparencyDrawLayout;
	}

	DUOLGraphicsLibrary::RenderTarget* OrderIndependentTransparencyRenderer::GetDefaultDepth() const
	{
		return _defaultDepth;
	}
}
