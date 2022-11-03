#include "PhysicsShapeBaseImpl.h"
#include "../PhysicsMaterialImpl.h"
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
	PhysicsShapeBase::Impl::Impl() :
		_shape(nullptr)
	{

	}

	PhysicsShapeBase::Impl::~Impl()
	{
		Release();
	}

	void PhysicsShapeBase::Impl::Create(PxPhysics* physics, const PhysicsShapeDesc& shapeDesc)
	{
		if (physics == nullptr)
			ERROR_THROW("Failed to create PxBox. (No PxPhysics.)");

		PxMaterial* material = shapeDesc._material.lock()->_impl->GetMaterial();

		if (material == nullptr)
			ERROR_THROW("Failed to create PxBox. (No PxMaterial.)");

		PxBoxGeometry boxGeometry(shapeDesc._box._x, shapeDesc._box._y, shapeDesc._box._z);
		_shape = physics->createShape(boxGeometry, *material, shapeDesc._isExclusive, ConvertShapeFlags(shapeDesc._flag));

		if (_shape == nullptr)
			ERROR_THROW("Failed to create PxBox.");
	}

	PxShape* PhysicsShapeBase::Impl::GetShape()
	{
		return _shape;
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