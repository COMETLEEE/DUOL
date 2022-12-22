#include "DUOLGameEngine/ECS/Component/Rigidbody.h"

namespace DUOLGameEngine
{
	Rigidbody::Rigidbody(const std::weak_ptr<DUOLGameEngine::GameObject>& owner, const DUOLCommon::tstring& name) :
		ComponentBase(owner, name)
		, _dynamicActor()
		, _useGravity(true)
		, _mass(50.f)
		, _isFreezeRotation(false)
	{
		
	}

	Rigidbody::~Rigidbody()
	{

	}

	void Rigidbody::OnInitializeDynamicActor(const std::weak_ptr<DUOLPhysics::PhysicsDynamicActor>& dynamicActor)
	{
		_dynamicActor = dynamicActor;

		const std::shared_ptr<DUOLPhysics::PhysicsDynamicActor> dActor = _dynamicActor.lock();

		dActor->SetMass(_mass);
		dActor->SetGravityEnable(_useGravity);

		this->SetIsFreezeRotation(_isFreezeRotation);
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

		_dynamicActor.lock()->SetGravityEnable(value);
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

	void Rigidbody::SetIsFreezeRotation(bool value)
	{
		_isFreezeRotation = value;

		if (_isFreezeRotation)
		{
			constexpr DUOLPhysics::AxisLock axisLock =
				static_cast<DUOLPhysics::AxisLock>(static_cast<int>(DUOLPhysics::AxisLock::ANGULAR_X) |
					static_cast<int>(DUOLPhysics::AxisLock::ANGULAR_Y) |
					static_cast<int>(DUOLPhysics::AxisLock::ANGULAR_Z));

			if (!_dynamicActor.expired())
				_dynamicActor.lock()->SetAxisLock(axisLock, true);
		}
		else
		{
			constexpr DUOLPhysics::AxisLock axisLock = DUOLPhysics::AxisLock::NONE;

			if (!_dynamicActor.expired())
				_dynamicActor.lock()->SetAxisLock(axisLock, false);
		}
	}

	DUOLMath::Vector3 Rigidbody::GetLinearVelocity() const
	{
		return _dynamicActor.expired() ? DUOLMath::Vector3::Zero : _dynamicActor.lock()->GetLinearVelocity();
	}

	void Rigidbody::SetLinearVelocity(const DUOLMath::Vector3& velocity)
	{
		auto actor = _dynamicActor.lock();

		if (actor != nullptr)
			actor->SetLinearVelocity(velocity);
	}

	DUOLMath::Vector3 Rigidbody::GetAngularVelocity() const
	{
		return _dynamicActor.expired() ? DUOLMath::Vector3::Zero : _dynamicActor.lock()->GetAngularVelocity();
	}

	void Rigidbody::SetAngularVelocity(const DUOLMath::Vector3& velocity)
	{
		auto actor = _dynamicActor.lock();

		if (actor != nullptr)
			actor->SetAngularVelocity(velocity);
	}
}