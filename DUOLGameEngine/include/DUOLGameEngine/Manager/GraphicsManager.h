/**

    @file      GraphicsManager.h
    @brief     Work With Rendering Engine
    @details   ~
    @author    COMETLEE
    @date      17.10.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/

#pragma once
#include "DUOLGameEngine/Util/Defines.h"
#include "DUOLGameEngine/Util/SingletonBase.h"
#include "DUOLGameEngine/Util/EngineSpecification.h"

#include "DUOLCommon/Event/Event.h"

#include "DUOLGraphicsEngine/GraphicsEngine/GraphicsEngine.h"

namespace DUOLGameEngine
{
	struct RenderingPipelineSetup
	{
		std::vector<DUOLGraphicsEngine::RenderingPipeline*> _opaquePipelines;

		std::vector<DUOLGraphicsEngine::RenderingPipeline*> _transparencyPipelines;
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

		uint32_t _screenWidth;

		uint32_t _screenHeight;

	public:
		const uint32_t& GetScreenWidth() const { return _screenWidth; }

		const uint32_t& GetScreenHeight() const { return _screenHeight; }

	private:
		/**
		 * \brief Graphics API를 사용하기 위한 엔진 객체
		 */
		std::shared_ptr<DUOLGraphicsEngine::GraphicsEngine> _graphicsEngine;

		/**
		 * \brief perframe constant buffer를 반환합니다.
		 * \return 매 프레임 한 번만 보내주면 되는 상수 버퍼의 주소
		 */
		DUOLGraphicsEngine::ConstantBufferPerFrame* GetConstantBufferPerFrame();

	private:
		std::vector<DUOLGraphicsEngine::RenderObject*> _reservedRenderObjects;

		/**
		 * \brief Game View Pipeline states setup + @.
		 */
		std::vector<RenderingPipelineSetup> _renderingPipelineSetups;

		/**
		 * \brief Constant buffers for pipeline setup. (same index with RenderingPipeline setup)
		 */
		std::vector<DUOLGraphicsEngine::ConstantBufferPerFrame> _cbPerFrames;

		/**
		 * \brief Render Object (== Render Queue) lists to submit to graphics engine. (same index with RenderingPipeline setup)
		 */
		std::vector<std::vector<DUOLGraphicsEngine::RenderObject*>> _reservedRenderObjectLists;

	private:
		void ReserveRenderObject(DUOLGraphicsEngine::RenderObject& renderObjectInfo);

		void ReserveRenderDebugObject(DUOLGraphicsEngine::RenderObject& renderObjectInfo);

		int _currentRenderObjectListIndex;

	private:
		void UpdateConstantBufferPerFrame(int index, float deltaTime);

		void ClearConstantBufferPerFrame(int index);

		int _currentConstantBufferPerFrameIndex;

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
		 * \brief Execute 들을 수행하기 전에 모든 렌더 타겟들을 Clear 합니다.
		 */
		void PreUpdate();

		/**
		 * \brief 그래픽스 엔진 모듈을 사용하여 그림을 그립니다.
		 * \param deltaTime scaled frame time.
		 */
		void Update(float deltaTime);

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

		// TODO - 'DUOLGameEngine::EventManager' 로 빼버리자 ..
#pragma region GRAPHICS_MANAGER_EVENTS
	private:
		void OnResize(const uint32_t& screenWidth, const uint32_t& screenHeight);

		DUOLCommon::Event<void, const uint32_t&, const uint32_t&> _onResizeEvent;

		DUOLCommon::Event<void, const uint32_t&, const uint32_t&>& GetOnResizeEvent() { return _onResizeEvent; }

		void Render();

		DUOLCommon::Event<void> _renderEventHandlers;

		DUOLCommon::EventListenerID AddRenderEventHandler(std::function<void()> functor);

		bool RemoveRenderEventHandler(DUOLCommon::EventListenerID id);
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