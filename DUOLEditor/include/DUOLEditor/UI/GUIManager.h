/**

    @file      GUIManager.h
    @brief     GUI component's manager class.
    @details   ~
    @author    COMETLEE
    @date      15.12.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include "DUOLGameEngine/Util/SingletonBase.h"
#include "DUOLCommon/ImGui/imgui.h"

namespace DUOLEditor
{
	/**
	 * \brief Editor 실행 중 GUI의 생성, 그리기를 담당함.
	 */
	class GUIManager : public DUOLGameEngine::SingletonBase<GUIManager>
	{
		DECLARE_SINGLETON(GUIManager)

		DELETE_COPY_MOVE(GUIManager)

	private:
		virtual ~GUIManager() override;

	public:
		// ImGUI 프로그램의 스테이트. 일단 대충 넣어놓아유.
		bool showDemoWindow = true;

		bool showAnotherWindow = false;

		ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.6f, 1.f);

		bool isDeviceOpen = false;

	public:
		void Initialize(HWND hWnd);

		void UnInitialize();

		void OnResize(const uint32_t& screenWidth, const uint32_t& screenHeight);

		void Update(float deltaTime);
	};
}
