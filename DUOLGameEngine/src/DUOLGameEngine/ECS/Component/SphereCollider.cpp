#include "DUOLGameEngine/ECS/Component/SphereCollider.h"

namespace DUOLGameEngine
{
	SphereCollider::SphereCollider(const std::weak_ptr<DUOLGameEngine::GameObject>& owner,
		const DUOLCommon::tstring& name) :
		ColliderBase(owner, name)
		, _physicsSphere(std::weak_ptr<DUOLPhysics::PhysicsSphere>())
		, _center(DUOLMath::Vector3::Zero)
		, _radius(10.f)
	{
	}

	SphereCollider::~SphereCollider()
	{
		_physicsSphere.lock()->Release();

		_physicsSphere.reset();
	}

	void SphereCollider::OnEnable()
	{
		_physicsActor.lock()->AttachShape(_physicsSphere);
	}

	void SphereCollider::OnDisable()
	{
		_physicsActor.lock()->DetachShape(_physicsSphere);
	}

	void SphereCollider::SetCenter(const DUOLMath::Vector3& center)
	{
		_center = center;

		if (!_physicsSphere.expired())
		{
			_physicsSphere.lock()->SetLocalPose(center);
		}
	}

	void SphereCollider::SetRadius(float radius)
	{
		if (!_physicsSphere.expired())
		{
			// 다시 만들어줘야 하나 ..?
		}
	}
}