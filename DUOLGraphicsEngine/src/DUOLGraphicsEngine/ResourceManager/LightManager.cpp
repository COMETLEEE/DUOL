#include "DUOLGraphicsEngine/ResourceManager/LightManager.h"
#include "DUOLGraphicsEngine/ResourceManager/ResourceManager.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/Light.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/CascadeShadow.h"
#include "DUOLGraphicsEngine/Util/Hash/Hash.h"
#include "DUOLGraphicsLibrary/Renderer/Renderer.h"

DUOLGraphicsEngine::LightManager::	LightManager(ResourceManager* resourceManager) :
	_directionalUsableSpace(true)
{
	GenerateShadowMaps(resourceManager);
	SetPipelines(resourceManager);
}

DUOLGraphicsEngine::LightManager::~LightManager()
{
}

int DUOLGraphicsEngine::LightManager::GetShadowMapSpace(LightType type)
{
	switch (type)
	{
	case LightType::Direction:
	{
		if (_directionalUsableSpace)
		{
			_directionalUsableSpace = false;
			return 0;
		}
		else
		{
			return -1;
		}
	}
	case LightType::Point:
	{
		return GetCubeMapShadowSpace();
	}
	case LightType::Spot:
	{	
		return GetTexture2DShadowSpace();
	}
	case LightType::Area:
	{
		return GetTexture2DShadowSpace();
	};
	case LightType::Unknown: break;
	default:;
	}

	return -1;
}

bool DUOLGraphicsEngine::LightManager::ReturnShadowMapSpace(int idx, LightType type)
{
	if(idx == -1)
	{
		//idx -1 이라는 것은.. 할당된 셰도우가 없다는 뜻.
		return true;
	}

	switch (type)
	{
	case LightType::Direction:
	{

		if (_directionalUsableSpace)
		{
			//error... 반납하려는데 왜 또있음?
			return false;
		}
		else
		{
			_directionalUsableSpace =  true;
			return true;
		}
	}
	case LightType::Point:
	{
		return ReturnCubeMapShadowSpace(idx);
	}
	case LightType::Spot:
	{
		return ReturnTexture2DShadowSpace(idx);
	}
	case LightType::Area:
	{
		return ReturnTexture2DShadowSpace(idx);
	};
	case LightType::Unknown: break;
	default:;
	}

	return false;
}

void DUOLGraphicsEngine::LightManager::ClearCubeMapRenderTarget(int idx, DUOLGraphicsLibrary::Renderer* renderer)
{
	renderer->ClearRenderTarget(_cubeMapIndividualShadowRenderTargets[idx]);
}

