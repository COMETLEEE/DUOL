#include "PhysicsActorBaseImpl.h"

/* Shape */
#include "../Shapes/PhysicsShapeBaseImpl.h"

/* etc */
#include <iostream>
#include <string>

#define ERROR_THROW(errStr)				\
{										\
	std::string errTemp = errStr;		\
	errTemp += " / File : ";			\
	errTemp += __FILE__;				\
	errTemp += ", Line : ";				\
	errTemp += std::to_string(__LINE__);\
	throw errTemp;						\
}

namespace DUOLPhysics
{
	PhysicsActorBase::PhysicsActorBase()
	{

	}

	PhysicsActorBase::~PhysicsActorBase()
	{

	}

	void PhysicsActorBase::SetImpl(const std::shared_ptr<Impl>& impl)
	{
		_impl = impl;
	}

	GlobalPose PhysicsActorBase::GetGlobalPose()
	{
		try
		{
			if (_impl.expired() == true)
				ERROR_THROW("No Implementation was generated.");

			return _impl.lock()->GetGlobalPose();
		}
		catch (const std::string& errStr)
		{
			std::cerr << errStr << std::endl;
		}
		catch (...)
		{
			std::cerr << "Unknown Error." << std::endl;
		}

		return GlobalPose{};
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
			std::cerr << errStr << std::endl;
		}
		catch (...)
		{
			std::cerr << "Unknown Error." << std::endl;
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
			std::cerr << errStr << std::endl;
		}
		catch (...)
		{
			std::cerr << "Unknown Error." << std::endl;
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
			std::cerr << errStr << std::endl;
		}
		catch (...)
		{
			std::cerr << "Unknown Error." << std::endl;
		}
	}

	void PhysicsActorBase::SetGlobalPose(const GlobalPose& globalPose)
	{
		try
		{
			if (_impl.expired() == true)
				ERROR_THROW("No Implementation was generated.");

			_impl.lock()->SetGlobalPose(globalPose);
		}
		catch (const std::string& errStr)
		{
			std::cerr << errStr << std::endl;
		}
		catch (...)
		{
			std::cerr << "Unknown Error." << std::endl;
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
			std::cerr << errStr << std::endl;
		}
		catch (...)
		{
			std::cerr << "Unknown Error." << std::endl;
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
			std::cerr << errStr << std::endl;
		}
		catch (...)
		{
			std::cerr << "Unknown Error." << std::endl;
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
			std::cerr << errStr << std::endl;
		}
		catch (...)
		{
			std::cerr << "Unknown Error." << std::endl;
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
			std::cerr << errStr << std::endl;
		}
		catch (...)
		{
			std::cerr << "Unknown Error." << std::endl;
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
			std::cerr << errStr << std::endl;
		}
		catch (...)
		{
			std::cerr << "Unknown Error." << std::endl;
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
			std::cerr << errStr << std::endl;
		}
		catch (...)
		{
			std::cerr << "Unknown Error." << std::endl;
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
			std::cerr << errStr << std::endl;
		}
		catch (...)
		{
			std::cerr << "Unknown Error." << std::endl;
		}
	}

	PhysicsBoundingBox PhysicsActorBase::GetBoundingBox(float inflation)
	{
		try
		{
			if (_impl.expired() == true)
				ERROR_THROW("No Implementation was generated.");

			return _impl.lock()->GetBoundingBox(inflation);
		}
		catch (const std::string& errStr)
		{
			std::cerr << errStr << std::endl;
		}
		catch (...)
		{
			std::cerr << "Unknown Error." << std::endl;
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
			std::cerr << errStr << std::endl;
		}
		catch (...)
		{
			std::cerr << "Unknown Error." << std::endl;
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
			std::cerr << errStr << std::endl;
		}
		catch (...)
		{
			std::cerr << "Unknown Error." << std::endl;
		}
	}
}