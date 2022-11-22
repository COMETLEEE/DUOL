#include "PhysicsStaticActorImpl.h"

/* etc */
#include "DUOLPhysics/Util/PhysicsDefines.h"

#include <iostream>

namespace DUOLPhysics
{
	PhysicsStaticActor::PhysicsStaticActor() :
		_impl(std::make_shared<Impl>())
	{
		SetImpl(_impl);
	}
}