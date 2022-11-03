#include "PhysicsBoxImpl.h"
/* System */
#include "../System/PhysicsSystemImpl.h"

/* Scene */
#include "../Scene/PhysicsSceneImpl.h"

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
	PhysicsBox::PhysicsBox()
	{

	}

	PhysicsBox::~PhysicsBox()
	{

	}

	void PhysicsBox::Create(PhysicsSystem* system, const PhysicsShapeDesc& shapeDesc)
	{
		if (_impl == nullptr)
			ERROR_THROW("Failed to create PxBox. (No Implementation.)");

		PxPhysics* physics = system->_impl->GetPhysics();

		_impl->Create(physics, shapeDesc);
	}

	void PhysicsBox::Create(PhysicsScene* scene, const PhysicsShapeDesc& shapeDesc)
	{
		if (_impl == nullptr)
			ERROR_THROW("Failed to create PxBox. (No Implementation.)");

		PxPhysics* physics = scene->_impl->GetPhysics();

		_impl->Create(physics, shapeDesc);
	}
}