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
		// ��ϵ� �̺�Ʈ���� ��� ȣ���Ѵ�.
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