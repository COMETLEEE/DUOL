#pragma once
#include <memory>
#include "DUOLGameEngine/Util/Defines.h"
#include "DUOLGameEngine/Util/EngineSpecification.h"

#include "DUOLMath/DUOLMath.h"
#include "DUOLCommon/Event/Event.h"
#include "Util/SingletonBase.h"

namespace DUOLGameEngine
{
	/**
	 * \brief 게임 엔진. .exe로 노출되어 초기 설정 파일을 통해 초기화를 하고 Game Loop를 실시한다.
	 */
	class Engine : public SingletonBase<Engine>
	{
		DECLARE_SINGLETON(Engine);

		DELETE_COPY_MOVE(Engine)

	private:
		virtual ~Engine() override;

	public:
		void Initialize(const EngineSpecification& gameSpecification);

		void UnInitialize();

	private:
		EngineSpecification _engineSpec;
		
#pragma region EVENTS
	private:
		/**
		 * \brief 'DUOLGame.exe' Window 의 크기가 변경되었을 때 호출할 이벤트
		 */
		void Resize(const uint32_t& screenWidth, const uint32_t& screenHeight);

		/**
		 * \brief Resize 이벤트에 호출되도록 등록된 이벤트 핸들러들의 모임
		 */
		DUOLCommon::Event<void, const uint32_t&, const uint32_t&> _resizeEvent;

		DUOLCommon::Event<void, const uint32_t&, const uint32_t&>& GetResizeEvent() { return _resizeEvent; }
#pragma endregion

	public:
		/**
		 * \brief Call from Game application's message handler. Keep game message handler unless this function returns TRUE.
		 * \param hWnd Window handle.
		 * \param msg message from platform handler.
		 * \param wParam W parameter.
		 * \param lParam L parameter.
		 * \return if it returns true, than program going on 
		 */
		bool DUOLGameEngine_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

		/**
		 * \brief 게임 엔진 생애 주기에 따른 루프를 실시합니다.
		 */
		void Update();

#pragma region FRIEND_CLASS
		friend class GraphicsManager;
#pragma endregion
	};
}
