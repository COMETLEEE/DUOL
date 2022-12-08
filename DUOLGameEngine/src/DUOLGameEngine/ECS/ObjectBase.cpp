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
		// SceneManager �� ObjectManager�� �Բ� �����ؾ��մϴ�.


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
}