#include "DUOLPhysics/PhysicsMaterial.h"

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
	PhysicsMaterial::PhysicsMaterial() :
		_material(nullptr)
	{

	}

	void PhysicsMaterial::CreateMaterial(HidedPhysicsMaterialDesc& materialDesc)
	{
		if (materialDesc._physics == nullptr)
			ERROR_THROW("Failed to create PxMaterial. (No PxPhysics.)");

		PhysicsMaterialDesc& desc = materialDesc._default;

		_material = materialDesc._physics->createMaterial(desc._staticFriction, desc._dynamicFriction, desc._restitution);

		if (_material == nullptr)
			ERROR_THROW("Failed to create PxMaterial.");
	}

	void PhysicsMaterial::Release()
	{
		if (_material != nullptr)
		{
			_material->release();
			_material = nullptr;
		}
	}
}