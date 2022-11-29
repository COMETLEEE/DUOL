#include "DUOLGameEngine/ECS/Component/CapsuleCollider.h"

namespace DUOLGameEngine
{
	CapsuleCollider::CapsuleCollider(const std::weak_ptr<DUOLGameEngine::GameObject>& owner, const DUOLCommon::tstring& name) :
		ColliderBase(owner, name)
		, _physicsCapsule()
		, _center (DUOLMath::Vector3::Up * 10.f)
		// , _center (DUOLMath::Vector3::Zero)
		, _height(10.f)
		, _radius(3.f)
	{
	}

	CapsuleCollider::~CapsuleCollider()
	{
	}

	void CapsuleCollider::OnEnable()
	{
		_physicsActor.lock()->AttachShape(_physicsCapsule);
	}

	void CapsuleCollider::OnDisable()
	{
		_physicsActor.lock()->DetachShape(_physicsCapsule);
	}

	void CapsuleCollider::SetCenter(const DUOLMath::Vector3& center)
	{
		_center = center;

		_physicsCapsule.lock()->SetLocalPose(center);
	}

	void CapsuleCollider::SetHeight(float height)
	{

	}

	void CapsuleCollider::SetRadius(float radius)
	{

	}
}