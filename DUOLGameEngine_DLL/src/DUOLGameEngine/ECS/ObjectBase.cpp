#include "DUOLGameEngine/ECS/ObjectBase.h"

#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"
#include "DUOLGameEngine/ECS/GameObject.h"

namespace DUOLGameEngine
{
	ObjectBase::ObjectBase(const DUOLCommon::tstring& name, ObjectType objectType) :
		_isDontDestroyOnLoad(false)
		, _name(name)
		, _uuid(UUID())
		, _objectType(objectType)
	{

	}

	ObjectBase::~ObjectBase()
	{

	}

	ObjectBase::operator bool()
	{
		return true;
	}

	bool ObjectBase::operator==(ObjectBase* other) const
	{
		return other == this ? true : false;
	}

	bool ObjectBase::operator!=(ObjectBase* other) const
	{
		return !(*this == other);
	}

	void ObjectBase::Destroy(ObjectBase* obj, float t)
	{
		// ���� ������Ʈ���
		if (obj->_objectType == ObjectType::GameObject)
		{
			DUOLGameEngine::GameObject* gameObject = reinterpret_cast<DUOLGameEngine::GameObject*>(obj);

			DUOLGameEngine::Scene* currentScene = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene();

			if (currentScene != nullptr)
				currentScene->RegisterDestroyGameObject(gameObject, t);
		}
		else if (obj->_objectType == ObjectType::Component)
		{
			DUOLGameEngine::ComponentBase* component = reinterpret_cast<DUOLGameEngine::ComponentBase*>(obj);

			DUOLGameEngine::GameObject* gameObject = component->GetGameObject();

			if (gameObject != nullptr)
				gameObject->RegisterDestroyComponent(component, t);
		}
	}

	void ObjectBase::DontDestroyOnLoad(ObjectBase* target)
	{
		target->_isDontDestroyOnLoad = true;

		// TODO : SceneManager�� Ư���� ���� �ʿ��մϴ�.
	}

	void ObjectBase::DestroyOnLoad(ObjectBase* target)
	{
		target->_isDontDestroyOnLoad = false;

		// TODO : SceneManager�� Ư���� ���� �ʿ��մϴ�.
	}

	const DUOLCommon::tstring& ObjectBase::GetName() const
	{
		return _name;
	}

	void ObjectBase::SetName(const DUOLCommon::tstring& name)
	{
		_name = name;
	}

	const UUID& ObjectBase::GetUUID() const
	{
		return _uuid;
	}
}