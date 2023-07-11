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
	class InstancingManager;
	struct Frustum;
	class OrderIndependentTransparencyRenderer;
	class LightManager;
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

		std::unique_ptr<InstancingManager> _instancingManager;

		std::unique_ptr<SkyBox> _skyBox;

		std::unique_ptr<LightManager> _lightManager;

		std::unique_ptr<OcclusionCulling> _occlusionCulling;

		std::unique_ptr<CascadeShadow> _cascadeShadow;

		std::unique_ptr<OrderIndependentTransparencyRenderer> _oitRenderer;

	private:
		std::unique_ptr<DUOLGraphicsLibrary::RenderPass> _backbufferRenderPass;

		RenderingPipeline* _drawCanvasOnGameViewPipeline;

		RenderingPipeline* _drawGameViewOnBakcBufferPipeline;

		/**
		 * \brief Occludee인 오브젝트들입니다.
		 */
		std::vector<RenderObject*> _opaqueOccluderObjects;

		/**
		 * \brief Occludee가 아닌 오브젝트들입니다.
		 */
		std::vector<RenderObject*> _opaqueRenderObjects;

		/**
		 * \brief Occludee 오브젝트중 컬링 "완료"
		 */
		std::vector<RenderObject*> _opaqueCulledRenderObjects;

		std::vector<DecomposedRenderData> _opaqueOccluderRenderQueue;

		std::vector<DecomposedRenderData> _opaqueCulledRenderQueue;

		std::vector<DecomposedRenderData> _transparencyRenderQueue;

		std::vector<DecomposedRenderData> _debugRenderQueue;

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
		void CreateLightManager(int textureSize, int sliceCount);

		//OIT Renderer
		void CreateRenderers(const DUOLMath::Vector2& screenSize);

		void RegistBackBufferRenderPass();

		void RegistTexturesInLightPass();

		void RegistRenderQueue(const std::vector<RenderObject*>& renderObjects, const Frustum& cameraFrustum, const RenderingPipelinesList&);

		void RegistLight(Light* const *  lights, int lightCount, const Frustum& cameraFrustum, ConstantBufferPerCamera& perCamera, Light** currentSceneLight);

		void PrepareBakeShadows(const ConstantBufferPerFrame& perFrame, ConstantBufferPerCamera& perCamera, Light** currentSceneLight);

		void BakeShadows(const ConstantBufferPerFrame& perFrame, ConstantBufferPerCamera& perCamera, Light** currentSceneLight, const std::
		                 vector<DUOLGraphicsEngine::RenderObject*>& renderObjects);

		void DecomposeRenderObject(const std::vector<RenderObject*>& renderObjects, std::vector<DecomposedRenderData>& opaqueRenderData, std::vector<DecomposedRenderData>& transparencyRenderData);

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
			const std::vector<RenderingPipelinesList>& renderPipelinesList,
			const std::vector<DUOLGraphicsLibrary::ICanvas*>& canvases,
			const CurrentSceneInfo& currentSceneInfo);

		void RunProcedure(const RenderingPipelineLayout& layout, const std::vector<DecomposedRenderData>& renderQueue);

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
		Model* CreateModelFromFBX(const DUOLCommon::tstring& objectID);

		/**
		 * \brief 멀티스레드로 텍스쳐를 로드하기 위한 함수입니다. 모델들을 이함수로 모두 호출후 LoadTextureWithMultiThread를 호출해야합니다.
		 * \param objectID 
		 * \return 
		 */
		Model* CreateModelFromFBXWithMultiThread(const DUOLCommon::tstring& objectID);

		void LoadTexturesWithMultiThread();

		MeshBase* CreateMesh(const DUOLCommon::tstring& objectID, void* vertices, UINT vertexSize, UINT vertexStructureSize, void* indices, UINT indexSize);

		Material* CreateMaterial(const DUOLCommon::tstring& objectID, DUOLGraphicsEngine::MaterialDesc& material);

		DUOLGraphicsLibrary::Texture* CreateTexture(const DUOLCommon::tstring& objectID, float width, float height, int size, void* initialData);

		DUOLGraphicsLibrary::Texture* CreateTexture(const DUOLCommon::tstring& objectID, float width, float height, int size, DUOLGraphicsLibrary::ResourceFormat format, void* initialData);

		DUOLGraphicsLibrary::Texture* CreateTexture(const DUOLCommon::tstring& objectID);

		MeshBase* CreateParticle(const DUOLCommon::tstring&, int maxParticle);

  /**
      @brief  
      @param  rendertype - rendertype이 backbuffer 아래의 파라미터는 필요하지 않습니다.
      @param  canvasName - 
      @param  width    - 
      @retval            - 
  **/
		DUOLGraphicsLibrary::ICanvas* CreateCanvas(DUOLGraphicsLibrary::CanvasRenderMode rendertype, const DUOLCommon::tstring& canvasName= _T(""), int width = 0, int height = 0);

		DUOLGraphicsLibrary::IFont* CreateIFont(const DUOLCommon::tstring& fontName);

		DUOLGraphicsEngine::Light* CreateLight(const uint64_t& lightId);

		bool DeleteLight(const uint64_t& lightId);

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

		bool DeleteCanvas(const DUOLCommon::tstring& canvasName);
		//void LoadMeshTable(const DUOLCommon::tstring& path);
	};
}

