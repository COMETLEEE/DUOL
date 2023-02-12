#include "DUOLGameEngine/ECS/Component/CapsuleCollider.h"
#include "DUOLGameEngine/ECS/GameObject.h"

#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"

using namespace rttr;

RTTR_PLUGIN_REGISTRATION
{
	rttr::registration::class_<DUOLGameEngine::CapsuleCollider>("CapsuleCollider")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.property("_center", &DUOLGameEngine::CapsuleCollider::GetCenter, &DUOLGameEngine::CapsuleCollider::SetCenter)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float3)
	)
	.property("_height", &DUOLGameEngine::CapsuleCollider::GetHeight, &DUOLGameEngine::CapsuleCollider::SetHeight)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
	)
	.property("_radius", &DUOLGameEngine::CapsuleCollider::GetRadius, &DUOLGameEngine::CapsuleCollider::SetRadius)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
	);
}

namespace DUOLGameEngine
{
	CapsuleCollider::CapsuleCollider(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		ColliderBase(owner, name)
		, _physicsCapsule()
		, _center (DUOLMath::Vector3::Up * 1.5f)
		, _height(1.f)
		, _radius(0.5f)
	{
	}

	CapsuleCollider::~CapsuleCollider()
	{
		_physicsCapsule.reset();
	}

	void CapsuleCollider::OnEnable()
	{
		_physicsActor.lock()->AttachShape(_physicsCapsule);
	}

	void CapsuleCollider::OnDisable()
	{
		_physicsActor.lock()->DetachShape(_physicsCapsule);
	}

	const DUOLMath::Vector3& CapsuleCollider::GetCenter() const
	{
		return _center;
	}

	void CapsuleCollider::SetCenter(const DUOLMath::Vector3& center)
	{
		_center = center;

		DUOLPhysics::PhysicsPose pose;

		pose._position = _center;

		// DirectX 기본 각도로 (위를 볼 수 있도록) 돌려준다
		pose._quaternion = DUOLMath::Quaternion::CreateFromYawPitchRoll(0.f, 0.f, DUOLMath::MathHelper::DegreeToRadian(90.f));

		if (!_physicsCapsule.expired())
			_physicsCapsule.lock()->SetLocalPose(pose);
	}

	float CapsuleCollider::GetHeight() const
	{
		return _height;
	}

	void CapsuleCollider::SetHeight(float height)
	{
		// 다시 만들어줘야 하나 ..?
		_height = height;

		if (!_physicsCapsule.expired())
			_physicsCapsule.lock()->SetScale(_radius, _height / 2.f);
	}

	float CapsuleCollider::GetRadius() const
	{
		return _radius;
	}

	void CapsuleCollider::SetRadius(float radius)
	{
		// 다시 만들어줘야 하나 ..?
		_radius = radius;

		if (!_physicsCapsule.expired())
			_physicsCapsule.lock()->SetScale(_radius, _height / 2.f);
	}
}