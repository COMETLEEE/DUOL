#pragma once
#include <memory>
#include <unordered_map>
#include <vector>

#include "DUOLGraphicsEngine/ResourceManager/Resource/Light.h"

namespace DUOLGraphicsLibrary
{
	class PipelineState;
	class RenderTarget;
	class Texture;
}

namespace DUOLGraphicsEngine
{
	class CascadeShadow;
	class RenderingPipeline;
	class ResourceManager;
	/**
     @class   LightManager
     @brief   그림자맵을 관리하는 클래스
			  
     @details ~
     @author  KyungMin Oh

 **/

	class LightManager
	{
	public:
		LightManager(ResourceManager* resourceManager);

		~LightManager();

	public:
		int GetShadowMapSpace(LightType type);

		bool ReturnShadowMapSpace(int idx, LightType type);

		Light* CreateLight(uint64_t uuid);

		bool DeleteLight(uint64_t uuid);

		DUOLGraphicsLibrary::PipelineState* GetSpotSkinnedPipeline() const;

		DUOLGraphicsLibrary::PipelineState* GetSpotStaticPipeline() const;

		DUOLGraphicsLibrary::Texture* GetSpotShadowMaps() const;

		DUOLGraphicsLibrary::RenderTarget* GetSpotRenderTargets() const;

		DUOLGraphicsLibrary::Texture* GetPointLightShadowMaps() const;

		DUOLGraphicsLibrary::RenderTarget* GetPointRenderTargets() const;
		 
	private:
		void GenerateShadowMaps(ResourceManager* resourceManager);

		void SetPipelines(ResourceManager* resourceManager);

		int GetTexture2DShadowSpace();

		bool ReturnTexture2DShadowSpace(int idx);

		//Point Light Using CubeMap.
		int GetCubeMapShadowSpace();

		bool ReturnCubeMapShadowSpace(int idx);

	private:
		std::unordered_map<uint64_t, std::unique_ptr<Light>> _lights;

		std::unique_ptr<CascadeShadow> _cascadeShadow;

		bool _directionalUsableSpace;

		std::vector<int> _spotUsableSpace;

		std::vector<int> _pointUsableSpace;

		DUOLGraphicsLibrary::Texture* _spotShadowMaps;

		DUOLGraphicsLibrary::RenderTarget* _spotRenderTargets;

		DUOLGraphicsLibrary::Texture* _pointLightShadowMaps;

		DUOLGraphicsLibrary::RenderTarget* _pointRenderTargets;

		DUOLGraphicsLibrary::PipelineState* _spotSkinned;

		DUOLGraphicsLibrary::PipelineState* _spotStatic;

		int _maxDirectionalShadowMapCount = 1;

		int _maxTextur2DShadowMapCount = 24;

		int _maxCubeShadowMapCount = 12;
	};


}
