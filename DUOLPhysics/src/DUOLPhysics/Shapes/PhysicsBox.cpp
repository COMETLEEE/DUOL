#pragma once
#ifndef _SILENCE_CXX20_CISO646_REMOVED_WARNING
#define _SILENCE_CXX20_CISO646_REMOVED_WARNING
#endif

#include "DUOLPhysics/Shapes/PhysicsBox.h"

/* System */
#include "../System/PhysicsSystemImpl.h"

/* Shape */
#include "PhysicsShapeBaseImpl.h"

/* Scene */
#include "../Scene/PhysicsSceneImpl.h"

/* Actor */
#include "../Actor/PhysicsDynamicActorImpl.h"

/* etc */
#include "../Util/PhysicsTypeConverter.h"
#include "DUOLPhysics/Util/PhysicsDefines.h"

namespace DUOLPhysics
{
	PhysicsBox::PhysicsBox()
	{

	}

	void PhysicsBox::Create(PhysicsSystem* system, const PhysicsShapeDesc& shapeDesc)
	{
		if (_impl == nullptr)
			ERROR_THROW("Failed to create PxBox. (No Implementation.)");

		PxBoxGeometry boxGeometry(shapeDesc._box._halfExtentX, shapeDesc._box._halfExtentY, shapeDesc._box._halfExtentZ);

		_impl->Create(system, boxGeometry, shapeDesc);
	}

	void PhysicsBox::Create(PhysicsScene* scene, const PhysicsShapeDesc& shapeDesc)
	{
		if (_impl == nullptr)
			ERROR_THROW("Failed to create PxBox. (No Implementation.)");

		PxBoxGeometry boxGeometry(shapeDesc._box._halfExtentX, shapeDesc._box._halfExtentY, shapeDesc._box._halfExtentZ);

		_impl->Create(scene, boxGeometry, shapeDesc);
	}

	void PhysicsBox::SetScale(float halfExtentX, float halfExtentY, float halfExtentZ)
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

			_impl->GetShape()->setGeometry(PxBoxGeometry(halfExtentX, halfExtentY, halfExtentZ));
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

	void PhysicsBox::SetScale(const DUOLMath::Vector3& halfScale)
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

			_impl->GetShape()->setGeometry(PxBoxGeometry(ConvertVector3(halfScale)));
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