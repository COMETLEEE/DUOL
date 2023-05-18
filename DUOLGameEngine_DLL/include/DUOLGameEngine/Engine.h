#pragma once
#include "DUOLGameEngine/Util/EngineSpecification.h"

namespace DUOLGameEngine
{
	/**
	 * \brief 게임 엔진. .exe로 노출되어 초기 설정 파일을 통해 초기화를 하고 Game Loop를 실시한다.
	 */
	class DUOL_GAMEENGINE_API Engine
	{
	public:
		Engine();

		~Engine();

	public:
		void Initialize(const EngineSpecification& gameSpecification);

		void UnInitialize();

	private:
		/**
		 * \brief DUOLEditor or DUOLGame 으로 부터 전달받은 실행 옵션
		 */
		EngineSpecification _engineSpec;

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
		 * \brief 게임 엔진 생애 주기에 따른 루프를 실시합니다. (Without rendering.)
		 */
		void Update();

		/**
		 * \brief 게임 엔진 생에 주기에 따른 루프를 실시합니다. (With rendering.)
		 */
		void UpdateGame();

		/**
		 * \brief 레벨 에디터에서 에디트 모드에 따른 루프를 실시합니다.
		 */
		void UpdateEditMode();

		/**
		 * \brief 레벨 에디터에서 퍼즈 모드에 따른 루프를 실시합니다.
		 */
		void UpdatePauseMode();

		/**
		 * \brief 레벨 에디터에서 프레임 모드에 따른 루프를 실시합니다.
		 * TODO : Scene, Physics, Debug 등에 대한 컨트롤을 직접 해주셔야합니다.
		 */
		void UpdateFrameMode();

		/**
		 * \brief 'DUOLGame.exe' 를 위한 Start rendering 함수입니다.
		 */
		void StartRenderingForGame();

		/**
		 * \brief 'DUOLGame.exe' 를 위한 End rendering 함수입니다.
		 */
		void EndRenderingForGame();

#pragma region FRIEND_CLASS
		friend class GraphicsManager;
#pragma endregion
	};
}