void DUOLGraphicsEngine::LightManager::GenerateShadowMaps(ResourceManager* resourceManager)
{
	float textureResoluiton = 1024;

	DUOLGraphicsLibrary::TextureDesc spotShadowDesc;

	spotShadowDesc._format = DUOLGraphicsLibrary::ResourceFormat::FORMAT_R32_TYPELESS;
	spotShadowDesc._arraySize = _maxTextur2DShadowMapCount;
	spotShadowDesc._mipLevels = 1;
	spotShadowDesc._textureExtent = DUOLMath::Vector3{ textureResoluiton, textureResoluiton, 1 };
	spotShadowDesc._bindFlags = (static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE) | static_cast<long>(DUOLGraphicsLibrary::BindFlags::DEPTHSTENCIL));
	spotShadowDesc._type = DUOLGraphicsLibrary::TextureType::TEXTURE2DARRAY;

	_texture2DShadowMaps = resourceManager->CreateTexture(_T("spotShadowMap"), spotShadowDesc);

	DUOLGraphicsLibrary::RenderTargetDesc renderTargetDesc;

	renderTargetDesc._texture = _texture2DShadowMaps;
	renderTargetDesc._type = DUOLGraphicsLibrary::RenderTargetType::Depth;
	renderTargetDesc._arrayLayer = 0;
	renderTargetDesc._mipLevel = 0;
	renderTargetDesc._arraySize = _maxTextur2DShadowMapCount;

	_texture2DRenderTargets = resourceManager->CreateRenderTarget(_T("spotShadowMap"), renderTargetDesc, false);

	DUOLGraphicsLibrary::TextureDesc pointShadowDesc;

	pointShadowDesc._format = DUOLGraphicsLibrary::ResourceFormat::FORMAT_R32_TYPELESS;
	pointShadowDesc._mipLevels = 1;
	pointShadowDesc._textureExtent = DUOLMath::Vector3{ textureResoluiton, textureResoluiton, 1 };
	pointShadowDesc._arraySize = _maxCubeShadowMapCount * 6;
	pointShadowDesc._bindFlags = (static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE) | static_cast<long>(DUOLGraphicsLibrary::BindFlags::DEPTHSTENCIL));
	pointShadowDesc._type = DUOLGraphicsLibrary::TextureType::TEXTURECUBEARRAY;

	_cubeMapLightShadowMaps = resourceManager->CreateTexture(_T("pointShadowMap"), pointShadowDesc);

	renderTargetDesc._texture = _cubeMapLightShadowMaps;
	renderTargetDesc._type = DUOLGraphicsLibrary::RenderTargetType::Depth;
	renderTargetDesc._arrayLayer = 0;
	renderTargetDesc._arraySize = _maxCubeShadowMapCount * 6;

	_cubeMapRenderTargets = resourceManager->CreateRenderTarget(_T("pointShadowMap"), renderTargetDesc, false);

	_texture2DUsableSpace.reserve(_maxTextur2DShadowMapCount);
	_cubeMapUsableSpace.reserve(_maxCubeShadowMapCount);
	_cubeMapIndividualShadowRenderTargets.reserve(_maxCubeShadowMapCount);
	_texture2DIndividualShadowRenderTargets.reserve(_maxCubeShadowMapCount*6);

	for (int i = 0; i < _maxTextur2DShadowMapCount; i++)
	{
		_texture2DUsableSpace.push_back(i);

		DUOLGraphicsLibrary::RenderTargetDesc individualRTV;

		individualRTV._texture = _texture2DShadowMaps;
		individualRTV._type = DUOLGraphicsLibrary::RenderTargetType::Depth;
		individualRTV._arrayLayer = i;
		individualRTV._mipLevel = 0;
		individualRTV._arraySize = 1;

		DUOLCommon::tstring id = _T("spotShadowMap");
		id += i;

		auto rtv = resourceManager->CreateRenderTarget(id, individualRTV, false);

		_texture2DIndividualShadowRenderTargets.push_back(rtv);
	}

	for (int i = 0; i < _maxCubeShadowMapCount; i++)
	{
		_cubeMapUsableSpace.push_back(i);

		DUOLGraphicsLibrary::RenderTargetDesc individualRTV;

		individualRTV._texture = _cubeMapLightShadowMaps;
		individualRTV._type = DUOLGraphicsLibrary::RenderTargetType::Depth;
		individualRTV._arrayLayer = i*6;
		individualRTV._mipLevel = 0;
		individualRTV._arraySize = 6;

		DUOLCommon::tstring id = _T("pointShadowMap");
		id += i;

		auto rtv = resourceManager->CreateRenderTarget(id, individualRTV, false);

		_cubeMapIndividualShadowRenderTargets.push_back(rtv);

	}
}

void DUOLGraphicsEngine::LightManager::SetPipelines(ResourceManager* resourceManager)
{
	_spotSkinned = resourceManager->GetPipelineState(Hash::Hash64(_T("SpotShadowSkinned")));
	_spotStatic = resourceManager->GetPipelineState(Hash::Hash64(_T("SpotShadowMesh")));
	_pointSkinned = resourceManager->GetPipelineState(Hash::Hash64(_T("PointShadowSkinned")));
	_pointStatic = resourceManager->GetPipelineState(Hash::Hash64(_T("PointShadowMesh")));
}

