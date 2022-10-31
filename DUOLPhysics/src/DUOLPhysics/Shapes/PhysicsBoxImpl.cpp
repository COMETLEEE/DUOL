#include "PhysicsBoxImpl.h"
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
	PhysicsBox::Impl::Impl() :
		_shape(nullptr)
	{

	}

	PhysicsBox::Impl::~Impl()
	{
		Release();
	}

	void PhysicsBox::Impl::Create(PxPhysics* physics, const PhysicsShapeDesc& shapeDesc)
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

	void PhysicsBox::Impl::Release()
	{
		if (_shape != nullptr)
		{
			_shape->release();
			_shape = nullptr;
		}
	}
}