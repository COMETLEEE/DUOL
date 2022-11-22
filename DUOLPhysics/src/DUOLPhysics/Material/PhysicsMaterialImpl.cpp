#include "PhysicsMaterialImpl.h"

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
	PhysicsMaterial::Impl::Impl() :
		_material(nullptr)
	{

	}

	PhysicsMaterial::Impl::~Impl()
	{
		Release();
	}

	void PhysicsMaterial::Impl::Create(PxPhysics* physics, const PhysicsMaterialDesc& materialDesc)
	{
		if (physics == nullptr)
			ERROR_THROW("Failed to create PxMaterial. (No PxPhysics.)");

		_material = physics->createMaterial(materialDesc._staticFriction, materialDesc._dynamicFriction, materialDesc._restitution);

		if (_material == nullptr)
			ERROR_THROW("Failed to create PxMaterial.");

		_material->userData = nullptr;
	}

	PxMaterial* PhysicsMaterial::Impl::GetMaterial() const
	{
		return _material;
	}

	void PhysicsMaterial::Impl::Release()
	{
		if (_material != nullptr)
		{
			_material->release();
			_material = nullptr;
		}
	}
}