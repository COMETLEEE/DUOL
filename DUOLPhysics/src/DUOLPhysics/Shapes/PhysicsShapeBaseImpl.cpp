#include "PhysicsShapeBaseImpl.h"

/* System */
#include "../System/PhysicsSystemImpl.h"

/* Scene */
#include "../Scene/PhysicsSceneImpl.h"

/* Material */
#include "../Material/PhysicsMaterialImpl.h"

/* etc */
#include "../Util/PhysicsTypeConverter.h"

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
	bool PhysicsShapeBase::Impl::_usePVD = false;

	PhysicsShapeBase::Impl::Impl() :
		_shape(nullptr)
	{

	}

	PhysicsShapeBase::Impl::~Impl()
	{
		Release();
	}

	void PhysicsShapeBase::Impl::Create(PhysicsSystem* system, const PxGeometry& geometry, const PhysicsShapeDesc& shapeDesc)
	{
		Create(system->_impl->GetPhysics(), geometry, shapeDesc);
	}

	void PhysicsShapeBase::Impl::Create(PhysicsScene* scene, const PxGeometry& geometry, const PhysicsShapeDesc& shapeDesc)
	{
		Create(scene->_impl->GetPhysics(), geometry, shapeDesc);
	}

	void PhysicsShapeBase::Impl::Create(PxPhysics* physics, const PxGeometry& geometry, const PhysicsShapeDesc& shapeDesc)
	{
		if (physics == nullptr)
			ERROR_THROW("Failed to create PxShape. (No PxPhysics.)");

		PxMaterial* material = shapeDesc._material.lock()->_impl->GetMaterial();

		if (material == nullptr)
			ERROR_THROW("Failed to create PxShape. (No PxMaterial.)");

		auto flag = ConvertShapeFlags(shapeDesc._flag);

		if (_usePVD == true)
			flag |= PxShapeFlag::Enum::eVISUALIZATION;

		_shape = physics->createShape(geometry, *material, shapeDesc._isExclusive, flag);

		if (_shape == nullptr)
			ERROR_THROW("Failed to create PxShape.");
	}

	PxShape* PhysicsShapeBase::Impl::GetShape()
	{
		return _shape;
	}

	PxPhysics* PhysicsShapeBase::Impl::GetPhysics(PhysicsSystem* system)
	{
		return system->_impl->GetPhysics();
	}

	PxPhysics* PhysicsShapeBase::Impl::GetPhysics(PhysicsScene* scene)
	{
		return scene->_impl->GetPhysics();
	}

	PxCooking* PhysicsShapeBase::Impl::GetCooking(PhysicsSystem* system)
	{
		return system->_impl->GetCooking();
	}

	PxCooking* PhysicsShapeBase::Impl::GetCooking(PhysicsScene* scene)
	{
		return scene->_impl->GetCooking();
	}

	void PhysicsShapeBase::Impl::Release()
	{
		if (_shape != nullptr)
		{
			_shape->release();
			_shape = nullptr;
		}
	}
}