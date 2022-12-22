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
		// 게임 오브젝트라면
		if (obj->_objectType == ObjectType::GameObject)
		{
			DUOLGameEngine::GameObject* gameObject = reinterpret_cast<DUOLGameEngine::GameObject*>(obj);

			DUOLGameEngine::Scene* currentScene = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene();

			if (currentScene != nullptr)
				currentScene->RegisterDestroyGameObject(gameObject, t);
		}
		else if (obj->_objectType == ObjectType::Component)
		{
			// TODO : Component manager (== GameObject) 가 할 수 있도록 구현해야합니다.
		}
	}

	void ObjectBase::DontDestroyOnLoad(ObjectBase* target)
	{
		target->_isDontDestroyOnLoad = true;

		// TODO : SceneManager의 특정한 동작 필요합니다.
	}

	void ObjectBase::DestroyOnLoad(ObjectBase* target)
	{
		target->_isDontDestroyOnLoad = false;

		// TODO : SceneManager의 특정한 동작 필요합니다.
	}
}
