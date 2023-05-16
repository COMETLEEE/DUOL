#include "DUOLGameEngine/ECS/Component/Rigidbody.h"

#include "DUOLGameEngine/Manager/PhysicsManager.h"
#include "DUOLGameEngine/ECS/GameObject.h"

#include "DUOLGameEngine/ECS/Component/Transform.h"
#include "DUOLGameEngine/ECS/Component/ColliderBase.h"

#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"
#include "DUOLGameEngine/Manager/TimeManager.h"

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
	.property("_isInterpolate", &DUOLGameEngine::Rigidbody::GetIsInterpolate, &DUOLGameEngine::Rigidbody::SetIsInterpolate)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Bool)
	)
	.property("_isFreezeXRotation", &DUOLGameEngine::Rigidbody::GetIsFreezeXRotation, &DUOLGameEngine::Rigidbody::SetIsFreezeXRotation)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Bool)
	)
	.property("_isFreezeYRotation", &DUOLGameEngine::Rigidbody::GetIsFreezeYRotation, &DUOLGameEngine::Rigidbody::SetIsFreezeYRotation)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Bool)
	)
	.property("_isFreezeZRotation", &DUOLGameEngine::Rigidbody::GetIsFreezeZRotation, &DUOLGameEngine::Rigidbody::SetIsFreezeZRotation)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Bool)
	)
	.property("_isFreezeXPosition", &DUOLGameEngine::Rigidbody::GetIsFreezeXPosition, &DUOLGameEngine::Rigidbody::SetIsFreezeXPosition)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Bool)
	)
	.property("_isFreezeYPosition", &DUOLGameEngine::Rigidbody::GetIsFreezeYPosition, &DUOLGameEngine::Rigidbody::SetIsFreezeYPosition)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Bool)
	)
	.property("_isFreezeZPosition", &DUOLGameEngine::Rigidbody::GetIsFreezeZPosition, &DUOLGameEngine::Rigidbody::SetIsFreezeZPosition)
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
	)
	.property("_isCCD", &DUOLGameEngine::Rigidbody::GetIsCCD, &DUOLGameEngine::Rigidbody::SetIsCCD)
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
		, _isInterpolate(false)
		, _isFreezeXRotation(false)
		, _isFreezeYRotation(false)
		, _isFreezeZRotation(false)
		, _isFreezeXPosition(false)
		, _isFreezeYPosition(false)
		, _isFreezeZPosition(false)
		, _isKinematic(false)
		, _isCCD(false)
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

		/*this->SetIsFreezeXRotation(_isFreezeXRotation);
		this->SetIsFreezeXPosition(_isFreezeXPosition);
		this->SetCenterOfMass(_centerOfMass);
		this->SetIsKinematic(_isKinematic);*/

		this->SetIsInterpolate(_isInterpolate);
	}

	void Rigidbody::ExecuteAllMessages()
	{
		for (auto message : _allPhysicsMessages)
			message();

		_allPhysicsMessages.clear();
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
		auto message = [this, value]()
		{
			_useGravity = value;

			if (!_dynamicActor.expired())
				_dynamicActor.lock()->SetGravityEnable(value);
		};

		_allPhysicsMessages.push_back(message);
	}

	float Rigidbody::GetMass()
	{
		return _mass;
	}

	void Rigidbody::SetMass(float value)
	{
		auto message = [this, value]()
		{
			_mass = value;

			if (!_dynamicActor.expired())
				_dynamicActor.lock()->SetMass(_mass);
		};

		_allPhysicsMessages.push_back(message);
	}

	void Rigidbody::AddForce(const DUOLMath::Vector3& force)
	{
		auto message = [this, force]()
		{
			if (!_dynamicActor.expired())
				_dynamicActor.lock()->AddForce(force);
		};

		_allPhysicsMessages.push_back(message);
	}

	void Rigidbody::AddTorque(const DUOLMath::Vector3& force)
	{
		auto message = [this, force]()
		{
			if (!_dynamicActor.expired())
				_dynamicActor.lock()->AddTorque(force);
		};

		_allPhysicsMessages.push_back(message);
	}

	void Rigidbody::AddImpulse(const DUOLMath::Vector3& force)
	{
		auto message = [this, force]()
		{
			if (!_dynamicActor.expired())
				_dynamicActor.lock()->AddImpulse(force);
		};

		_allPhysicsMessages.push_back(message);
	}

	bool DUOLGameEngine::Rigidbody::GetIsFreezeXRotation() const
	{
		return _isFreezeXRotation;
	}

	bool Rigidbody::GetIsFreezeYRotation() const
	{
		return _isFreezeYRotation;
	}

	bool Rigidbody::GetIsFreezeZRotation() const
	{
		return _isFreezeZRotation;
	}

	void Rigidbody::SetIsFreezeXRotation(bool value)
	{
		auto message = [this, value]()
		{
			_isFreezeXRotation = value;

			if (_isFreezeXRotation)
				_axisLockFlags |= DUOLPhysics::AxisLock::ANGULAR_X;
			else
				_axisLockFlags &= ~static_cast<int>(DUOLPhysics::AxisLock::ANGULAR_X);

			if (!_dynamicActor.expired())
				_dynamicActor.lock()->SetAxesLock(_axisLockFlags);
		};

		_allPhysicsMessages.push_back(message);
	}

	void Rigidbody::SetIsFreezeYRotation(bool value)
	{
		auto message = [this, value]()
		{
			_isFreezeYRotation = value;

			if (_isFreezeYRotation)
				_axisLockFlags |= DUOLPhysics::AxisLock::ANGULAR_Y;
			else
				_axisLockFlags &= ~static_cast<int>(DUOLPhysics::AxisLock::ANGULAR_Y);

			if (!_dynamicActor.expired())
				_dynamicActor.lock()->SetAxesLock(_axisLockFlags);
		};

		_allPhysicsMessages.push_back(message);
	}

	void Rigidbody::SetIsFreezeZRotation(bool value)
	{
		auto message = [this, value]()
		{
			_isFreezeZRotation = value;

			if (_isFreezeZRotation)
				_axisLockFlags |= DUOLPhysics::AxisLock::ANGULAR_Z;
			else
				_axisLockFlags &= ~static_cast<int>(DUOLPhysics::AxisLock::ANGULAR_Z);

			if (!_dynamicActor.expired())
				_dynamicActor.lock()->SetAxesLock(_axisLockFlags);
		};

		_allPhysicsMessages.push_back(message);
	}

	bool Rigidbody::GetIsFreezeXPosition() const
	{
		return _isFreezeXPosition;
	}

	bool Rigidbody::GetIsFreezeYPosition() const
	{
		return _isFreezeYPosition;
	}

	bool Rigidbody::GetIsFreezeZPosition() const
	{
		return _isFreezeZPosition;
	}

	void Rigidbody::SetIsFreezeXPosition(bool value)
	{
		auto message = [this, value]()
		{
			_isFreezeXPosition = value;

			if (_isFreezeXPosition)
				_axisLockFlags |= DUOLPhysics::AxisLock::LINEAR_X;
			else
				_axisLockFlags &= ~static_cast<int>(DUOLPhysics::AxisLock::LINEAR_X);

			if (!_dynamicActor.expired())
				_dynamicActor.lock()->SetAxesLock(_axisLockFlags);
		};

		_allPhysicsMessages.push_back(message);
	}

	void Rigidbody::SetIsFreezeYPosition(bool value)
	{
		auto message = [this, value]()
		{
			_isFreezeYPosition = value;

			if (_isFreezeYPosition)
				_axisLockFlags |= DUOLPhysics::AxisLock::LINEAR_Y;
			else
				_axisLockFlags &= ~static_cast<int>(DUOLPhysics::AxisLock::LINEAR_Y);

			if (!_dynamicActor.expired())
				_dynamicActor.lock()->SetAxesLock(_axisLockFlags);
		};

		_allPhysicsMessages.push_back(message);
	}

	void Rigidbody::SetIsFreezeZPosition(bool value)
	{
		auto message = [this, value]()
		{
			_isFreezeZPosition = value;

			if (_isFreezeZPosition)
				_axisLockFlags |= DUOLPhysics::AxisLock::LINEAR_Z;
			else
				_axisLockFlags &= ~static_cast<int>(DUOLPhysics::AxisLock::LINEAR_Z);

			if (!_dynamicActor.expired())
				_dynamicActor.lock()->SetAxesLock(_axisLockFlags);
		};

		_allPhysicsMessages.push_back(message);
	}

	DUOLMath::Vector3 Rigidbody::GetLinearVelocity() const
	{
		return _dynamicActor.expired() ? DUOLMath::Vector3::Zero : _dynamicActor.lock()->GetLinearVelocity();
	}

	void Rigidbody::SetLinearVelocity(const DUOLMath::Vector3& velocity)
	{
		auto message = [this, velocity]()
		{
			if (!_dynamicActor.expired())
				_dynamicActor.lock()->SetLinearVelocity(velocity);
		};

		_allPhysicsMessages.push_back(message);
	}

	DUOLMath::Vector3 Rigidbody::GetAngularVelocity() const
	{
		return _dynamicActor.expired() ? DUOLMath::Vector3::Zero : _dynamicActor.lock()->GetAngularVelocity();
	}

	void Rigidbody::SetAngularVelocity(const DUOLMath::Vector3& velocity)
	{
		auto message = [this, velocity]()
		{
			if (!_dynamicActor.expired())
				_dynamicActor.lock()->SetAngularVelocity(velocity);
		};

		_allPhysicsMessages.push_back(message);
	}

	const DUOLMath::Vector3& Rigidbody::GetCenterOfMass() const
	{
		return _centerOfMass;
	}

	void Rigidbody::SetCenterOfMass(const DUOLMath::Vector3& center)
	{
		auto message = [this, center]()
		{
			_centerOfMass = center;

			if (!_dynamicActor.expired())
				_dynamicActor.lock()->SetCenterOfMass(center);
		};

		_allPhysicsMessages.push_back(message);
	}

	bool Rigidbody::GetIsKinematic() const
	{
		return _isKinematic;
	}

	void Rigidbody::SetIsKinematic(bool value)
	{
		auto message = [this, value]()
		{
			_isKinematic = value;

			if (!_dynamicActor.expired())
				_dynamicActor.lock()->SetKinematicActor(value);
		};

		_allPhysicsMessages.push_back(message);
	}

	bool Rigidbody::GetIsInterpolate() const
	{
		return _isInterpolate;
	}

	void Rigidbody::SetIsInterpolate(bool value)
	{
		_isInterpolate = value;

		if (!_dynamicActor.expired())
		{
			DUOLCommon::tstring uuidTString = DUOLCommon::StringHelper::ToTString(GetGameObject()->GetUUID());

			if (_isInterpolate)
			{
				DUOLGameEngine::Transform* transform = GetTransform();

				const DUOLMath::Vector3& position = transform->GetWorldPosition();

				const DUOLMath::Quaternion& rotation = transform->GetWorldRotation();

				float currentTime = DUOLGameEngine::TimeManager::GetInstance()->GetRealtimeSinceStartup();

				float fixedTimeStep = DUOLGameEngine::PhysicsManager::GetInstance()->_fixedTimeStep;

				DUOLGameEngine::PhysicsManager::GetInstance()->_physicsInterpolateDatas.insert({ uuidTString, 
					{ GetTransform(), { {position, rotation,currentTime}, {position, rotation,currentTime + fixedTimeStep } } } });
			}
			else
				if (DUOLGameEngine::PhysicsManager::GetInstance()->_physicsInterpolateDatas.contains(uuidTString))
					DUOLGameEngine::PhysicsManager::GetInstance()->_physicsInterpolateDatas.erase(uuidTString);
		}
	}

	bool Rigidbody::GetIsCCD() const
	{
		return _isCCD;
	}

	void Rigidbody::SetIsCCD(bool value)
	{
		auto message = [this, value]()
		{
			_isCCD = value;

			if (!_dynamicActor.expired())
				_dynamicActor.lock()->SetCCDActor(value);
		};

		_allPhysicsMessages.push_back(message);
	}
}