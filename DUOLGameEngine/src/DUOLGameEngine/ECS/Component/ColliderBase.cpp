#include "DUOLGameEngine/ECS/Component/ColliderBase.h"

#include "DUOLGameEngine/ECS/GameObject.h"

namespace DUOLGameEngine
{
	ColliderBase::ColliderBase(const std::weak_ptr<DUOLGameEngine::GameObject>& owner, const DUOLCommon::tstring& name) :
		DUOLGameEngine::BehaviourBase(owner, name)
		, _attachedRigidbody()
		, _isTrigger(false)
	{

	}

	ColliderBase::~ColliderBase()
	{
		_physicsActor.reset();
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

		// TODO : �̰� �ϱ� ���ؼ� Base�� shape pointer ���δ°� �´��� ������ ��
		_physicsShapeBase.lock()->SetTriggerEnable(value);
	}
}