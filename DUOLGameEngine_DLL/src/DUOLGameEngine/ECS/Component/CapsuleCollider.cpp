#include "DUOLGameEngine/ECS/Component/CapsuleCollider.h"
#include "DUOLGameEngine/ECS/GameObject.h"

#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"

using namespace rttr;

RTTR_PLUGIN_REGISTRATION
{
	rttr::registration::enumeration<DUOLGameEngine::CapsuleDirection>("CapsuleDirection")
	(
		value("X-Axis", DUOLGameEngine::CapsuleDirection::X)
		, value("Y-Axis", DUOLGameEngine::CapsuleDirection::Y)
		, value("Z-Axis", DUOLGameEngine::CapsuleDirection::Z)
	);

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
	)
	.property("Direction", &DUOLGameEngine::CapsuleCollider::GetDirection, &DUOLGameEngine::CapsuleCollider::SetDirection)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Enumeration)
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
		, _currentDirection(CapsuleDirection::X)
	{
		
	}

	CapsuleCollider::~CapsuleCollider()
	{
		_physicsCapsule.reset();
	}

	void CapsuleCollider::SetCapsuleLocalPose()
	{
		DUOLPhysics::PhysicsPose pose;

		pose._position = Vector3(_center.x, _center.y, _center.z);

		// Axis에 따라서 바꿔줍니다.
		switch (_currentDirection)
		{
			case CapsuleDirection::X:
			{
				pose._quaternion = DUOLMath::Quaternion::CreateFromYawPitchRoll(0.f, DUOLMath::MathHelper::DegreeToRadian(90.f), 0.f);

				break;
			}

			case CapsuleDirection::Y:
			{
				pose._quaternion = DUOLMath::Quaternion::CreateFromYawPitchRoll(0.f, 0.f, DUOLMath::MathHelper::DegreeToRadian(90.f));

				break;
			}

			case CapsuleDirection::Z:
			{
				pose._quaternion = DUOLMath::Quaternion::CreateFromYawPitchRoll(0.f, DUOLMath::MathHelper::DegreeToRadian(90.f), DUOLMath::MathHelper::DegreeToRadian(90.f));

				break;
			}
		}

		if (!_physicsCapsule.expired())
			_physicsCapsule.lock()->SetLocalPose(pose);
	}

	void CapsuleCollider::OnEnable()
	{
		// TODO : 스케일드 이벤트 .. 캡슐 콜라이더는 어떻게 연산해야할지 잘 모르겠다 ..
		//_onScaledEventListenerID = GetTransform()->_scaledEvent += [this](const DUOLMath::Vector3& scale)
		//{
		//	if (!_physicsCapsule.expired())
		//	{
		//		// _physicsCapsule.lock()->SetScale(_radius *);
		//	}
		//};

		if (!_physicsActor.expired())
			_physicsActor.lock()->AttachShape(_physicsCapsule);
	}

	void CapsuleCollider::OnDisable()
	{
		if (!_physicsActor.expired())
			_physicsActor.lock()->DetachShape(_physicsCapsule);
	}

	const DUOLMath::Vector3& CapsuleCollider::GetCenter() const
	{
		return _center;
	}

	void CapsuleCollider::SetCenter(const DUOLMath::Vector3& center)
	{
		_center = center;

		SetCapsuleLocalPose();
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
		{
			_physicsCapsule.lock()->SetScale(_radius, _height / 2.f);
		}
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
		{
			_physicsCapsule.lock()->SetScale(_radius, _height / 2.f);
		}
	}

	DUOLGameEngine::CapsuleDirection DUOLGameEngine::CapsuleCollider::GetDirection() const
	{
		return _currentDirection;
	}

	void CapsuleCollider::SetDirection(DUOLGameEngine::CapsuleDirection direction)
	{
		_currentDirection = direction;

		SetCapsuleLocalPose();
	}
}