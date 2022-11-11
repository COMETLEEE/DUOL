#include "PhysicsShapeBaseImpl.h"

/* Actor */
#include "../Actor/PhysicsDynamicActorImpl.h"

/* etc */
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
	PhysicsShapeBase::PhysicsShapeBase() :
		_impl(std::make_shared<Impl>())
	{

	}

	PhysicsShapeBase::~PhysicsShapeBase()
	{
		Release();
	}

	void PhysicsShapeBase::Release()
	{
		if (_impl != nullptr)
			_impl->Release();
	}
}