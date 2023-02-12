#include "DUOLGameEngine/ECS/Component/SphereCollider.h"

#include "DUOLGameEngine/ECS/GameObject.h"

#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"

using namespace rttr;

RTTR_PLUGIN_REGISTRATION
{
	rttr::registration::class_<DUOLGameEngine::SphereCollider>("SphereCollider")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.property("_center", &DUOLGameEngine::SphereCollider::GetCenter, &DUOLGameEngine::SphereCollider::SetCenter)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float3)
	)
	.property("_radius", &DUOLGameEngine::SphereCollider::GetRadius, &DUOLGameEngine::SphereCollider::SetRadius)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
	);
}

namespace DUOLGameEngine
{
	SphereCollider::SphereCollider(DUOLGameEngine::GameObject* owner,
		const DUOLCommon::tstring& name) :
		ColliderBase(owner, name)
		, _physicsSphere(std::weak_ptr<DUOLPhysics::PhysicsSphere>())
		, _center(DUOLMath::Vector3::Zero)
		, _radius(10.f)
	{
	}

	SphereCollider::~SphereCollider()
	{
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
		// 다시 만들어줘야 하나 ..?
		_radius = radius;

		if (!_physicsSphere.expired())
		{
			_physicsSphere.lock()->SetScale(_radius);
		}
	}
}