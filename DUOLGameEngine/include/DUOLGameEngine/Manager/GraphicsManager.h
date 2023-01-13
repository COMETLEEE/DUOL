/**

    @file      GraphicsManager.h
    @brief     Work With Rendering Engine
    @details   ~
    @author    COMETLEE
    @date      17.10.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/

#pragma once
#include <any>

#include "DUOLGameEngine/Util/Defines.h"
#include "DUOLGameEngine/Util/SingletonBase.h"
#include "DUOLGameEngine/Util/EngineSpecification.h"

#include "DUOLCommon/Event/Event.h"
#include "DUOLGameEngine/Util/UUID.h"

#include "DUOLGraphicsEngine/GraphicsEngine/GraphicsEngine.h"

namespace DUOLGameEngine
{
	struct RenderingPipelineSetup
	{
		std::vector<DUOLGraphicsEngine::RenderingPipeline*> _opaquePipelines = {};

		DUOLGraphicsEngine::RenderingPipeline*				_skyBoxPipeline = nullptr;

		std::vector<DUOLGraphicsEngine::RenderingPipeline*> _transparencyPipelines = {};

		DUOLCommon::tstring									_pipelineSetupName = TEXT("");
	};

	/**
	 * \brief 게임 진행 중 렌더링 엔진과 1대 1로 대응하는 매니저.
	 * Layer를 나눈다거나 .. Mask를 나눈다거나 등의 역할을 여기서 수행할 수 있을 것 같다.
	 */
	class GraphicsManager : public SingletonBase<GraphicsManager>
	{
		DECLARE_SINGLETON(GraphicsManager)

		DELETE_COPY_MOVE(GraphicsManager) 

	private:
		virtual ~GraphicsManager() override;

		DUOLMath::Vector2 _screenSize;

	public:
		const DUOLMath::Vector2& GetScreenSize() const { return _screenSize; }

		void SetScreenSize(const DUOLMath::Vector2& screenSize) { _screenSize = screenSize; }

	private:
		/**
		 * \brief Graphics API를 사용하기 위한 엔진 객체
		 */
		std::shared_ptr<DUOLGraphicsEngine::GraphicsEngine> _graphicsEngine;

		/**
		 * \brief Per-frame constant buffer를 반환합니다.
		 * \return 매 프레임 한 번만 보내주면 되는 상수 버퍼의 주소
		 */
		DUOLGraphicsEngine::ConstantBufferPerFrame* GetConstantBufferPerFrame();

	private:
		/**
		 * \brief Game View Pipeline states setup + @.
		 */
		std::vector<RenderingPipelineSetup> _renderingPipelineSetups;

		std::unordered_map<DUOLCommon::tstring, RenderingPipelineSetup> _pipelineSetups;

		std::vector<DUOLGraphicsEngine::RenderObject*> _renderObjectList;

		DUOLGraphicsEngine::ConstantBufferPerFrame _cbPerFrame;

	private:
		/**
		 * \brief 'Game' Setup 의 리스트에 쌓습니다.
		 * \param renderObjectInfo 그리기 위한 오브젝트의 정보
		 */
		void ReserveRenderObject(DUOLGraphicsEngine::RenderObject* renderObjectInfo);

		void ReserveRenderDebugObject(DUOLGraphicsEngine::RenderObject* renderObjectInfo);
		
	public:
		/**
		 * \brief Get Native shader resouce address. (wrapper for graphics engine api)
		 * \param id Shader resource's id.
		 * \return Native shader resource address.
		 */
		void* GetShaderResourceAddress(const DUOLCommon::tstring& id) const;

	public:
		void Initialize(const EngineSpecification& gameSpecification);

		void UnInitialize();
		
		/**
		 * \brief Initialize all pipeline setup for rendering.
		 * TODO - Setup 목록화 되어 있는 테이블을 로드하여 환경 별 필요한 파이프라인 셋업 자동화 셋팅.
		 */
		void InitializeGraphicsPipelineSetups();

	public:
		void* GetGraphicsDevice();

		void* GetGraphicsDeviceContext();

		/**
		 * \brief Swap Chain (== Back buffer) 에 렌더링할 준비를 합니다.
		 */
		void PrePresent();

		/**
		 * \brief 모든 렌더링을 마치고 Back Buffer와 Front Buffer를 스왑합니다.
		 */
		void Present();

	private:
		/**
		 * \brief invoked in 'Resize' event.
		 * \param screenSize 'DUOLMath::Vectror2*' type any.
		 */
		void OnResize(std::any screenSize);

		/**
		 * \brief 해당 파이프라인 셋업에서 사용하는 렌더 타겟을 클리어합니다.
		 * \param setupName 수행할 파이프라인 셋업의 이름
		 */
		void PreExecute(const DUOLCommon::tstring& setupName);

#pragma region OPEN_APIS
	public:
		/**
		 * \brief 그래픽스 엔진에 제출할 해당 setup name의 렌더 큐를 비웁니다.
		 */
		void ClearRenderObjectList();

		/**
		 * \brief Per-frame constant buffer context를 clear 합니다.
		 */
		void ClearConstantBufferPerFrame();

		/**
		 * \brief 모든 렌더 타겟을 청소합니다.
		 */
		void ClearAllRenderTarget();

		/**
		 * \brief 현재 씬을 구성하는 오브젝트들을 사용해 (Attached 'RendererBase' component.) 렌더 큐를 채웁니다.
		 */
		void RenderCurrentScene();

		/**
		 * \brief 이번 Execute에서 렌더링할 결과물의 스크린 사이즈를 조정합니다.
		 * \param screenSize 그림을 그릴 스크린 사이즈
		 */
		void UpdateRenderScreenSize(const DUOLMath::Vector2& screenSize);

		/**
		 * \brief 이번 Execute에서 렌더링할 때 사용할 카메라 정보를 조정합니다.
		 * \param cameraInfo 이번에 적용할 카메라 정보
		 */
		void UpdateCameraInfo(const DUOLGraphicsEngine::Camera* cameraInfo);

		/**
		 * \brief 현재 그래픽 컨텍스트를 참조하여 제출받은 이름에 맞는 셋업을 수행합니다.
		 * \param setupName 수행할 파이프라인 셋업의 이름
		 * \param cleanContext Execute 후 context clear 여부
		 */
		void Execute(const DUOLCommon::tstring& setupName, bool cleanContext = false);

		/**
		 * \brief 텍스쳐를 복사합니다. (같은 해상도의 텍스처만 온전하게 복사됩니다.)
		 * \param destTextureID 복사 '받을' Texture의 이름
		 * \param srcTextureID 복사 '할' Texture의 이름
		 */
		void CopyTexture(const DUOLCommon::tstring& destTextureID, const DUOLCommon::tstring& srcTextureID);

		/**
		 * \brief GPU를 이용해 빠른 Picking을 수행합니다.
		 * \param pixel 선택한 픽셀
		 * \return 해당 픽셀에 지정된 UUID
		 */
		DUOLGameEngine::UUID FastPicking(const DUOLMath::Vector2& pixel);

		/**
		 * \brief 'DUOLGame.exe' 를 위한 Start rendering 함수입니다.
		 */
		void StartRenderingForGame();

		/**
		 * \brief 'DUOLGame.exe' 를 위한 End rendering 함수입니다.
		 */
		void EndRenderingForGame();
#pragma endregion




#pragma region FRIEND_CLASS
		friend class Light;

		friend class Camera;
		
		friend class Engine;

		friend class RendererBase;

		friend class MeshRenderer;

		friend class SkinnedMeshRenderer;
		
		friend class ParticleRenderer;

		friend class PhysicsManager;

		friend class DebugManager;
#pragma endregion
	};
}