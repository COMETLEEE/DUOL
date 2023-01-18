/**

    @file      GUIManager.h
    @brief     GUI component's manager class.
    @details   ~
    @author    COMETLEE
    @date      15.12.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include <unordered_map>

#include "DUOLGameEngine/Util/SingletonBase.h"
#include "DUOLCommon/ImGui/imgui.h"

namespace DUOLEditor
{
	struct Font;
	class Page;
}

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

		/**
		 * \brief .ttf 파일을 이용해 ImFont를 로드합니다.
		 * \param fontID 해당 폰트의 ID로 적용할 문자열
		 * \param path 파일 경로
		 * \param fontSize 폰트의 사이즈
		 * \return 성공하면 true, 실패하면 false
		 */
		bool LoadFont(const DUOLCommon::tstring& fontID, const DUOLCommon::tstring& path, float fontSize);

		/**
		 * \brief 폰트를 해제합니다.
		 * \param fontID 해제할 폰트의 ID
		 * \return 성공하면 true, 실패하면 false
		 */
		bool UnloadFont(const DUOLCommon::tstring& fontID);

		/**
		 * \brief 현재 폰트 컨텍스트를 해당 아이디의 폰트로 적용합니다.
		 * \param fontID 사용할 폰트의 ID
		 * \return 성공하면 true, 실패하면 false
		 */
		bool UseFont(const DUOLCommon::tstring& fontID);

		/**
		 * \brief 폰트를 기본 폰트로 적용합니다.
		 */
		void UseDefaultFont();

		/**
		 * \brief .ini generation to save user's editor layout.
		 * \param value true or false.
		 */
		void SetIsEditorLayoutSave(bool value);

		/**
		 * \brief layout save 기능이 켜져있는지에 대해 반환합니다.
		 * \return true if on layout save function.
		 */
		bool GetIsEditorLayoutSave() const;

		/**
		 * \brief set file name that saved layout.
		 * \param fileName that layout saved file name.
		 */
		void SetEditorLayoutSaveFileName(const DUOLCommon::tstring& fileName);

		/**
		 * \brief 레이아웃이 자동 세이브가 되는 주기를 조정합니다.
		 * \param frequency 초 단위의 주기
		 */
		void SetEditorLayoutAutosaveFrequency(float frequency);

		/**
		 * \brief 레이아웃 자동 세이브 주기를 반환합니다.
		 * \return 초 단위의 주기
		 */
		float GetEditorLayoutAutosaveFrequency() const;

		/**
		 * \brief Docking이 가능하도록 조정합니다.
		 * \param value true or false.
		 */
		void SetIsEnableDocking(bool value);

		/**
		 * \brief docking 가능 여부를 반환합니다.
		 * \return 호출 시점에 docking이 가능하면 true 아니면 false.
		 */
		bool GetIsEnableDocking() const;

		/**
		 * \brief 파일을 참조하여 layout을 해당 파일에 맞게 초기화합니다.
		 * \param config 참조할 파일의 이름
		 */
		void ResetLayout(const DUOLCommon::tstring& config) const;

		/**
		 * \brief 그림을 그리기 위한 페이지 객체를 주입합니다.
		 * \param page 그림 그릴 GUI Page.
		 */
		void SetPage(std::shared_ptr<DUOLEditor::Page> page);

		/**
		 * \brief 현재 그리기 위해 설정된 페이지의 주소를 반환합니다.
		 * \param tstringID 페이지의 ID
		 * \return 현재 설정된 페이지의 주소 
		 */
		DUOLEditor::Page* GetPage(const DUOLCommon::tstring& tstringID);

		/**
		 * \brief 현재 페이지를 리셋합니다.
		 */
		void RemovePage();

		/**
		 * \brief Render ImGui current frame.
		 * \param deltaTime deltaTime in application.
		 */
		void Update(float deltaTime);

		/**
		 * \brief Back Buffer을 세팅하고 사전 Update를 통해 기록된 GUI를 렌더링합니다.
		 */
		void RenderGUI();

	private:
		/**
		 * \brief Editor style 변경합니다.
		 */
		void ApplyImGuiStyle();

	private:
		std::shared_ptr<DUOLEditor::Page> _currentPage;

		bool _isDockingEnable;

		std::unordered_map<DUOLCommon::tstring, DUOLEditor::Font> _fonts;

		DUOLCommon::tstring _layoutSaveFileName = TEXT("imgui.ini");

#pragma region FRIEND_CLASS
		friend class Editor;
#pragma endregion
	};
}