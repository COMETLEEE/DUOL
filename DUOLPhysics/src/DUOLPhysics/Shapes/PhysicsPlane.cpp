#include "PhysicsPlaneImpl.h"

/* System */
#include "../System/PhysicsSystemImpl.h"

namespace DUOLPhysics
{
	PhysicsPlane::PhysicsPlane() :
		_impl(std::make_shared<Impl>())
	{

	}
}