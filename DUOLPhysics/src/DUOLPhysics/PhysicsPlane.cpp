#include "DUOLPhysics/PhysicsPlane.h"

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
	PhysicsPlane::PhysicsPlane() :
		_plane(nullptr)
	{

	}

	PxRigidStatic* PhysicsPlane::CreatePlane(HidedPhysicsPlaneDesc& planeDesc)
	{
		if (planeDesc._physics == nullptr)
			ERROR_THROW("Failed to create PxPlane. (No PxPhysics.)");

		if (planeDesc._material == nullptr)
			ERROR_THROW("Failed to create PxPlane. (No PxMaterial.)");

		PhysicsPlaneDesc& desc = planeDesc._default;

		_plane = PxCreatePlane(*planeDesc._physics, PxPlane(desc._normal.x, desc._normal.y, desc._normal.z, desc._distance), *planeDesc._material);

		if (_plane == nullptr)
			ERROR_THROW("Failed to create PxPlane.");

		return _plane;
	}

	void PhysicsPlane::Release()
	{
		if (_plane != nullptr)
		{
			_plane->release();
			_plane = nullptr;
		}
	}
}