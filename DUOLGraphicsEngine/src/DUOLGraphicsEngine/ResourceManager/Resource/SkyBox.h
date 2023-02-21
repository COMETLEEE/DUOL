#pragma once
#include "DUOLCommon/StringHelper.h"

namespace DUOLGraphicsLibrary
{
	class Texture;
}

namespace DUOLGraphicsEngine
{
	class RenderingPipeline;
	class RenderManager;
	class MeshBase;
	class ResourceManager;
	class GraphicsEngine;

 /**

     @class   SkyBox
     @brief   Skybox를 관리하는 클래스입니다. 
     @details ~
     @author  KyungMin Oh

 **/
	class SkyBox
	{
	public:
		SkyBox(DUOLGraphicsEngine::GraphicsEngine* const graphicsEngine, const DUOLCommon::tstring& skyboxPath, DUOLGraphicsEngine::RenderingPipeline* skyboxPipeline);

	public:
		bool SetSkyboxTexture(const DUOLCommon::tstring& skyboxPath, DUOLGraphicsEngine::GraphicsEngine* const graphicsEngine);

	private:
		bool CreateSkyboxMesh(DUOLGraphicsEngine::ResourceManager* const resourceManager);

		DUOLGraphicsLibrary::Texture* CreateCubeMapFromPanoramaImage(DUOLGraphicsEngine::ResourceManager* const resourceManager, DUOLGraphicsEngine::RenderManager* const renderManager,DUOLGraphicsLibrary::Texture* panorama);

		DUOLGraphicsLibrary::Texture* BakeIBLIrradianceMap(DUOLGraphicsEngine::ResourceManager* const resourceManager, DUOLGraphicsEngine::RenderManager* const renderManager, DUOLGraphicsLibrary::Texture* cubeMap, float width, float height);

		DUOLGraphicsLibrary::Texture* BakeIBLPreFilteredMap(DUOLGraphicsEngine::ResourceManager* const resourceManager, DUOLGraphicsEngine::RenderManager* const renderManager, DUOLGraphicsLibrary::Texture* cubeMap, int mipSize, float width, float height);

		DUOLGraphicsLibrary::Texture* BakeBRDFLookUpTable(DUOLGraphicsEngine::ResourceManager* const resourceManager, DUOLGraphicsEngine::RenderManager* const renderManager, float width, float height);

	private:
		DUOLGraphicsLibrary::Texture* _skyboxTexture;

		DUOLGraphicsLibrary::Texture* _skyboxPreFilteredTexture;

		DUOLGraphicsLibrary::Texture* _skyboxIrradianceTexture;

		DUOLGraphicsLibrary::Texture* _skyboxBRDFLookUpTexture;

		DUOLGraphicsEngine::MeshBase* _skyboxSphereMesh;

		DUOLGraphicsEngine::RenderingPipeline* _skyboxPipeline;

	public:
		DUOLGraphicsEngine::RenderingPipeline* GetSkyboxPipeline() const
		{
			return _skyboxPipeline;
		}

		void SetSkyboxPipeline(DUOLGraphicsEngine::RenderingPipeline* skyboxPipeline)
		{
			_skyboxPipeline = skyboxPipeline;
		}

		DUOLGraphicsLibrary::Texture* GetSkyboxTexture() const
		{
			return _skyboxTexture;
		}

		DUOLGraphicsLibrary::Texture* GetSkyboxPreFilteredTexture() const
		{
			return _skyboxPreFilteredTexture;
		}

		DUOLGraphicsLibrary::Texture* GetSkyboxIrradianceTexture() const
		{
			return _skyboxIrradianceTexture;
		}

		DUOLGraphicsLibrary::Texture* GetSkyboxBRDFLookUpTexture() const
		{
			return _skyboxBRDFLookUpTexture;
		}

		DUOLGraphicsEngine::MeshBase* GetSkyboxSphereMesh() const
		{
			return _skyboxSphereMesh;
		}
	};
}
