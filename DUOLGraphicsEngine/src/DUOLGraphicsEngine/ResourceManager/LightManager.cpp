#include "DUOLGraphicsEngine/ResourceManager/LightManager.h"
#include "DUOLGraphicsEngine/ResourceManager/ResourceManager.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/Light.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/CascadeShadow.h"
#include "DUOLGraphicsEngine/Util/Hash/Hash.h"

DUOLGraphicsEngine::LightManager::LightManager(ResourceManager* resourceManager)
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

void DUOLGraphicsEngine::LightManager::GenerateShadowMaps(ResourceManager* resourceManager)
{
	_spotUsableSpace.reserve(_maxTextur2DShadowMapCount);
	_pointUsableSpace.reserve(_maxCubeShadowMapCount);

	for(int i = 0; i < _maxTextur2DShadowMapCount; i++)
	{
		_spotUsableSpace.push_back(i);
	}
	for(int i = 0; i < _maxCubeShadowMapCount; i++)
	{
		_pointUsableSpace.push_back(i);
	}


	DUOLGraphicsLibrary::TextureDesc spotShadowDesc;

	spotShadowDesc._format = DUOLGraphicsLibrary::ResourceFormat::FORMAT_R32_TYPELESS;
	spotShadowDesc._arraySize = _maxTextur2DShadowMapCount;
	spotShadowDesc._mipLevels = 1;
	spotShadowDesc._textureExtent = DUOLMath::Vector3{ 512, 512, 1 };
	spotShadowDesc._bindFlags = (static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE) | static_cast<long>(DUOLGraphicsLibrary::BindFlags::DEPTHSTENCIL));
	spotShadowDesc._type = DUOLGraphicsLibrary::TextureType::TEXTURE2DARRAY;

	_spotShadowMaps = resourceManager->CreateTexture(_T("spotShadowMap"), spotShadowDesc);

	DUOLGraphicsLibrary::RenderTargetDesc renderTargetDesc;

	renderTargetDesc._texture = _spotShadowMaps;
	renderTargetDesc._type = DUOLGraphicsLibrary::RenderTargetType::Depth;
	renderTargetDesc._arrayLayer = 0;
	renderTargetDesc._mipLevel = 0;
	renderTargetDesc._arraySize = _maxTextur2DShadowMapCount;

	_spotRenderTargets = resourceManager->CreateRenderTarget(_T("spotShadowMap"), renderTargetDesc);

	DUOLGraphicsLibrary::TextureDesc pointShadowDesc;

	pointShadowDesc._format = DUOLGraphicsLibrary::ResourceFormat::FORMAT_R32_TYPELESS;
	pointShadowDesc._mipLevels = 1;
	pointShadowDesc._textureExtent = DUOLMath::Vector3{ 512, 512, 1 };
	pointShadowDesc._arraySize = _maxCubeShadowMapCount * 6;
	pointShadowDesc._bindFlags = (static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE) | static_cast<long>(DUOLGraphicsLibrary::BindFlags::DEPTHSTENCIL));
	pointShadowDesc._type = DUOLGraphicsLibrary::TextureType::TEXTURECUBEARRAY;

	_pointLightShadowMaps = resourceManager->CreateTexture(_T("pointShadowMap"), pointShadowDesc);

	renderTargetDesc._texture = _pointLightShadowMaps;
	renderTargetDesc._type = DUOLGraphicsLibrary::RenderTargetType::Depth;
	renderTargetDesc._arrayLayer = 0;
	renderTargetDesc._arraySize = _maxCubeShadowMapCount * 6;

	_pointRenderTargets = resourceManager->CreateRenderTarget(_T("pointShadowMap"), renderTargetDesc);
}

void DUOLGraphicsEngine::LightManager::SetPipelines(ResourceManager* resourceManager)
{
	_spotSkinned = resourceManager->GetPipelineState(Hash::Hash64(_T("SpotShadowSkinnedVS")));
	_spotStatic = resourceManager->GetPipelineState(Hash::Hash64(_T("SpotShadowMeshVS")));
}

DUOLGraphicsLibrary::Texture* DUOLGraphicsEngine::LightManager::GetSpotShadowMaps() const
{
	return _spotShadowMaps;
}

DUOLGraphicsLibrary::RenderTarget* DUOLGraphicsEngine::LightManager::GetSpotRenderTargets() const
{
	return _spotRenderTargets;
}

DUOLGraphicsLibrary::Texture* DUOLGraphicsEngine::LightManager::GetPointLightShadowMaps() const
{
	return _pointLightShadowMaps;
}

DUOLGraphicsLibrary::RenderTarget* DUOLGraphicsEngine::LightManager::GetPointRenderTargets() const
{
	return _pointRenderTargets;
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
	if (_spotUsableSpace.size() > 0)
	{
		auto ret = _spotUsableSpace.back();
		_spotUsableSpace.pop_back();

		return ret;
	}

	//더이상 여유공간이 없음.
	return -1;
}

bool DUOLGraphicsEngine::LightManager::ReturnTexture2DShadowSpace(int idx)
{
	//brutal force 순회 셰도우맵이 많아봤자... 100개 되려나? 그리고, 게임 런타임중에 돌 일이 없음.
	for (auto& spaceIdx : _spotUsableSpace)
	{
		//에러상황
		if (spaceIdx == idx)
			return false;
	}

	_spotUsableSpace.push_back(idx);

	return true;
}

int DUOLGraphicsEngine::LightManager::GetCubeMapShadowSpace()
{
	if (_pointUsableSpace.size() > 0)
	{
		auto ret = _pointUsableSpace.back();
		_pointUsableSpace.pop_back();

		return ret;
	}

	//더이상 여유공간이 없음.
	return -1;
}

bool DUOLGraphicsEngine::LightManager::ReturnCubeMapShadowSpace(int idx)
{
	//brutal force 순회 셰도우맵이 많아봤자... 100개 되려나? 그리고, 게임 런타임중에 돌 일이 없음.
	for (auto& spaceIdx : _pointUsableSpace)
	{
		//에러상황
		if (spaceIdx == idx)
			return false;
	}

	_pointUsableSpace.push_back(idx);

	return true;
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

