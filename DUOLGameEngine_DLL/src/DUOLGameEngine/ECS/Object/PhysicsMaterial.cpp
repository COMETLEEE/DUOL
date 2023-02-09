#include "DUOLGameEngine/ECS/Object/PhysicsMaterial.h"

#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"
using namespace rttr;

RTTR_PLUGIN_REGISTRATION
{
	rttr::registration::class_<DUOLGameEngine::PhysicsMaterial>("PhysicsMaterial")
	.constructor<>()
	(

	);
}

namespace DUOLGameEngine
{
	PhysicsMaterial::PhysicsMaterial(const DUOLCommon::tstring& name) :
		DUOLGameEngine::ObjectBase(name, ObjectType::Resource)
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