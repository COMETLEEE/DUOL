#include "PhysicsStaticActorImpl.h"

/* etc */
#include "../Util/PhysicsTypeConverter.h"
#include "DUOLPhysics/Util/PhysicsDefines.h"

namespace DUOLPhysics
{
	PhysicsStaticActor::Impl::Impl() :
		_actor(nullptr)
	{

	}

	PxRigidStatic* PhysicsStaticActor::Impl::Create(PxPhysics* physics, const PhysicsActorDesc& staticDesc)
	{
		if (physics == nullptr)
			ERROR_THROW("Failed to create Static Actor. (No PxPhysics.)");

		_actor = physics->createRigidStatic(ConvertTransform(staticDesc._transform));

		if (_actor == nullptr)
			ERROR_THROW("Failed to create Static Actor.");

		_actor->userData = nullptr;

		return _actor;
	}

	PxRigidActor* PhysicsStaticActor::Impl::GetActor() const
	{
		return _actor;
	}

	PxRigidStatic* PhysicsStaticActor::Impl::GetStaticActor() const
	{
		return _actor;
	}
}