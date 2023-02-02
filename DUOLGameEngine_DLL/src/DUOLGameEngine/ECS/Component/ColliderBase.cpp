#include "DUOLGameEngine/ECS/Component/ColliderBase.h"

#include "DUOLGameEngine/Manager/PhysicsManager.h"
#include "DUOLGameEngine/ECS/GameObject.h"

#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"

using namespace rttr;

RTTR_PLUGIN_REGISTRATION
{
	rttr::registration::class_<DUOLGameEngine::ColliderBase>("ColliderBase")
	.constructor<const std::weak_ptr<DUOLGameEngine::GameObject>&, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	);
}

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
		if (!_physicsShapeBase.expired())
			_physicsShapeBase.lock()->Release();

		_physicsShapeBase.reset();

		_physicsActor.reset();
	}

	Rigidbody* ColliderBase::GetAttachedRigidbody() const
	{
		return _attachedRigidbody;
	}

	void ColliderBase::SetAttachedRigidbody(DUOLGameEngine::Rigidbody* rigidbody)
	{
		_attachedRigidbody = rigidbody;

		// Rigidbody�� �ְ� .. ���� ���� ������ �Ǿ���� �� ������ ..?
	}

	void ColliderBase::OnDestroy()
	{
		DUOLGameEngine::PhysicsManager::GetInstance()->DetachPhysicsCollider(GetGameObject(), this);
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