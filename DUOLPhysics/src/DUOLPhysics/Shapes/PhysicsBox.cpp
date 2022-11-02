#include "PhysicsBoxImpl.h"
/* System */
#include "../System/PhysicsSystemImpl.h"

/* Actor */
#include "../Actor/PhysicsDynamicActorImpl.h"

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
	PhysicsBox::PhysicsBox() :
		_impl(std::make_shared<Impl>())
	{

	}

	PhysicsBox::~PhysicsBox()
	{

	}

	void PhysicsBox::Create(PhysicsSystem* system, const PhysicsShapeDesc& shapeDesc)
	{
		if (_impl == nullptr)
			ERROR_THROW("Failed to create PxBox. (No Implementation.)");

		PxPhysics* physics = system->_impl->_physics;

		_impl->Create(physics, shapeDesc);
	}

	void PhysicsBox::Attachment(PhysicsDynamicActor* actor)
	{
		if (_impl == nullptr)
			ERROR_THROW("Failed to Attachment. (No Implementation.)");

		auto originActor = actor->_impl->GetActor();

		if (originActor == nullptr)
			ERROR_THROW("Failed to Attachment. (No Actor.)");

		originActor->attachShape(*_impl->GetShape());
	}

	void PhysicsBox::Release()
	{
		if (_impl != nullptr)
			_impl = nullptr;
	}
}