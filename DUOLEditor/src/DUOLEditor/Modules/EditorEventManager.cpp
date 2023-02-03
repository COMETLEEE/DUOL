#include "DUOLEditor/Modules/EditorEventManager.h"

#include "DUOLGameEngine/ECS/GameObject.h"

namespace DUOLEditor
{
	EditorEventManager::EditorEventManager()
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
}