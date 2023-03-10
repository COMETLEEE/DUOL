#include "DUOLEditor/Modules/EditorEventManager.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/Manager/SerializeManager.h"
#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"

#include "DUOLEditor/Editor.h"

namespace DUOLEditor
{
	EditorEventManager::EditorEventManager() :
		_editorMode(EditorMode::Edit)
	{
		DUOLGameEngine::EventManager::GetInstance()->AddEventFunction(TEXT("SceneChanging"), [this]()
			{
				DUOLGameEngine::Scene* currentScene = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene();

				_sceneChangedEvent.Invoke(currentScene);
			});
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
			DUOLGameEngine::SceneManager::GetInstance()->SaveCurrentScene();
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

			// 현재 씬 파일 그래도 가져옵니다. 여기서 Scene Changed Event On..
			DUOLGameEngine::SceneManager::GetInstance()->LoadSceneFile(currentSceneName);
		}

		SetEditorMode(EditorMode::Edit);
	}

	void EditorEventManager::NextFrame()
	{
		if (_editorMode == EditorMode::Play || _editorMode == EditorMode::Pause)
			SetEditorMode(EditorMode::FRAME_BY_FRAME);
	}

	void EditorEventManager::ExitEditor()
	{
		if (_editorMode != EditorMode::Edit)
			SetEditorMode(EditorMode::Edit);

		// 에디터를 꺼줍니다.
		_editor->_isRunning = false;
	}

	void EditorEventManager::SaveLoadedFromFileScene()
	{
		DUOLGameEngine::SceneManager::GetInstance()->SaveCurrentScene();
	}

	void EditorEventManager::SaveAs()
	{
		// 파일 탐색기 열어라.

		// 그리고 이름, 뭐 등 지정할 수 있게 하고 저장 ..!
	}

	void EditorEventManager::LoadEmptyScene()
	{
		// 만약, 현재 에디팅 중인 상태가 아니라면 아무 것도 하지 않는다.
		if (_editorMode != EditorMode::Edit)
			return;

		// 만약, 현재 씬이 파일로 부터 불려졍 왔다면 수정 사항이 있을 수도 있으니 저장하자.
		if (DUOLGameEngine::SceneManager::GetInstance()->GetIsCurrentSceneLoadedFromFile())
			SaveLoadedFromFileScene();

		DUOLGameEngine::SceneManager::GetInstance()->LoadEmptyScene();
	}

	void EditorEventManager::SaveScene()
	{
		// 만약, 현재 에디팅 중인 상태가 아니라면 아무 것도 하지 않는다.
		if (_editorMode != EditorMode::Edit)
			return;

		if (DUOLGameEngine::SceneManager::GetInstance()->GetIsCurrentSceneLoadedFromFile())
		{
			SaveLoadedFromFileScene();
		}
		else
		{
			SaveAs();
		}
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