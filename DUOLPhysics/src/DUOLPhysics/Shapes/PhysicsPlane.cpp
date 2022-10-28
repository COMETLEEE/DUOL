#include "PhysicsPlaneImpl.h"
#include "../System/PhysicsSystemImpl.h"

namespace DUOLPhysics
{
	PhysicsPlane::PhysicsPlane() :
		_impl(std::make_shared<Impl>())
	{

	}

	void PhysicsPlane::Release()
	{
		if (_impl != nullptr)
			_impl = nullptr;
	}
}