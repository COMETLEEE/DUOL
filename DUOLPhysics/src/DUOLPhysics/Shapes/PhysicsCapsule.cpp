#pragma once
#ifndef _SILENCE_CXX20_CISO646_REMOVED_WARNING
#define _SILENCE_CXX20_CISO646_REMOVED_WARNING
#endif

#include "DUOLPhysics/Shapes/PhysicsCapsule.h"

/* System */
#include "../System/PhysicsSystemImpl.h"

/* Shape */
#include "PhysicsShapeBaseImpl.h"

/* Scene */
#include "../Scene/PhysicsSceneImpl.h"

/* Actor */
#include "../Actor/PhysicsDynamicActorImpl.h"

/* etc */
#include "DUOLPhysics/Util/PhysicsDefines.h"

namespace DUOLPhysics
{
	PhysicsCapsule::PhysicsCapsule()
	{

	}

	void PhysicsCapsule::Create(PhysicsSystem* system, const PhysicsShapeDesc& shapeDesc)
	{
		if (_impl == nullptr)
			ERROR_THROW("Failed to create PxBox. (No Implementation.)");

		PxCapsuleGeometry capsuleGeometry(shapeDesc._capsule._radius, shapeDesc._capsule._halfHeight);

		_impl->Create(system, capsuleGeometry, shapeDesc);
	}

	void PhysicsCapsule::Create(PhysicsScene* scene, const PhysicsShapeDesc& shapeDesc)
	{
		if (_impl == nullptr)
			ERROR_THROW("Failed to create PxBox. (No Implementation.)");

		PxCapsuleGeometry capsuleGeometry(shapeDesc._capsule._radius, shapeDesc._capsule._halfHeight);

		_impl->Create(scene, capsuleGeometry, shapeDesc);
	}

	void PhysicsCapsule::SetScale(float radius, float halfHeight)
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("Failed to set scale. (No Implementation.)");

			auto shape = _impl->GetShape();

			if (shape == nullptr)
				ERROR_THROW("Failed to set scale. (No PxShape.)");

			if (shape->isExclusive() != true)
				ERROR_THROW("Failed to set scale. (PxShape is not exclusive.)");

			_impl->GetShape()->setGeometry(PxCapsuleGeometry(radius, halfHeight));
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