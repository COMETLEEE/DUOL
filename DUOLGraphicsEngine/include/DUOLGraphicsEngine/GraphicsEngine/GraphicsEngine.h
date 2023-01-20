#pragma once
#include "DUOLGraphicsEngine/Export.h"
#include "DUOLGraphicsEngine/GraphicsEngineFlags.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/RenderConstantBuffer.h"

#include "DUOLCommon/StringHelper.h"
#include <memory>

#include "DUOLGraphicsEngine/ResourceManager/Resource/Material.h"

namespace DUOLGraphicsLibrary
{
	class RenderContext;
	class Buffer;
	class Renderer;
	class RenderTarget;
	struct RenderPass;
}

namespace DUOLGraphicsEngine
{
	class AnimationClip;
	class Model;
	class Material;
	class MeshBase;
	struct RenderObject;

	class ResourceManager;
	class RenderManager;

	/**
		@class   GraphicsEngine
		@brief   그래픽스 엔진
		@details ~
		@author  KyungMin Oh

	**/
	class DUOLGRAPHICSENGINE_EXPORT GraphicsEngine
	{
	public:
		GraphicsEngine(const GraphicsEngineDesc& engineDesc);

		~GraphicsEngine();

	private:
		DUOLGraphicsLibrary::Renderer* _renderer;

		DUOLGraphicsLibrary::RenderContext* _context;

		std::unique_ptr<ResourceManager> _resourceManager;

		std::unique_ptr<RenderManager> _renderManager;

		//for IMGUI
		std::unique_ptr<DUOLGraphicsLibrary::RenderPass> _backbufferRenderPass;

		//SkyBox... 어디론가 없애버려야한다
		DUOLGraphicsLibrary::Buffer* _skyboxVertex;

		DUOLGraphicsLibrary::Buffer* _skyboxIndex;

		DUOLGraphicsLibrary::Texture* _skyboxTexture;

		DUOLGraphicsLibrary::Texture* _skyboxPreFilteredTexture;

		DUOLGraphicsLibrary::Texture* _skyboxIrradianceTexture;

		DUOLGraphicsLibrary::Texture* _skyboxBRDFLookUpTexture;

		//Shadow........ 어디론가 없애버려야한다. ver.2

		DUOLGraphicsLibrary::Texture* _shadowMap;

		DUOLGraphicsLibrary::RenderTarget* _shadowMapDepth;

	private:
		void LoadRenderingPipelineTables(const DUOLMath::Vector2& screenSize);

		void ReadFromStaticMesh(MeshBase* const mesh, std::vector<DUOLMath::Vector3>& vertexInfo, std::vector<UINT32>& indexInfo);

		void ReadFromSkinnedMesh(MeshBase* const mesh, std::vector<DUOLMath::Vector3>& vertexInfo, std::vector<UINT32>& indexInfo);

		void Initialize();

		//SkyBox
		void CreateSkyBox();

		//Shadow
		void CreateCascadeShadow(int textureSize, int sliceCount);

		DUOLGraphicsLibrary::Texture* CreateCubeMapFromPanoramaImage(DUOLGraphicsLibrary::Texture* panorama);

		DUOLGraphicsLibrary::Texture* BakeIBLIrradianceMap(DUOLGraphicsLibrary::Texture* cubeMap, float width, float height);

		DUOLGraphicsLibrary::Texture* BakeIBLPreFilteredMap(DUOLGraphicsLibrary::Texture* cubeMap, int mipSize, float width, float height);

		DUOLGraphicsLibrary::Texture* BakeBRDFLookUpTable(float width, float height);

	public:
		DUOLGraphicsEngine::ModuleInfo GetModuleInfo();

		void RenderDebugObject(DUOLGraphicsEngine::RenderObject* object);

		void ClearRenderTargets();

		void ClearRenderTarget(DUOLGraphicsLibrary::RenderTarget& renderTarget);

		void ResizeRenderTarget(DUOLGraphicsLibrary::RenderTarget* renderTarget, const DUOLMath::Vector2& resolution);

		void Execute(const ConstantBufferPerFrame& perFrameInfo);

		void Execute(const std::vector<DUOLGraphicsEngine::RenderObject*>& renderObjects, const std::vector<RenderingPipelineLayout>& opaquePipelines, const std::vector<RenderingPipelineLayout>& transparencyPipelines, const ConstantBufferPerFrame& perFrameInfo);

		void Execute(const std::vector<DUOLGraphicsEngine::RenderObject*>& renderObjects, const std::vector<RenderingPipelineLayout>& opaquePipelines, RenderingPipeline* skyBoxPipeline, const std::vector<RenderingPipelineLayout>& transparencyPipelines, const ConstantBufferPerFrame& perFrameInfo);

		void PrePresent();

		void Present();

		void OnResize(const DUOLMath::Vector2& resolution);

		void CopyTexture(DUOLGraphicsLibrary::Texture* destTexture, DUOLGraphicsLibrary::Texture* srcTexture);

		uint64_t FastPicking(const DUOLMath::Vector2& pixel);

		//Resource
		void UpdateMesh(MeshBase* mesh, void* vertices, UINT vertexSize, void* indices, UINT indexSize);

		void SetDataName(std::vector<std::pair<uint64, DUOLCommon::tstring>>& materialname, std::vector<std::pair<uint64, DUOLCommon::tstring>>& animationname);

		//생성
		Model* CreateModelFromFBX(const DUOLCommon::tstring& objectID, std::pair<std::vector<uint64>, std::vector<uint64>>& modeldatas);

		MeshBase* CreateMesh(const DUOLCommon::tstring& objectID, void* vertices, UINT vertexSize, UINT vertexStructureSize, void* indices, UINT indexSize);

		Material* CreateMaterial(const DUOLCommon::tstring& objectID, DUOLGraphicsEngine::MaterialDesc& material);
		
		MeshBase* CreateParticle(const DUOLCommon::tstring&, int maxParticle, int emitterSize);

		//Read & Write
		Model* LoadModel(const DUOLCommon::tstring& objectID);

		bool ReadMeshInfo(const DUOLCommon::tstring& objectID, std::vector<DUOLMath::Vector3>& vertexInfo, std::vector<UINT32>& indexInfo);

		bool ReadMeshInfo(MeshBase* const mesh, std::vector<DUOLMath::Vector3>& vertexInfo, std::vector<UINT32>& indexInfo);

		MeshBase* LoadMesh(const DUOLCommon::tstring& objectID);

		Material* LoadMaterial(const DUOLCommon::tstring& objectID);

		AnimationClip* LoadAnimationClip(const DUOLCommon::tstring& objectID);

		DUOLGraphicsLibrary::Texture* LoadTexture(const DUOLCommon::tstring& objectID);

		RenderingPipeline* LoadRenderingPipeline(const DUOLCommon::tstring& objectID);

		//void LoadMeshTable(const DUOLCommon::tstring& path);
	};
}
