#include "DUOLGameEngine/ECS/Component/Rigidbody.h"

#include "DUOLGameEngine/Manager/PhysicsManager.h"
#include "DUOLGameEngine/ECS/GameObject.h"

#include "DUOLGameEngine/ECS/Component/ColliderBase.h"

#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"

using namespace rttr;

RTTR_PLUGIN_REGISTRATION
{
	rttr::registration::class_<DUOLGameEngine::Rigidbody>("Rigidbody")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.property("_useGravity", &DUOLGameEngine::Rigidbody::GetUseGravity, &DUOLGameEngine::Rigidbody::SetUseGravity)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Bool)
	)
	.property("_mass", &DUOLGameEngine::Rigidbody::GetMass, &DUOLGameEngine::Rigidbody::SetMass)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
	)
	.property("_centerOfMass", &DUOLGameEngine::Rigidbody::GetCenterOfMass, &DUOLGameEngine::Rigidbody::SetCenterOfMass)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float3)
	)
	.property("_isFreezeRotation", &DUOLGameEngine::Rigidbody::GetIsFreezeRotation, &DUOLGameEngine::Rigidbody::SetIsFreezeRotation)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Bool)
	)
	.property("_isFreezePosition", &DUOLGameEngine::Rigidbody::GetIsFreezePosition, &DUOLGameEngine::Rigidbody::SetIsFreezePosition)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Bool)
	)
	.property("_isKinematic", &DUOLGameEngine::Rigidbody::GetIsKinematic, &DUOLGameEngine::Rigidbody::SetIsKinematic)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Bool)
	);
}

