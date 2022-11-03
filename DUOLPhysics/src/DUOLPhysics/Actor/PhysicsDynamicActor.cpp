#include "PhysicsDynamicActorImpl.h"

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
	PhysicsDynamicActor::PhysicsDynamicActor() :
		_impl(std::make_shared<Impl>())
	{

	}

	PhysicsDynamicActor::~PhysicsDynamicActor()
	{

	}

	void PhysicsDynamicActor::SetLinearVelocity(DUOLMath::Vector3 velocity)
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("No Implementation was generated.");

			_impl->SetLinearVelocity(velocity);
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

	void PhysicsDynamicActor::SetMassAndInertia(float density, DUOLMath::Vector3 massLocalPose, bool isIncludedNonSimShape)
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("No Implementation was generated.");

			_impl->SetMassAndInertia(density, massLocalPose, isIncludedNonSimShape);
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
			std::cerr << errStr << std::endl;
		}
		catch (...)
		{
			std::cerr << "Unknown Error." << std::endl;
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
			std::cerr << errStr << std::endl;
		}
		catch (...)
		{
			std::cerr << "Unknown Error." << std::endl;
		}
	}

	PhysicsBoundingBox PhysicsDynamicActor::GetBoundingBox(float inflation)
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("No Implementation was generated.");

			return _impl->GetBoundingBox(inflation);
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

	void PhysicsDynamicActor::AttachShape(std::weak_ptr<PhysicsShapeBase> shape)
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("No Implementation was generated.");

			if (shape.expired() == true)
				ERROR_THROW("Failed to Attach Shape. (No Shape.)");

			_impl->AttachShape(shape.lock()->_impl->GetShape());
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

	void PhysicsDynamicActor::DetachShape(std::weak_ptr<PhysicsShapeBase> shape, bool isWakeOnLostTouch)
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("No Implementation was generated.");

			if (shape.expired() == true)
				ERROR_THROW("Failed to Attach Shape. (No Shape.)");

			_impl->DetachShape(shape.lock()->_impl->GetShape(), isWakeOnLostTouch);
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

	void PhysicsDynamicActor::Release()
	{
		if (_impl != nullptr)
		{
			_impl->Release();
			_impl = nullptr;
		}
	}
}