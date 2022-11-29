#include "DUOLGameEngine/ECS/Object/PhysicsMaterial.h"

namespace DUOLGameEngine
{
	PhysicsMaterial::PhysicsMaterial(const DUOLCommon::tstring& name) :
		DUOLGameEngine::ObjectBase(name)
	{
	}

	PhysicsMaterial::~PhysicsMaterial()
	{
	}

	void PhysicsMaterial::SetPhysicsMaterial(const std::weak_ptr<DUOLPhysics::PhysicsMaterial>& physicsMaterial)
	{
		if (physicsMaterial.expired())
			return;

		_physicsMaterial = physicsMaterial;

		_primitivePhysicsMaterial = _physicsMaterial.lock().get();
	}
}