namespace DUOLGameEngine
{
	Rigidbody::Rigidbody(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		ComponentBase(owner, name)
		, _dynamicActor()
		, _useGravity(true)
		, _mass(10.f)
		, _centerOfMass(DUOLMath::Vector3::Zero)
		, _isFreezeRotation(false)
		, _isFreezePosition(false)
		, _isKinematic(false)
	{
		
	}

	Rigidbody::~Rigidbody()
	{
		if (!_dynamicActor.expired())
		{
			// 제거해주자.
			DUOLGameEngine::PhysicsManager::GetInstance()->DetachPhysicsActor(GetGameObject());
		}
	}

	void Rigidbody::OnInitializeDynamicActor(const std::weak_ptr<DUOLPhysics::PhysicsDynamicActor>& dynamicActor)
	{
		_dynamicActor = dynamicActor;

		const std::shared_ptr<DUOLPhysics::PhysicsDynamicActor> dActor = _dynamicActor.lock();

		dActor->SetMass(_mass);
		dActor->SetGravityEnable(_useGravity);

		this->SetIsFreezeRotation(_isFreezeRotation);
		this->SetIsFreezePosition(_isFreezePosition);
		this->SetCenterOfMass(_centerOfMass);
		this->SetIsKinematic(_isKinematic);
	}

	void Rigidbody::OnDestroy()
	{
		// PhysX 시스템에서 연동된 Dynamic Actor를 삭제합니다.
		DUOLGameEngine::PhysicsManager::GetInstance()->DetachPhysicsActor(GetGameObject());

		const std::vector<ColliderBase*> hasCols
			= GetGameObject()->GetComponents<DUOLGameEngine::ColliderBase>();

		if (!hasCols.empty())
			DUOLGameEngine::PhysicsManager::GetInstance()->AttachPhysicsStaticActor(GetGameObject());
	}

	bool Rigidbody::GetUseGravity() const
	{
		return _useGravity;
	}

	void Rigidbody::SetUseGravity(bool value)
	{
		if (_useGravity == value)
			return;

		_useGravity = value;

		if (!_dynamicActor.expired())
			_dynamicActor.lock()->SetGravityEnable(value);
	}

	float Rigidbody::GetMass()
	{
		return _mass;
	}

	void Rigidbody::SetMass(float value)
	{
		_mass = value;

		if (!_dynamicActor.expired())
			_dynamicActor.lock()->SetMass(_mass);
	}

	void Rigidbody::AddForce(const DUOLMath::Vector3& force) const
	{
		if (!_dynamicActor.expired())
			_dynamicActor.lock()->AddForce(force);
	}

	void Rigidbody::AddTorque(const DUOLMath::Vector3& force) const
	{
		if (!_dynamicActor.expired())
			_dynamicActor.lock()->AddTorque(force);
	}

	void Rigidbody::AddImpulse(const DUOLMath::Vector3& force) const
	{
		if (!_dynamicActor.expired())
			_dynamicActor.lock()->AddImpulse(force);
	}

	bool DUOLGameEngine::Rigidbody::GetIsFreezeRotation() const
	{
		return _isFreezeRotation;
	}

	void Rigidbody::SetIsFreezeRotation(bool value)
	{
		_isFreezeRotation = value;

		if (_isFreezeRotation)
		{
			DUOLPhysics::AxisLockFlags flags = DUOLPhysics::AxisLock::ANGULAR_X;
										flags |= DUOLPhysics::AxisLock::ANGULAR_Y;
										flags |= DUOLPhysics::AxisLock::ANGULAR_Z;
			
			if (!_dynamicActor.expired())
				_dynamicActor.lock()->SetAxesLock(flags);
		}
		else
		{
			if (!_dynamicActor.expired())
				_dynamicActor.lock()->SetAxesLock(0);
		}
	}

	bool Rigidbody::GetIsFreezePosition() const
	{
		return _isFreezePosition;
	}

	void Rigidbody::SetIsFreezePosition(bool value)
	{
		_isFreezePosition = value;

		if (_isFreezePosition)
		{
			DUOLPhysics::AxisLockFlags flags = DUOLPhysics::AxisLock::LINEAR_X;
			flags |= DUOLPhysics::AxisLock::LINEAR_Y;
			flags |= DUOLPhysics::AxisLock::LINEAR_Z;
			flags |= DUOLPhysics::AxisLock::ANGULAR_X;
			flags |= DUOLPhysics::AxisLock::ANGULAR_Y;
			flags |= DUOLPhysics::AxisLock::ANGULAR_Z;

			if (!_dynamicActor.expired())
				_dynamicActor.lock()->SetAxesLock(flags);
		}
		else
		{
			if (!_dynamicActor.expired())
				_dynamicActor.lock()->SetAxesLock(0);
		}
	}

	DUOLMath::Vector3 Rigidbody::GetLinearVelocity() const
	{
		return _dynamicActor.expired() ? DUOLMath::Vector3::Zero : _dynamicActor.lock()->GetLinearVelocity();
	}

	void Rigidbody::SetLinearVelocity(const DUOLMath::Vector3& velocity)
	{
		if (!_dynamicActor.expired())
		{
			_dynamicActor.lock()->SetLinearVelocity(velocity);
		}
	}

	DUOLMath::Vector3 Rigidbody::GetAngularVelocity() const
	{
		return _dynamicActor.expired() ? DUOLMath::Vector3::Zero : _dynamicActor.lock()->GetAngularVelocity();
	}

	void Rigidbody::SetAngularVelocity(const DUOLMath::Vector3& velocity)
	{
		if (!_dynamicActor.expired())
		{
			_dynamicActor.lock()->SetAngularVelocity(velocity);
		}
	}

	const DUOLMath::Vector3& Rigidbody::GetCenterOfMass() const
	{
		return _centerOfMass;
	}

	void Rigidbody::SetCenterOfMass(const DUOLMath::Vector3& center)
	{
		_centerOfMass = center;

		if (!_dynamicActor.expired())
			_dynamicActor.lock()->SetCenterOfMass(center);
	}

	bool Rigidbody::GetIsKinematic() const
	{
		return _isKinematic;
	}

	void Rigidbody::SetIsKinematic(bool value)
	{
		_isKinematic = value;

		if (!_dynamicActor.expired())
			_dynamicActor.lock()->SetKinematicActor(value);
	}
}