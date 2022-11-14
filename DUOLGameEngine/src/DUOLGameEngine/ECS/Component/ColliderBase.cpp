#include "DUOLGameEngine/ECS/Component/ColliderBase.h"

namespace DUOLGameEngine
{
	ColliderBase::ColliderBase(const std::weak_ptr<DUOLGameEngine::GameObject>& owner, const DUOLCommon::tstring& name) :
		DUOLGameEngine::BehaviourBase(owner, name)
	{
		 
	}

	ColliderBase::~ColliderBase()
	{

	}

	void ColliderBase::SetAttachedRigidbody(const std::weak_ptr<DUOLGameEngine::Rigidbody>& rigidbody)
	{
		_attachedRigidbody = rigidbody;

		// Rigidbody�� �ְ� .. ���� ���� ������ �Ǿ���� �� ������ ..?
	}

	void ColliderBase::SetIsTrigger(bool value)
	{
		// Ʈ���� ���ο� ���ؼ� Scene�� ��ϵǴ� �Ӽ��� �޶����� ..?
		if (_isTrigger == value)
			return;

		_isTrigger = value;

	}
}