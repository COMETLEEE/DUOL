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

		std::shared_ptr<DUOLEditor::GUIManager> _guiManager;

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
