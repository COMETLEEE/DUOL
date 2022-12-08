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
		// TODO
		// SceneManager 및 ObjectManager와 함께 동작해야합니다.


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