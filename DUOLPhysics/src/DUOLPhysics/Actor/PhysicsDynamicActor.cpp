#include "PhysicsDynamicActorImpl.h"

/* etc */
#include "DUOLCommon/Log/LogHelper.h"
#include "DUOLPhysics/Util/PhysicsDefines.h"

#include <iostream>

namespace DUOLPhysics
{
	PhysicsDynamicActor::PhysicsDynamicActor() :
		_impl(std::make_shared<Impl>())
	{
		SetImpl(_impl);
	}

	bool PhysicsDynamicActor::GetGravityEnable() const
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("No Implementation was generated.");

			return _impl->GetGravityEnable();
		}
		catch (const std::string& errStr)
		{
			DUOL_ENGINE_ERROR(errStr.c_str());
		}
		catch (...)
		{
			DUOL_ENGINE_ERROR("Unknown Error.");
		}

		return false;
	}

	DUOLMath::Vector3 PhysicsDynamicActor::GetCenterOfMass() const
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("No Implementation was generated.");
			
			return _impl->GetCenterOfMass();
		}
		catch (const std::string& errStr)
		{
			DUOL_ENGINE_ERROR(errStr.c_str());
		}
		catch (...)
		{
			DUOL_ENGINE_ERROR("Unknown Error.");
		}

		return {};
	}

	void PhysicsDynamicActor::SetCenterOfMass(const DUOLMath::Vector3& localPose)
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("No Implementation was generated.");

			_impl->SetCenterOfMass(localPose);
		}
		catch (const std::string& errStr)
		{
			DUOL_ENGINE_ERROR(errStr.c_str());
		}
		catch (...)
		{
			DUOL_ENGINE_ERROR("Unknown Error.");
		}
	}

	void PhysicsDynamicActor::SetGravityEnable(bool useGravity)
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("No Implementation was generated.");

			_impl->SetGravityEnable(useGravity);
		}
		catch (const std::string& errStr)
		{
			DUOL_ENGINE_ERROR(errStr.c_str());
		}
		catch (...)
		{
			DUOL_ENGINE_ERROR("Unknown Error.");
		}
	}

	void PhysicsDynamicActor::AddForce(const DUOLMath::Vector3& force)
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("No Implementation was generated.");

			_impl->AddForce(force);
		}
		catch (const std::string& errStr)
		{
			DUOL_ENGINE_ERROR(errStr.c_str());
		}
		catch (...)
		{
			DUOL_ENGINE_ERROR("Unknown Error.");
		}
	}

	void PhysicsDynamicActor::AddImpulse(const DUOLMath::Vector3& impulse)
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("No Implementation was generated.");

			_impl->AddImpulse(impulse);
		}
		catch (const std::string& errStr)
		{
			DUOL_ENGINE_ERROR(errStr.c_str());
		}
		catch (...)
		{
			DUOL_ENGINE_ERROR("Unknown Error.");
		}
	}

	void PhysicsDynamicActor::ResetForce()
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("No Implementation was generated.");

			_impl->ResetForce();
		}
		catch (const std::string& errStr)
		{
			DUOL_ENGINE_ERROR(errStr.c_str());
		}
		catch (...)
		{
			DUOL_ENGINE_ERROR("Unknown Error.");
		}
	}

	void PhysicsDynamicActor::AddTorque(const DUOLMath::Vector3& torque)
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("No Implementation was generated.");

			_impl->AddTorque(torque);
		}
		catch (const std::string& errStr)
		{
			DUOL_ENGINE_ERROR(errStr.c_str());
		}
		catch (...)
		{
			DUOL_ENGINE_ERROR("Unknown Error.");
		}
	}

	void PhysicsDynamicActor::ResetTorque()
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("No Implementation was generated.");

			_impl->ResetTorque();
		}
		catch (const std::string& errStr)
		{
			DUOL_ENGINE_ERROR(errStr.c_str());
		}
		catch (...)
		{
			DUOL_ENGINE_ERROR("Unknown Error.");
		}
	}

	bool PhysicsDynamicActor::GetAxisLockState(AxisLock targetAxis) const
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("No Implementation was generated.");

			return _impl->GetAxisLockState(targetAxis);
		}
		catch (const std::string& errStr)
		{
			DUOL_ENGINE_ERROR(errStr.c_str());
		}
		catch (...)
		{
			DUOL_ENGINE_ERROR("Unknown Error.");
		}

		return false;
	}

	AxisLockFlags PhysicsDynamicActor::GetAxesLockState() const
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("No Implementation was generated.");

			return _impl->GetAxesLockState();
		}
		catch (const std::string& errStr)
		{
			DUOL_ENGINE_ERROR(errStr.c_str());
		}
		catch (...)
		{
			DUOL_ENGINE_ERROR("Unknown Error.");
		}

		return AxisLockFlags{};
	}

	void PhysicsDynamicActor::SetAxisLock(AxisLock targetAxis, bool isLock)
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("No Implementation was generated.");

			_impl->SetAxisLock(targetAxis, isLock);
		}
		catch (const std::string& errStr)
		{
			DUOL_ENGINE_ERROR(errStr.c_str());
		}
		catch (...)
		{
			DUOL_ENGINE_ERROR("Unknown Error.");
		}
	}

	void PhysicsDynamicActor::SetAxesLock(AxisLockFlags targetAxes)
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("No Implementation was generated.");

			_impl->SetAxesLock(targetAxes);
		}
		catch (const std::string& errStr)
		{
			DUOL_ENGINE_ERROR(errStr.c_str());
		}
		catch (...)
		{
			DUOL_ENGINE_ERROR("Unknown Error.");
		}
	}

	DUOLMath::Vector3 PhysicsDynamicActor::GetLinearVelocity() const
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("No Implementation was generated.");

			return _impl->GetLinearVelocity();
		}
		catch (const std::string& errStr)
		{
			DUOL_ENGINE_ERROR(errStr.c_str());
		}
		catch (...)
		{
			DUOL_ENGINE_ERROR("Unknown Error.");
		}

		return {};
	}

	void PhysicsDynamicActor::SetLinearVelocity(const DUOLMath::Vector3& velocity)
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("No Implementation was generated.");

			_impl->SetLinearVelocity(velocity);
		}
		catch (const std::string& errStr)
		{
			DUOL_ENGINE_ERROR(errStr.c_str());
		}
		catch (...)
		{
			DUOL_ENGINE_ERROR("Unknown Error.");
		}
	}

	DUOLMath::Vector3 PhysicsDynamicActor::GetAngularVelocity() const
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("No Implementation was generated.");

			return _impl->GetAngularVelocity();
		}
		catch (const std::string& errStr)
		{
			DUOL_ENGINE_ERROR(errStr.c_str());
		}
		catch (...)
		{
			DUOL_ENGINE_ERROR("Unknown Error.");
		}

		return {};
	}

	void PhysicsDynamicActor::SetAngularVelocity(const DUOLMath::Vector3& velocity)
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("No Implementation was generated.");

			_impl->SetAngularVelocity(velocity);
		}
		catch (const std::string& errStr)
		{
			DUOL_ENGINE_ERROR(errStr.c_str());
		}
		catch (...)
		{
			DUOL_ENGINE_ERROR("Unknown Error.");
		}
	}

	float PhysicsDynamicActor::GetMass() const
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("No Implementation was generated.");

			return _impl->GetMass();
		}
		catch (const std::string& errStr)
		{
			DUOL_ENGINE_ERROR(errStr.c_str());
		}
		catch (...)
		{
			DUOL_ENGINE_ERROR("Unknown Error.");
		}

		return 0.0f;
	}

	float PhysicsDynamicActor::GetInverseMass() const
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("No Implementation was generated.");

			return _impl->GetInverseMass();
		}
		catch (const std::string& errStr)
		{
			DUOL_ENGINE_ERROR(errStr.c_str());
		}
		catch (...)
		{
			DUOL_ENGINE_ERROR("Unknown Error.");
		}

		return 0.0f;
	}

	void PhysicsDynamicActor::SetMass(float mass)
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("No Implementation was generated.");

			_impl->SetMass(mass);
		}
		catch (const std::string& errStr)
		{
			DUOL_ENGINE_ERROR(errStr.c_str());
		}
		catch (...)
		{
			DUOL_ENGINE_ERROR("Unknown Error.");
		}
	}

	void PhysicsDynamicActor::SetMassAndInertia(float density, const DUOLMath::Vector3& massLocalPose, bool isIncludedNonSimShape)
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("No Implementation was generated.");

			_impl->SetMassAndInertia(density, massLocalPose, isIncludedNonSimShape);
		}
		catch (const std::string& errStr)
		{
			DUOL_ENGINE_ERROR(errStr.c_str());
		}
		catch (...)
		{
			DUOL_ENGINE_ERROR("Unknown Error.");
		}
	}

	void PhysicsDynamicActor::SetLinearDamping(float linDamp)
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("No Implementation was generated.");

			_impl->SetLinearDamping(linDamp);
		}
		catch (const std::string& errStr)
		{
			DUOL_ENGINE_ERROR(errStr.c_str());
		}
		catch (...)
		{
			DUOL_ENGINE_ERROR("Unknown Error.");
		}
	}

	void PhysicsDynamicActor::SetAngularDamping(float angDamp)
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("No Implementation was generated.");

			_impl->SetAngularDamping(angDamp);
		}
		catch (const std::string& errStr)
		{
			DUOL_ENGINE_ERROR(errStr.c_str());
		}
		catch (...)
		{
			DUOL_ENGINE_ERROR("Unknown Error.");
		}
	}

	bool PhysicsDynamicActor::GetKinematicActor() const
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("No Implementation was generated.");

			return _impl->GetKinematicActor();
		}
		catch (const std::string& errStr)
		{
			DUOL_ENGINE_ERROR(errStr.c_str());
		}
		catch (...)
		{
			DUOL_ENGINE_ERROR("Unknown Error.");
		}

		return false;
	}

	void PhysicsDynamicActor::SetKinematicActor(bool value)
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("No Implementation was generated.");

			_impl->SetKinematicActor(value);
		}
		catch (const std::string& errStr)
		{
			DUOL_ENGINE_ERROR(errStr.c_str());
		}
		catch (...)
		{
			DUOL_ENGINE_ERROR("Unknown Error.");
		}
	}

	void PhysicsDynamicActor::SetMaxLinearVelocity(float velocity)
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("No Implementation was generated.");

			_impl->SetMaxLinearVelocity(velocity);
		}
		catch (const std::string& errStr)
		{
			DUOL_ENGINE_ERROR(errStr.c_str());
		}
		catch (...)
		{
			DUOL_ENGINE_ERROR("Unknown Error.");
		}
	}

	void PhysicsDynamicActor::SetMaxAngularVelocity(float velocity)
	{
				try
		{
			if (_impl == nullptr)
				ERROR_THROW("No Implementation was generated.");

			_impl->SetMaxAngularVelocity(velocity);
		}
		catch (const std::string& errStr)
		{
			DUOL_ENGINE_ERROR(errStr.c_str());
		}
		catch (...)
		{
			DUOL_ENGINE_ERROR("Unknown Error.");
		}
	}

	void PhysicsDynamicActor::SetMaxContactImpulse(float contactImpulse)
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("No Implementation was generated.");

			_impl->SetMaxContactImpulse(contactImpulse);
		}
		catch (const std::string& errStr)
		{
			DUOL_ENGINE_ERROR(errStr.c_str());
		}
		catch (...)
		{
			DUOL_ENGINE_ERROR("Unknown Error.");
		}
		}

	void PhysicsDynamicActor::SetMaxDepenetrationVelocity(float depenetionVelocity)
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("No Implementation was generated.");

			_impl->SetMaxDepenetrationVelocity(depenetionVelocity);
		}
		catch (const std::string& errStr)
		{
			DUOL_ENGINE_ERROR(errStr.c_str());
		}
		catch (...)
		{
			DUOL_ENGINE_ERROR("Unknown Error.");
		}
	}

	bool PhysicsDynamicActor::GetCCDActor() const
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("No Implementation was generated.");

			return _impl->GetCCDActor();
		}
		catch (const std::string& errStr)
		{
			DUOL_ENGINE_ERROR(errStr.c_str());
		}
		catch (...)
		{
			DUOL_ENGINE_ERROR("Unknown Error.");
		}

		return false;
	}

	void PhysicsDynamicActor::SetCCDActor(bool value)
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("No Implementation was generated.");

			_impl->SetCCDActor(value);
		}
		catch (const std::string& errStr)
		{
			DUOL_ENGINE_ERROR(errStr.c_str());
		}
		catch (...)
		{
			DUOL_ENGINE_ERROR("Unknown Error.");
		}
	}
}