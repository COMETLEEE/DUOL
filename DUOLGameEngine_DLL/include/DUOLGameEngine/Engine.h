#pragma once
#include "DUOLGameEngine/Util/EngineSpecification.h"

namespace DUOLGameEngine
{
	/**
	 * \brief ���� ����. .exe�� ����Ǿ� �ʱ� ���� ������ ���� �ʱ�ȭ�� �ϰ� Game Loop�� �ǽ��Ѵ�.
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
		 * \brief DUOLEditor or DUOLGame ���� ���� ���޹��� ���� �ɼ�
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
		 * \brief ���� ���� ���� �ֱ⿡ ���� ������ �ǽ��մϴ�. (Without rendering.)
		 */
		void Update();

		/**
		 * \brief ���� ���� ���� �ֱ⿡ ���� ������ �ǽ��մϴ�. (With rendering.)
		 */
		void UpdateGame();

		/**
		 * \brief ���� �����Ϳ��� ����Ʈ ��忡 ���� ������ �ǽ��մϴ�.
		 */
		void UpdateEditMode();

		/**
		 * \brief ���� �����Ϳ��� ���� ��忡 ���� ������ �ǽ��մϴ�.
		 */
		void UpdatePauseMode();

		/**
		 * \brief ���� �����Ϳ��� ������ ��忡 ���� ������ �ǽ��մϴ�.
		 * TODO : Scene, Physics, Debug � ���� ��Ʈ���� ���� ���ּž��մϴ�.
		 */
		void UpdateFrameMode();

		/**
		 * \brief 'DUOLGame.exe' �� ���� Start rendering �Լ��Դϴ�.
		 */
		void StartRenderingForGame();

		/**
		 * \brief 'DUOLGame.exe' �� ���� End rendering �Լ��Դϴ�.
		 */
		void EndRenderingForGame();

#pragma region FRIEND_CLASS
		friend class GraphicsManager;
#pragma endregion
	};
}