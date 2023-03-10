#pragma once
#include "DUOLGraphicsEngine/Export.h"
#include "DUOLGraphicsEngine/GraphicsEngineFlags.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/RenderConstantBuffer.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/RenderObject.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/Material.h"
#include "DUOLGraphicsLibrary/FontEngine/IFontEngine.h"

#include "DUOLCommon/StringHelper.h"

#include <vector>
#include <memory>

namespace DUOLGraphicsLibrary
{
	class RenderContext;
	class Buffer;
	class Renderer;
	class RenderTarget;
	struct RenderPass;
	struct QueryInfo;
}

namespace DUOLGraphicsEngine
{
	class CascadeShadow;
	class OcclusionCulling;
	class AnimationClip;
	class Model;
	class Material;
	class MeshBase;
	class ResourceManager;
	class RenderManager;
	class SkyBox;

	struct RenderObject;

	//todo 
	struct Temp
	{
		


	};

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

		DUOLGraphicsLibrary::IFontEngine* _fontEngine;

		std::unique_ptr<ResourceManager> _resourceManager;

		std::unique_ptr<RenderManager> _renderManager;

		std::unique_ptr<SkyBox> _skyBox;

		std::unique_ptr<OcclusionCulling> _occlusionCulling;

		std::unique_ptr<CascadeShadow> _cascadeShadow;

	private:
		//for IMGUI
		std::unique_ptr<DUOLGraphicsLibrary::RenderPass> _backbufferRenderPass;

		//렌더큐들...
		std::vector<DecomposedRenderData> _opaqueOccluderRenderQueue;

		std::vector<DecomposedRenderData> _opaqueRenderQueue;

		std::vector<DecomposedRenderData> _opaqueOccludeCulledRenderQueue;

		std::vector<DecomposedRenderData> _transparencyRenderQueue;

		std::vector<DecomposedRenderData> _debugRenderQueue;

		//todo:: 인스턴싱을 위해 렌더큐를 아래와 같이 바꿔주어야한다.. ^^7 
		//std::unordered_map<MeshBase*, std::unordered_map<Material*, RenderInstancingData>> _instancingDataHolder;


	private:
		void LoadRenderingPipelineTables(const DUOLMath::Vector2& screenSize);

		void ReadFromStaticMesh(MeshBase* const mesh, std::vector<DUOLMath::Vector3>& vertexInfo, std::vector<UINT32>& indexInfo);

		void ReadFromSkinnedMesh(MeshBase* const mesh, std::vector<DUOLMath::Vector3>& vertexInfo, std::vector<UINT32>& indexInfo);

		void Initialize();

		//SkyBox
		void CreateSkyBox();

		//Occlusion
		void CreateOcclusionCulling();

		//Shadow
		void CreateCascadeShadow(int textureSize, int sliceCount);

		void RegistRenderQueue(const std::vector<RenderObject*>& renderObjects, const Camera& cameraInfo);

	public:
		ResourceManager* GetResourceManager() const
		{
			return _resourceManager.get();
		}

		RenderManager* GetRenderManager() const
		{
			return _renderManager.get();
		}

		DUOLGraphicsLibrary::Renderer* GetRenderer() const
		{
			return _renderer;
		}

		DUOLGraphicsEngine::ModuleInfo GetModuleInfo();

	public:
		void RenderDebugObject(RenderObject* render_object);

		void ClearRenderTargets();

		void ClearRenderTarget(DUOLGraphicsLibrary::RenderTarget* renderTarget);
		
		void ResizeTexture(DUOLGraphicsLibrary::Texture* texture, const DUOLMath::Vector2& resolution);

		void Execute(
		const std::vector<DUOLGraphicsEngine::RenderObject*>& renderObjects,
			const std::vector<RenderingPipelineLayout>& opaquePipelines,
			RenderingPipeline* skyBoxPipeline,
			const std::vector<RenderingPipelineLayout>& transparencyPipelines,
			const ConstantBufferPerFrame& perFrameInfo,
			const std::vector<DUOLGraphicsLibrary::ICanvas*>& canvases);

		void Execute(
			const std::vector<DUOLGraphicsEngine::RenderObject*>& renderObjects,
			const std::vector<RenderingPipelinesList>& renderPipelinesList,
			const std::vector<DUOLGraphicsLibrary::ICanvas*>& canvases,
			const ConstantBufferPerFrame& perFrameInfo);

		void Begin();

		void End();

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

		DUOLGraphicsLibrary::Texture* CreateTexture(const DUOLCommon::tstring& objectID, float width, float height, int size, void* initialData);

		DUOLGraphicsLibrary::Texture* CreateTexture(const DUOLCommon::tstring& objectID);

		MeshBase* CreateParticle(const DUOLCommon::tstring&, int maxParticle, int emitterSize);

		DUOLGraphicsLibrary::ICanvas* CreateCanvas(DUOLGraphicsLibrary::CanvasRenderMode rendertype, const DUOLCommon::tstring& canvasName= _T(""), DUOLGraphicsLibrary::Texture* const texture = nullptr);

		DUOLGraphicsLibrary::IFont* CreateIFont(const DUOLCommon::tstring& fontName);
		
		//Read & Write & Getter
		Model* LoadModel(const DUOLCommon::tstring& objectID);

		bool ReadMeshInfo(const DUOLCommon::tstring& objectID, std::vector<DUOLMath::Vector3>& vertexInfo, std::vector<UINT32>& indexInfo);

		bool ReadMeshInfo(MeshBase* const mesh, std::vector<DUOLMath::Vector3>& vertexInfo, std::vector<UINT32>& indexInfo);

		MeshBase* LoadMesh(const DUOLCommon::tstring& objectID);

		Material* LoadMaterial(const DUOLCommon::tstring& objectID);

		AnimationClip* LoadAnimationClip(const DUOLCommon::tstring& objectID);

		AnimationClip* LoadAnimationClip(int index);

		int LoadAnimationClipSize();

		DUOLGraphicsLibrary::Texture* LoadTexture(const DUOLCommon::tstring& objectID);

		RenderingPipeline* LoadRenderingPipeline(const DUOLCommon::tstring& objectID);

		DUOLGraphicsLibrary::PipelineState* LoadPipelineState(const DUOLCommon::tstring& objectID);

		DUOLGraphicsLibrary::IFont* GetFont(const DUOLCommon::tstring& fontPath);

		bool GetRenderData(DUOLGraphicsLibrary::QueryInfo& outData);

		//void LoadMeshTable(const DUOLCommon::tstring& path);
	};
}
