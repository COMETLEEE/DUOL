#pragma once
#ifndef _SILENCE_CXX20_CISO646_REMOVED_WARNING
#define _SILENCE_CXX20_CISO646_REMOVED_WARNING
#endif

#include "DUOLPhysics/Shapes/PhysicsSphere.h"

/* System */
#include "../System/PhysicsSystemImpl.h"

/* Shape */
#include "PhysicsShapeBaseImpl.h"

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
	PhysicsSphere::PhysicsSphere()
	{

	}

	PhysicsSphere::~PhysicsSphere()
	{

	}

	void PhysicsSphere::Create(PhysicsSystem* system, const PhysicsShapeDesc& shapeDesc)
	{
		if (_impl == nullptr)
			ERROR_THROW("Failed to create PxBox. (No Implementation.)");

		PxSphereGeometry sphereGeometry(shapeDesc._sphere._radius);

		_impl->Create(system, sphereGeometry, shapeDesc);
	}

	void PhysicsSphere::Create(PhysicsScene* scene, const PhysicsShapeDesc& shapeDesc)
	{
		if (_impl == nullptr)
			ERROR_THROW("Failed to create PxBox. (No Implementation.)");

		PxSphereGeometry sphereGeometry(shapeDesc._sphere._radius);

		_impl->Create(scene, sphereGeometry, shapeDesc);
	}
}