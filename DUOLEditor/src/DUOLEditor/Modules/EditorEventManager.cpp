#include "DUOLEditor/Modules/EditorEventManager.h"

namespace DUOLEditor
{
	EditorEventManager::EditorEventManager()
	{
		
	}

	EditorEventManager::~EditorEventManager()
	{
	}

	void EditorEventManager::SelectGameObject(DUOLGameEngine::GameObject* gameObject)
	{
		// 등록된 이벤트들을 모두 호출한다.
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
}