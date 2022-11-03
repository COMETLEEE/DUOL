#include "PhysicsMaterialImpl.h"

namespace DUOLPhysics
{
	PhysicsMaterial::PhysicsMaterial() :
		_impl(std::make_shared<Impl>())
	{

	}

	void PhysicsMaterial::Release()
	{
		if (_impl != nullptr)
			_impl->Release();
	}
}