DUOLGraphicsLibrary::Texture* DUOLGraphicsEngine::LightManager::GetSpotShadowMaps() const
{
	return _texture2DShadowMaps;
}

DUOLGraphicsLibrary::RenderTarget* DUOLGraphicsEngine::LightManager::GetSpotRenderTargets() const
{
	return _texture2DRenderTargets;
}

DUOLGraphicsLibrary::Texture* DUOLGraphicsEngine::LightManager::GetPointLightShadowMaps() const
{
	return _cubeMapLightShadowMaps;
}

DUOLGraphicsLibrary::RenderTarget* DUOLGraphicsEngine::LightManager::GetPointRenderTargets() const
{
	return _cubeMapRenderTargets;
}

void DUOLGraphicsEngine::LightManager::ClearTexture2DRenderTarget(int idx, DUOLGraphicsLibrary::Renderer* renderer)
{
	renderer->ClearRenderTarget(_texture2DIndividualShadowRenderTargets[idx]);
}

DUOLGraphicsLibrary::PipelineState* DUOLGraphicsEngine::LightManager::GetSpotSkinnedPipeline() const
{
	return _spotSkinned;
}

DUOLGraphicsLibrary::PipelineState* DUOLGraphicsEngine::LightManager::GetSpotStaticPipeline() const
{
	return _spotStatic;
}

int DUOLGraphicsEngine::LightManager::GetTexture2DShadowSpace()
{
	if (_texture2DUsableSpace.size() > 0)
	{
		auto ret = _texture2DUsableSpace.back();
		_texture2DUsableSpace.pop_back();

		return ret;
	}

	//더이상 여유공간이 없음.
	return -1;
}

bool DUOLGraphicsEngine::LightManager::ReturnTexture2DShadowSpace(int idx)
{
	//brutal force 순회 셰도우맵이 많아봤자... 100개 되려나? 그리고, 게임 런타임중에 돌 일이 없음.
	for (auto& spaceIdx : _texture2DUsableSpace)
	{
		//에러상황
		if (spaceIdx == idx)
			return false;
	}

	_texture2DUsableSpace.push_back(idx);

	return true;
}

int DUOLGraphicsEngine::LightManager::GetCubeMapShadowSpace()
{
	if (_cubeMapUsableSpace.size() > 0)
	{
		auto ret = _cubeMapUsableSpace.back();
		_cubeMapUsableSpace.pop_back();

		return ret;
	}

	//더이상 여유공간이 없음.
	return -1;
}

bool DUOLGraphicsEngine::LightManager::ReturnCubeMapShadowSpace(int idx)
{
	//brutal force 순회 셰도우맵이 많아봤자... 100개 되려나? 그리고, 게임 런타임중에 돌 일이 없음.
	for (auto& spaceIdx : _cubeMapUsableSpace)
	{
		//에러상황
		if (spaceIdx == idx)
			return false;
	}

	_cubeMapUsableSpace.push_back(idx);

	return true;
}

DUOLGraphicsLibrary::PipelineState* DUOLGraphicsEngine::LightManager::GetPointSkinnedPipeline() const
{
	return _pointSkinned;
}

DUOLGraphicsLibrary::PipelineState* DUOLGraphicsEngine::LightManager::GetPointStaticPipeline() const
{
	return _pointStatic;
}

DUOLGraphicsEngine::Light* DUOLGraphicsEngine::LightManager::CreateLight(uint64_t uuid)
{
	auto foundLight = _lights.find(uuid);

	Light* rawPtr = nullptr;

	if (foundLight == _lights.end())
	{
		auto lightObj = std::make_unique<Light>(this);
		rawPtr = lightObj.get();

		_lights.emplace(uuid, std::move(lightObj));
	}
	else
	{
		rawPtr = foundLight->second.get();
	}

	return rawPtr;
}

bool DUOLGraphicsEngine::LightManager::DeleteLight(uint64_t uuid)
{
	_lights.erase(uuid);

	return true;
}

