#include "PhysicsPlaneImpl.h"

/* etc */
#include "DUOLPhysics/Util/PhysicsDefines.h"

namespace DUOLPhysics
{
	PhysicsPlane::Impl::Impl() :
		_plane(nullptr)
	{

	}
	PhysicsPlane::Impl::~Impl()
	{
		Release();
	}

	PxRigidStatic* PhysicsPlane::Impl::Create(PxPhysics* physics, PxMaterial* material, const PhysicsPlaneDesc& planeDesc)
	{
		if (physics == nullptr)
			ERROR_THROW("Failed to create PxPlane. (No PxPhysics.)");

		if (material == nullptr)
			ERROR_THROW("Failed to create PxPlane. (No PxMaterial.)");

		_plane = PxCreatePlane(*physics, PxPlane(planeDesc._normal.x, planeDesc._normal.y, planeDesc._normal.z, planeDesc._distance), *material);

		if (_plane == nullptr)
			ERROR_THROW("Failed to create PxPlane.");

		_plane->userData = nullptr;

		return _plane;
	}

	void PhysicsPlane::Impl::Release()
	{
		if (_plane != nullptr)
		{
			_plane->release();
			_plane = nullptr;
		}
	}
}