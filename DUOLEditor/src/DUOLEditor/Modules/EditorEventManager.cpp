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

		// ��������� �ڽ� ������Ʈ�� ���� �̺�Ʈ�� �����մϴ�.
		auto&& childrens = gameObject->GetTransform()->GetAllChildGameObjects();

		for (auto& child : childrens)
			_createGameObjectEvent.Invoke(child);
	}

	void EditorEventManager::DeleteGameObject(DUOLGameEngine::GameObject* gameObject)
	{
		_deleteGameObjectEvent.Invoke(gameObject);

		// ��������� �ڽ� ������Ʈ�� ���� �̺�Ʈ�� �����մϴ�.
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
		// ����Ʈ ��忡�� ������ ��
		if (_editorMode == EditorMode::Edit)
		{
			// Play Event ..?

			// ������ ���� ���� �ø���������մϴ�. (����, ���̺갡 �� �Ǿ� ���� ���� �����ϱ� ..!)
			DUOLGameEngine::SceneManager::GetInstance()->SaveCurrentScene();
		}
		// �ƴ� ���
		else
		{
			// �״�� ��ŸƮ �ϴ°ǰ� ?
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

			// ���� ������Ʈ �� �����ú��� ..
			_gameObjectUnselectedEvent.Invoke();

			// ���� �� ���� �׷��� �����ɴϴ�. ���⼭ Scene Changed Event On..
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

		// �����͸� ���ݴϴ�.
		_editor->_isRunning = false;
	}

	void EditorEventManager::SaveLoadedFromFileScene()
	{
		DUOLGameEngine::SceneManager::GetInstance()->SaveCurrentScene();
	}

	void EditorEventManager::SaveAs()
	{
		// ���� Ž���� �����.

		// �׸��� �̸�, �� �� ������ �� �ְ� �ϰ� ���� ..!
	}

	void EditorEventManager::LoadEmptyScene()
	{
		// ����, ���� ������ ���� ���°� �ƴ϶�� �ƹ� �͵� ���� �ʴ´�.
		if (_editorMode != EditorMode::Edit)
			return;

		// ����, ���� ���� ���Ϸ� ���� �ҷ��� �Դٸ� ���� ������ ���� ���� ������ ��������.
		if (DUOLGameEngine::SceneManager::GetInstance()->GetIsCurrentSceneLoadedFromFile())
			SaveLoadedFromFileScene();

		DUOLGameEngine::SceneManager::GetInstance()->LoadEmptyScene();
	}

	void EditorEventManager::SaveScene()
	{
		// ����, ���� ������ ���� ���°� �ƴ϶�� �ƹ� �͵� ���� �ʴ´�.
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