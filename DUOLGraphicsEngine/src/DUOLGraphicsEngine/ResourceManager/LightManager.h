#pragma once
#include <memory>
#include <unordered_map>
#include <vector>

#include "DUOLGraphicsEngine/ResourceManager/Resource/Light.h"

namespace DUOLGraphicsLibrary
{
	class Renderer;
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

		DUOLGraphicsLibrary::PipelineState* GetPointSkinnedPipeline() const;

		DUOLGraphicsLibrary::PipelineState* GetPointStaticPipeline() const;

		DUOLGraphicsLibrary::Texture* GetSpotShadowMaps() const;

		DUOLGraphicsLibrary::RenderTarget* GetSpotRenderTargets() const;

		DUOLGraphicsLibrary::Texture* GetPointLightShadowMaps() const;

		DUOLGraphicsLibrary::RenderTarget* GetPointRenderTargets() const;

		void ClearTexture2DRenderTarget(int idx, DUOLGraphicsLibrary::Renderer* renderer);

		void ClearCubeMapRenderTarget(int idx, DUOLGraphicsLibrary::Renderer* renderer);
		 
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

		bool _directionalUsableSpace;

		std::vector<int> _texture2DUsableSpace;

		std::vector<int> _cubeMapUsableSpace;

		std::vector<DUOLGraphicsLibrary::RenderTarget*> _cubeMapIndividualShadowRenderTargets;

		std::vector<DUOLGraphicsLibrary::RenderTarget*> _texture2DIndividualShadowRenderTargets;

		DUOLGraphicsLibrary::Texture* _texture2DShadowMaps;

		DUOLGraphicsLibrary::RenderTarget* _texture2DRenderTargets;

		DUOLGraphicsLibrary::Texture* _cubeMapLightShadowMaps;

		DUOLGraphicsLibrary::RenderTarget* _cubeMapRenderTargets;

		DUOLGraphicsLibrary::PipelineState* _spotSkinned;

		DUOLGraphicsLibrary::PipelineState* _spotStatic;

		DUOLGraphicsLibrary::PipelineState* _pointSkinned;

		DUOLGraphicsLibrary::PipelineState* _pointStatic;

		int _maxDirectionalShadowMapCount = 1;

		int _maxTextur2DShadowMapCount = 24;

		int _maxCubeShadowMapCount = 12;
	};


}
