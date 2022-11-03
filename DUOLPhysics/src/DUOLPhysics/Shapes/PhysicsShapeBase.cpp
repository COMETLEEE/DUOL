#include "PhysicsShapeBaseImpl.h"

/* Actor */
#include "../Actor/PhysicsDynamicActorImpl.h"

/* etc */
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
	PhysicsShapeBase::PhysicsShapeBase() :
		_impl(std::make_shared<Impl>())
	{

	}

	PhysicsShapeBase::~PhysicsShapeBase()
	{

	}

	void PhysicsShapeBase::Attachment(PhysicsDynamicActor* actor)
	{
		if (_impl == nullptr)
			ERROR_THROW("Failed to Attachment. (No Implementation.)");

		auto originActor = actor->_impl->GetActor();

		if (originActor == nullptr)
			ERROR_THROW("Failed to Attachment. (No Actor.)");

		originActor->attachShape(*_impl->GetShape());
	}

	void PhysicsShapeBase::Detachment(PhysicsDynamicActor* actor, bool isWakeOnLostTouch)
	{
		if (_impl == nullptr)
			ERROR_THROW("Failed to Detachment. (No Implementation.)");

		auto originActor = actor->_impl->GetActor();

		if (originActor == nullptr)
			ERROR_THROW("Failed to Detachment. (No Actor.)");

		originActor->detachShape(*_impl->GetShape(), isWakeOnLostTouch);
	}

	void PhysicsShapeBase::Release()
	{
		if (_impl != nullptr)
			_impl->Release();
	}
}