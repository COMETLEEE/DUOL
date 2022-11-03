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

		SetActor(_actor);

		return _actor;
	}

	PxRigidDynamic* PhysicsDynamicActor::Impl::GetActor()
	{
		return _actor;
	}

	void PhysicsDynamicActor::Impl::SetLinearVelocity(DUOLMath::Vector3 velocity)
	{
		if (_actor == nullptr)
			ERROR_THROW("Failed to set Linear Velocity.");

		_actor->setLinearVelocity(ConvertVector3(velocity));
	}

	void PhysicsDynamicActor::Impl::SetMassAndInertia(float density, DUOLMath::Vector3 massLocalPose, bool isIncludedNonSimShape)
	{
		if (_actor == nullptr)
			ERROR_THROW("Failed to set Mass and Inertia.");

		PxVec3* pxMassLocal = nullptr;
		auto pxVec = ConvertVector3(massLocalPose);

		if (massLocalPose != DUOLMath::Vector3::Zero)
			pxMassLocal = &pxVec;

		if (PxRigidBodyExt::updateMassAndInertia(*_actor, density, pxMassLocal, isIncludedNonSimShape) != true)
			ERROR_THROW("Failed to update Mass and Inertia.");
	}

	void PhysicsDynamicActor::Impl::SetLinearDamping(float linDamp)
	{
		if (_actor == nullptr)
			ERROR_THROW("Failed to set Linear Damping.");

		_actor->setLinearDamping(linDamp);
	}

	void PhysicsDynamicActor::Impl::SetAngularDamping(float angDamp)
	{
		if (_actor == nullptr)
			ERROR_THROW("Failed to set Angular Damping.");

		_actor->setAngularDamping(angDamp);
	}
}