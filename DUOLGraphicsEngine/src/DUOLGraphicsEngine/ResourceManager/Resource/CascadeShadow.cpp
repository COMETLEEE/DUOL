#include "DUOLGraphicsEngine/ResourceManager/Resource/CascadeShadow.h"
#include "DUOLGraphicsEngine/GraphicsEngine/GraphicsEngine.h"
#include "DUOLGraphicsEngine/ResourceManager/ResourceManager.h"
#include "DUOLGraphicsEngine/Util/Hash/Hash.h"
#include "DUOLGraphicsLibrary/RenderTargetFlags.h"
#include "DUOLGraphicsLibrary/ResourceFlags.h"
#include "DUOLGraphicsLibrary/ResourceFormat.h"
#include "DUOLGraphicsLibrary/TextureFlags.h"

//DUOLGraphicsEngine::CascadeShadow::CascadeShadow(DUOLGraphicsEngine::GraphicsEngine* const graphicsEngine)
//{
//	auto resourceManger = graphicsEngine->GetResourceManager();
//
//	DUOLGraphicsLibrary::TextureDesc textureDesc;
//	textureDesc._textureExtent = DUOLMath::Vector3{ (float)textureSize , (float)textureSize, 0 };
//	textureDesc._arraySize = sliceCount;
//	textureDesc._type = DUOLGraphicsLibrary::TextureType::TEXTURE2DARRAY;
//	textureDesc._format = DUOLGraphicsLibrary::ResourceFormat::FORMAT_R24G8_TYPELESS;
//	textureDesc._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::DEPTHSTENCIL) | static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE);
//
//	_shadowMap = _resourceManager->CreateTexture(DUOLCommon::Hash::Hash64(_T("CascadeShadowMap")), textureDesc);
//
//	DUOLGraphicsLibrary::RenderTargetDesc renderTargetDesc;
//
//	renderTargetDesc._type = DUOLGraphicsLibrary::RenderTargetType::DepthStencil;
//	renderTargetDesc._arraySize = sliceCount;
//	renderTargetDesc._resolution = DUOLMath::Vector2{ (float)textureSize, (float)textureSize };
//	renderTargetDesc._texture = _shadowMap;
//
//	_shadowMapDepth = _resourceManager->CreateRenderTarget(_T("CascadeShadowMapDepth"), renderTargetDesc);
//
//	return {};
//}

namespace DUOLGraphicsEngine
{
	DUOLGraphicsEngine::CascadeShadow::CascadeShadow(DUOLGraphicsEngine::GraphicsEngine* const graphicsEngine, int textureSize, int sliceCount):
		_textureSize(textureSize),
		_sliceCount(sliceCount)
	{
		auto resourceManager = graphicsEngine->GetResourceManager();
		CreateShadowMap(resourceManager, _textureSize, _sliceCount);
	}

	DUOLGraphicsEngine::CascadeShadow::~CascadeShadow()
	{
	}

	void CascadeShadow::CreateShadowMap(ResourceManager* resourceManager, int textureSize, int sliceCount)
	{
		DUOLGraphicsLibrary::TextureDesc textureDesc;
		textureDesc._textureExtent = DUOLMath::Vector3{ (float)textureSize , (float)textureSize, 0 };
		textureDesc._arraySize = sliceCount;
		textureDesc._type = DUOLGraphicsLibrary::TextureType::TEXTURE2DARRAY;
		textureDesc._format = DUOLGraphicsLibrary::ResourceFormat::FORMAT_R24G8_TYPELESS;
		textureDesc._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::DEPTHSTENCIL) | static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE);

		_shadowMap = resourceManager->CreateTexture(Hash::Hash64(_T("CascadeShadowMap")), textureDesc);

		DUOLGraphicsLibrary::RenderTargetDesc renderTargetDesc;

		renderTargetDesc._type = DUOLGraphicsLibrary::RenderTargetType::DepthStencil;
		renderTargetDesc._arraySize = sliceCount;
		renderTargetDesc._resolution = DUOLMath::Vector2{ (float)textureSize, (float)textureSize };
		renderTargetDesc._texture = _shadowMap;

		_shadowMapDepth = resourceManager->CreateRenderTarget(_T("CascadeShadowMapDepth"), renderTargetDesc);

		static UINT64 shadowMesh = Hash::Hash64(_T("ShadowMeshVS"));
		static UINT64 shadowSkinned = Hash::Hash64(_T("ShadowSkinnedVS"));

		_shadowStatic = resourceManager->GetPipelineState(shadowMesh);
		_shadowSkinned = resourceManager->GetPipelineState(shadowSkinned);

	}

	DUOLGraphicsLibrary::Texture* CascadeShadow::GetShadowMap() const
	{
		return _shadowMap;
	}

	DUOLGraphicsLibrary::RenderTarget* CascadeShadow::GetShadowMapDepth() const
	{
		return _shadowMapDepth;
	}

	int CascadeShadow::GetTextureSize() const
	{
		return _textureSize;
	}

	int CascadeShadow::GetSliceCount() const
	{
		return _sliceCount;
	}

	DUOLGraphicsLibrary::PipelineState* CascadeShadow::GetShadowSkinned() const
	{
		return _shadowSkinned;
	}

	DUOLGraphicsLibrary::PipelineState* CascadeShadow::GetShadowStatic() const
	{
		return _shadowStatic;
	}
}
