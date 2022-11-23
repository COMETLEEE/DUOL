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

		// Rigidbody가 있고 .. 없고에 따라서 동작이 되어야할 것 같은데 ..?
	}

	void ColliderBase::SetIsTrigger(bool value)
	{
		// 트리거 여부에 대해서 Scene에 등록되는 속성이 달라질듯 ..?
		if (_isTrigger == value)
			return;

		_isTrigger = value;

		// TODO : 이걸 하기 위해서 Base에 shape pointer 놔두는거 맞는지 생각할 것
		_physicsShapeBase.lock()->SetTriggerEnable(value);
	}
}