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
	class Page;

	class GUIManager;
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

		void Initialize(DUOLGameEngine::Engine* gameEngine, DUOLGameEngine::EditorModeOption* editorModeOption);

	private:
		DUOLGameEngine::Engine* _gameEngine;

		/**
		 * \brief Application (== Editor + DUOLGameEngine) 이 가지고 있는 EditorModeOption의 Ref.
		 */
		DUOLGameEngine::EditorModeOption* _editorModeOption;

		/**
		 * \brief GUIManager 입니다. GUI 전체를 그리는 역할을 수행합니다.
		 */
		std::shared_ptr<DUOLEditor::GUIManager> _guiManager;

		/**
		 * \brief 에디터 전체를 나타내는 페이지입니다. 페이지가 한 장보다 더 많아지는 경우가 있을까요 ..?
		 */
		std::shared_ptr<DUOLEditor::Page> _editorPage;

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

	public:
		/**
		 * \brief Editor 기능의 모든 상태를 업데이트합니다.
		 * \param deltaTime 프레임 간격 시간
		 */
		void Update(float deltaTime);

	private:
		void PostUpdate(float deltaTime);

		void LateUpdate(float deltaTime);
	};
}
