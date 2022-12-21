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
		 * \brief 매 프레임 한 번, 그래픽스 엔진에게 보내주어야 하는 정보
		 */
		DUOLGraphicsEngine::ConstantBufferPerFrame _cbPerFrame;

	private:
		std::vector<DUOLGraphicsEngine::RenderObject*> _reservedRenderObjects;

	private:
		void ReserveRenderObject(DUOLGraphicsEngine::RenderObject& renderObjectInfo);

		void ReserveRenderDebugObject(DUOLGraphicsEngine::RenderObject& renderObjectInfo);

	private:
		void UpdateConstantBufferPerFrame();

	public:
		void Initialize(const EngineSpecification& gameSpecification);

		void UnInitialize();

		/**
		 * \brief 그래픽스 엔진 모듈을 사용하여 그림을 그립니다.
		 * \param deltaTime Scaled deltatime.
		 */
		void Update(float deltaTime);
#pragma region GRAPHICS_EVENTS
	private:
		void OnResize(const uint32_t& screenWidth, const uint32_t& screenHeight);

		DUOLCommon::Event<void, const uint32_t&, const uint32_t&> _onResizeEvent;

		DUOLCommon::Event<void, const uint32_t&, const uint32_t&>& GetOnResizeEvent() { return _onResizeEvent; }

		void Render();

		DUOLCommon::Event<void> _renderEventHandlers;

		DUOLCommon::EventHandlerID AddRenderEventHandler(std::function<void()> functor);

		bool RemoveRenderEventHandler(DUOLCommon::EventHandlerID id);
#pragma endregion

#pragma region FRIEND_CLASS
		friend class Camera;
		
		friend class Engine;

		friend class RendererBase;

		friend class MeshRenderer;

		friend class SkinnedMeshRenderer;

		friend class PhysicsManager;

		friend class DebugManager;
#pragma endregion
	};
}