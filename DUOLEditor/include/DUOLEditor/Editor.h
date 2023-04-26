/**

    @file      Editor.h
    @brief     DUOLEditor core class.
    @details   ~
    @author    COMETLEE
    @date      2.11.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include "DUOLGameEngine/Engine.h"

namespace DUOLEditor
{
	class PanelWindow;
	class Page;

	class ViewBase;

	class GUIManager;

	class EditorEventManager;
}

namespace DUOLEditor
{
	/**
	 * \brief Editor program core class.
	 */
	class Editor
	{
	public:
		Editor();

		~Editor();

		void Initialize(DUOLGameEngine::Engine* gameEngine);

	private:
		bool _isRunning;

		/**
		 * \brief GameEngine 입니다.
		 */
		DUOLGameEngine::Engine* _gameEngine;

		/**
		 * \brief EditorEventManager 입니다. 에디터에서 UI 의 상호 작용과 동작의 연결을 이벤트 방식으로 담당합니다.
		 */
		std::shared_ptr<DUOLEditor::EditorEventManager> _editorEventManager;

		/**
		 * \brief GUIManager 입니다. GUI 전체를 그리는 역할을 수행합니다.
		 */
		std::shared_ptr<DUOLEditor::GUIManager> _guiManager;

		/**
		 * \brief 에디터 전체를 나타내는 페이지입니다. 페이지가 한 장보다 더 많아지는 경우가 있을까요 ..?
		 */
		std::shared_ptr<DUOLEditor::Page> _editorPage;

		/**
		 * \brief 모든 View들의 list입니다. View 들은 Update가 필요합니다.
		 */
		std::list<DUOLEditor::ViewBase*> _views;

		/**
		 * \brief 저장하려는 Panel들의 List입니다. 
		 */
		std::unordered_map<DUOLCommon::tstring, DUOLEditor::PanelWindow*> _panels;

	private:
		/**
		 * \brief 페이지들을 만들고 시작 페이지를 GUIManager에 세팅합니다.
		 */
		void CreatePages();

		/**
		 * \brief 패널들을 만들고 각각의 페이지에 넣어줍니다.
		 */
		void CreatePanels();

		/**
		 * \brief 에디터에 사용되는 GUI 실체들을 생성하고 GUIManager에 넣습니다.
		 */
		void CreateEditorGUIs();

		void UpdateEngineCurrentEditorMode();

		void UpdateEnginePlayMode();

		void UpdateEnginePauseMode();

		void UpdateEngineEditMode();

		void UpdateEngineFrameMode();

		DUOLEditor::PanelWindow* GetPanel(const DUOLCommon::tstring& panelName);

	public:
		void PostUpdate(float deltaTime);

		void LateUpdate(float deltaTime);

#pragma region FRIEND_CLASS
		friend class EditorEventManager;

		friend class Application;
#pragma endregion
	};
}