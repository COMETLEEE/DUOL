#include "DUOLEditor/Modules/EditorEventManager.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/Manager/SerializeManager.h"
#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"

namespace DUOLEditor
{
	EditorEventManager::EditorEventManager() :
		_editorMode(EditorMode::Edit)
	{
		
	}

	EditorEventManager::~EditorEventManager()
	{
	}

	void EditorEventManager::CreateGameObject(DUOLGameEngine::GameObject* gameObject)
	{
		_createGameObjectEvent.Invoke(gameObject);

		// 재귀적으로 자식 오브젝트에 대한 이벤트도 실행합니다.
		auto&& childrens = gameObject->GetTransform()->GetAllChildGameObjects();

		for (auto& child : childrens)
			_createGameObjectEvent.Invoke(child);
	}

	void EditorEventManager::DeleteGameObject(DUOLGameEngine::GameObject* gameObject)
	{
		_deleteGameObjectEvent.Invoke(gameObject);

		// 재귀적으로 자식 오브젝트에 대한 이벤트도 실행합니다.
		auto&& childrens = gameObject->GetTransform()->GetAllChildGameObjects();

		for (auto& child : childrens)
			_deleteGameObjectEvent.Invoke(child);
	}

	void EditorEventManager::SelectGameObject(DUOLGameEngine::GameObject* gameObject)
	{
		_gameObjectSelectedEvent.Invoke(gameObject);
	}

	void EditorEventManager::UnselectGameObject()
	{
		_gameObjectUnselectedEvent.Invoke();
	}

	void EditorEventManager::SetEditorMode(DUOLEditor::EditorMode editorMode)
	{
		_editorMode = editorMode;
		_editorModeChangedEvent.Invoke(_editorMode);
	}

	void EditorEventManager::StartGame()
	{
		// 에디트 모드에서 실행할 때
		if (_editorMode == EditorMode::Edit)
		{
			// Play Event ..?

			// 강제로 현재 씬을 시리얼라이즈합니다. (저장, 세이브가 안 되어 있을 수도 있으니까 ..!)
			// DUOLGameEngine::SceneManager::GetInstance()->SaveCurrentScene();
		}
		// 아닌 경우
		else
		{
			// 그대로 스타트 하는건가 ?
		}

		SetEditorMode(EditorMode::Play);
	}

	void EditorEventManager::PauseGame()
	{
		SetEditorMode(EditorMode::Pause);
	}

	void EditorEventManager::StopGame()
	{
		if (_editorMode != EditorMode::Edit)
		{
			DUOLGameEngine::Scene* currentScene = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene();

			const DUOLCommon::tstring&	currentSceneName = currentScene->GetName();

			// 게임 오브젝트 언 셀렉팅부터 ..
			_gameObjectUnselectedEvent.Invoke();

			// 현재 씬 파일 그래도 가져옵니다.
			_sceneChangedEvent.Invoke(DUOLGameEngine::SceneManager::GetInstance()->LoadSceneFile(currentSceneName));
		}

		SetEditorMode(EditorMode::Edit);
	}

	void EditorEventManager::NextFrame()
	{
		if (_editorMode == EditorMode::Play || _editorMode == EditorMode::Pause)
			SetEditorMode(EditorMode::FRAME_BY_FRAME);
	}

	DUOLCommon::Event<void, DUOLGameEngine::GameObject*>& EditorEventManager::GetGameObjectSelectedEvent()
	{
		return _gameObjectSelectedEvent;
	}

	DUOLCommon::Event<void>& EditorEventManager::GetGameObjectUnselectedEvent()
	{
		return _gameObjectUnselectedEvent;
	}

	DUOLCommon::Event<void, DUOLGameEngine::GameObject*>& EditorEventManager::GetCreateGameObjectEvent()
	{
		return _createGameObjectEvent;
	}

	DUOLCommon::Event<void, DUOLGameEngine::GameObject*>& EditorEventManager::GetDeleteGameObjectEvent()
	{
		return _deleteGameObjectEvent;
	}

	DUOLCommon::Event<void, DUOLEditor::EditorMode>& EditorEventManager::GetEditorModeChangedEvent()
	{
		return _editorModeChangedEvent;
	}

	DUOLCommon::Event<void, DUOLGameEngine::Scene*>& EditorEventManager::GetSceneChangedEvent()
	{
		return _sceneChangedEvent;
	}
}
