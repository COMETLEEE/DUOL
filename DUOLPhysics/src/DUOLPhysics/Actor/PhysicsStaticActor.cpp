#include "PhysicsStaticActorImpl.h"

/* etc */
#include <iostream>
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
	PhysicsStaticActor::PhysicsStaticActor() :
		_impl(std::make_shared<Impl>())
	{
		SetImpl(_impl);
	}

	PhysicsStaticActor::~PhysicsStaticActor()
	{

	}
}