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
			// DUOLGameEngine::SceneManager::GetInstance()->SaveCurrentScene();
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

			// ���� �� ���� �׷��� �����ɴϴ�.
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
