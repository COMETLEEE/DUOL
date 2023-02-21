/**

    @file      EditorEventManager.h
    @brief     이벤트의 모든 feature에 대한 event managing을 담당하는 매니저 클래스
    @details   ~
    @author    COMETLEE
    @date      28.12.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include "DUOLGameEngine/Util/SingletonBase.h"

#include "DUOLCommon/Event/Event.h"

namespace DUOLGameEngine
{
	class Scene;
	class GameObject;
}

namespace DUOLEditor
{
	/**
	 * \brief 에디터의 현재 상황을 
	 */
	enum class EditorMode
	{
		// Stop Button
		Edit

		// Play Button
		, Play

		// Pause Button
		, Pause

		// Next Button
		, FRAME_BY_FRAME
	};

	/**
	 * \brief 에디터의 수행 기능에 따라서 발생해야 하는 각 이벤트에 대해서
	 * 여러 UI의 이벤트 리스너들을 이 매니저에 등록하고 호출합니다.
	 */
	class EditorEventManager : public DUOLGameEngine::SingletonBase<EditorEventManager>
	{
		DECLARE_SINGLETON(EditorEventManager)

		DELETE_COPY_MOVE(EditorEventManager)

	private:
		virtual ~EditorEventManager() override;

	private:
		/**
		 * \brief 현재 에디터가 어떤 모드에 있는지를 나타냅니다.
		 */
		EditorMode _editorMode;

	public:
#pragma region GAMEOBJECT_CONTROL
		void CreateGameObject(DUOLGameEngine::GameObject* gameObject);

		void DeleteGameObject(DUOLGameEngine::GameObject* gameObject);

		void SelectGameObject(DUOLGameEngine::GameObject* gameObject);

		void UnselectGameObject();
#pragma endregion

#pragma region EDITOR_STATE_CONTROL
		void SetEditorMode(DUOLEditor::EditorMode editorMode);

		void StartGame();

		void PauseGame();

		void StopGame();

		void NextFrame();
#pragma endregion

	public:
#pragma region EDITOR_ACTIONS
		DUOLCommon::Event<void, DUOLGameEngine::GameObject*>& GetGameObjectSelectedEvent();

		DUOLCommon::Event<void>& GetGameObjectUnselectedEvent();

		DUOLCommon::Event<void, DUOLGameEngine::GameObject*>& GetCreateGameObjectEvent();

		DUOLCommon::Event<void, DUOLGameEngine::GameObject*>& GetDeleteGameObjectEvent();

		DUOLCommon::Event<void, DUOLEditor::EditorMode>& GetEditorModeChangedEvent();

		DUOLCommon::Event<void, DUOLGameEngine::Scene*>& GetSceneChangedEvent();
#pragma endregion

	private:
		DUOLCommon::Event<void, DUOLGameEngine::GameObject*> _createGameObjectEvent;

		DUOLCommon::Event<void, DUOLGameEngine::GameObject*> _deleteGameObjectEvent;

		DUOLCommon::Event<void, DUOLGameEngine::GameObject*> _gameObjectSelectedEvent;

		DUOLCommon::Event<void> _gameObjectUnselectedEvent;

		DUOLCommon::Event<void, DUOLEditor::EditorMode> _editorModeChangedEvent;

		DUOLCommon::Event<void, DUOLGameEngine::Scene*> _sceneChangedEvent;

#pragma region FRIEND_CLASS
		friend class Editor;
#pragma endregion
	};
}
