#include "PhysicsActorBaseImpl.h"

/* Shape */
#include "../Shapes/PhysicsShapeBaseImpl.h"

/* etc */
#include "DUOLCommon/LogHelper.h"
#include "DUOLPhysics/Util/PhysicsDefines.h"

#include <iostream>

namespace DUOLPhysics
{
	void PhysicsActorBase::SetImpl(const std::shared_ptr<Impl>& impl)
	{
		_impl = impl;
	}

	bool PhysicsActorBase::GetSimulationEnable() const
	{
		try
		{
			if (_impl.expired() == true)
				ERROR_THROW("No Implementation was generated.");

			return _impl.lock()->GetSimulationEnable();
		}
		catch (const std::string& errStr)
		{
			DUOL_ENGINE_ERROR(errStr);
		}
		catch (...)
		{
			DUOL_ENGINE_ERROR("Unknown Error.");
		}

		return false;
	}

	void PhysicsActorBase::SetSimulationEnable(bool useSimulation)
	{
		try
		{
			if (_impl.expired() == true)
				ERROR_THROW("No Implementation was generated.");

			_impl.lock()->SetSimulationEnable(useSimulation);
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

	PhysicsPose PhysicsActorBase::GetGlobalPose() const
	{
		try
		{
			if (_impl.expired() == true)
				ERROR_THROW("No Implementation was generated.");

			return _impl.lock()->GetGlobalPose();
		}
		catch (const std::string& errStr)
		{
			DUOL_ENGINE_ERROR(errStr.c_str());
		}
		catch (...)
		{
			DUOL_ENGINE_ERROR("Unknown Error.");
		}

		return PhysicsPose{};
	}

	void PhysicsActorBase::SetGlobalPose(const DUOLMath::Vector3& worldPosition)
	{
		try
		{
			if (_impl.expired() == true)
				ERROR_THROW("No Implementation was generated.");

			_impl.lock()->SetGlobalPose(worldPosition);
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

	void PhysicsActorBase::SetGlobalPose(const DUOLMath::Quaternion& quat)
	{
		try
		{
			if (_impl.expired() == true)
				ERROR_THROW("No Implementation was generated.");

			_impl.lock()->SetGlobalPose(quat);
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

	void PhysicsActorBase::SetGlobalPose(const DUOLMath::Matrix& transform)
	{
		try
		{
			if (_impl.expired() == true)
				ERROR_THROW("No Implementation was generated.");

			_impl.lock()->SetGlobalPose(transform);
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

	void PhysicsActorBase::SetGlobalPose(const PhysicsPose& globalPose)
	{
		try
		{
			if (_impl.expired() == true)
				ERROR_THROW("No Implementation was generated.");

			_impl.lock()->SetGlobalPose(globalPose);
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

	void PhysicsActorBase::SetUserData(void* userData)
	{
		try
		{
			if (_impl.expired() == true)
				ERROR_THROW("No Implementation was generated.");

			_impl.lock()->SetUserData(userData);
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

	void PhysicsActorBase::SetTriggerEnterEvent(TriggerEvent enter)
	{
		try
		{
			if (_impl.expired() == true)
				ERROR_THROW("No Implementation was generated.");

			_impl.lock()->SetTriggerEnterEvent(enter);
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

	void PhysicsActorBase::SetTriggerStayEvent(TriggerEvent stay)
	{
		try
		{
			if (_impl.expired() == true)
				ERROR_THROW("No Implementation was generated.");

			_impl.lock()->SetTriggerStayEvent(stay);
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

	void PhysicsActorBase::SetTriggerExitEvent(TriggerEvent exit)
	{
		try
		{
			if (_impl.expired() == true)
				ERROR_THROW("No Implementation was generated.");

			_impl.lock()->SetTriggerExitEvent(exit);
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

	void PhysicsActorBase::SetCollisionEnterEvent(CollisionEvent enter)
	{
		try
		{
			if (_impl.expired() == true)
				ERROR_THROW("No Implementation was generated.");

			_impl.lock()->SetCollisionEnterEvent(enter);
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

	void PhysicsActorBase::SetCollisionStayEvent(CollisionEvent stay)
	{
		try
		{
			if (_impl.expired() == true)
				ERROR_THROW("No Implementation was generated.");

			_impl.lock()->SetCollisionStayEvent(stay);
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

	void PhysicsActorBase::SetCollisionExitEvent(CollisionEvent exit)
	{
		try
		{
			if (_impl.expired() == true)
				ERROR_THROW("No Implementation was generated.");

			_impl.lock()->SetCollisionExitEvent(exit);
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

	PhysicsBoundingBox PhysicsActorBase::GetBoundingBox(float inflation) const
	{
		try
		{
			if (_impl.expired() == true)
				ERROR_THROW("No Implementation was generated.");

			return _impl.lock()->GetBoundingBox(inflation);
		}
		catch (const std::string& errStr)
		{
			DUOL_ENGINE_ERROR(errStr.c_str());
		}
		catch (...)
		{
			DUOL_ENGINE_ERROR("Unknown Error.");
		}

		return PhysicsBoundingBox{};
	}

	void PhysicsActorBase::AttachShape(std::weak_ptr<PhysicsShapeBase> shape)
	{
		try
		{
			if (_impl.expired() == true)
				ERROR_THROW("No Implementation was generated.");

			if (shape.expired() == true)
				ERROR_THROW("Failed to Attach Shape. (No Shape.)");

			_impl.lock()->AttachShape(shape.lock()->_impl->GetShape());
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

	void PhysicsActorBase::DetachShape(std::weak_ptr<PhysicsShapeBase> shape, bool isWakeOnLostTouch)
	{
		try
		{
			if (_impl.expired() == true)
				ERROR_THROW("No Implementation was generated.");

			if (shape.expired() == true)
				ERROR_THROW("Failed to Attach Shape. (No Shape.)");

			_impl.lock()->DetachShape(shape.lock()->_impl->GetShape(), isWakeOnLostTouch);
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