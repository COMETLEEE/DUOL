#include "DUOLGameEngine/ECS/ObjectBase.h"

namespace DUOLGameEngine
{
	ObjectBase::ObjectBase(const DUOLCommon::tstring& name) :
		_isDontDestroyOnLoad(false)
		, _name(name)
		, _uuid(UUID())
	{

	}

	ObjectBase::~ObjectBase()
	{

	}

	ObjectBase::operator bool()
	{
		return true;
	}

	bool ObjectBase::operator==(std::shared_ptr<ObjectBase> other) const
	{
		return other.get() == this ? true : false;
	}

	bool ObjectBase::operator!=(std::shared_ptr<ObjectBase> other) const
	{
		return !(*this == other);
	}

	void ObjectBase::Destroy(std::shared_ptr<ObjectBase> obj, float t)
	{
		// TODO
		// SceneManager 및 ObjectManager와 함께 동작해야합니다.


	}

	void ObjectBase::DontDestroyOnLoad(std::shared_ptr<ObjectBase> target)
	{
		target->_isDontDestroyOnLoad = true;
	}

	void ObjectBase::DestroyOnLoad(std::shared_ptr<ObjectBase> target)
	{
		target->_isDontDestroyOnLoad = false;
	}
}