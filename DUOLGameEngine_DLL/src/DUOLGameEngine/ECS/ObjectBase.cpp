#include "DUOLGameEngine/ECS/ObjectBase.h"

#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"
#include "DUOLGameEngine/ECS/GameObject.h"

#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"

using namespace rttr;

RTTR_PLUGIN_REGISTRATION
{
	rttr::registration::class_<DUOLGameEngine::ObjectBase>("ObjectBase")
	.property("_name", &DUOLGameEngine::ObjectBase::_name)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)

	)
	.property("_isDontDestroyOnLoad", &DUOLGameEngine::ObjectBase::_isDontDestroyOnLoad)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("_uuid", &DUOLGameEngine::ObjectBase::_uuid)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.method("GetUUID", &DUOLGameEngine::ObjectBase::GetUUID)
	.method("GetName", &DUOLGameEngine::ObjectBase::GetName)
	.method("GetThis", &DUOLGameEngine::ObjectBase::GetThis);
}

namespace DUOLGameEngine
{
	ObjectBase::ObjectBase(const DUOLCommon::tstring& name, ObjectType objectType) :
		_isDontDestroyOnLoad(false)
		, _name(name)
		, _uuid(DUOLCommon::UUID())
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

		// TODO : SceneManager에서 무언가 할 수 있도록 ...
	}

	void ObjectBase::DestroyOnLoad(ObjectBase* target)
	{
		target->_isDontDestroyOnLoad = false;

		// TODO : SceneManager에서 무언가 할 수 있도록 ...
	}

	const DUOLCommon::tstring& ObjectBase::GetName() const
	{
		return _name;
	}

	void ObjectBase::SetName(const DUOLCommon::tstring& name)
	{
		_name = name;
	}

	const DUOLCommon::UUID& ObjectBase::GetUUID() const
	{
		return _uuid;
	}

	void* ObjectBase::GetThis()
	{
		return reinterpret_cast<void*>(this);
	}
}
