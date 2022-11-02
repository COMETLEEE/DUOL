#include "PhysicsDynamicActorImpl.h"

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
	PhysicsDynamicActor::Impl::Impl() :
		_actor(nullptr)
	{

	}

	PhysicsDynamicActor::Impl::~Impl()
	{

	}

	PxRigidDynamic* PhysicsDynamicActor::Impl::Create(PxPhysics* physics, const PhysicsDynamicDesc& dynamicDesc)
	{
		if (physics == nullptr)
			ERROR_THROW("Failed to create Dynamic Actor. (No PxPhysics.)");

		_actor = physics->createRigidDynamic(ConvertTransform(dynamicDesc._transform));

		if (_actor == nullptr)
			ERROR_THROW("Failed to create Dynamic Actor.");

		return _actor;
	}

	PxRigidDynamic* PhysicsDynamicActor::Impl::GetActor()
	{
		return _actor;
	}

	void PhysicsDynamicActor::Impl::Release()
	{

	}
}