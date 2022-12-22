#include "PhysicsDynamicActorImpl.h"

/* etc */
#include "../Util/PhysicsTypeConverter.h"
#include "DUOLPhysics/Util/PhysicsDefines.h"

namespace DUOLPhysics
{
	PhysicsDynamicActor::Impl::Impl() :
		_actor(nullptr)
	{

	}

	PxRigidDynamic* PhysicsDynamicActor::Impl::Create(PxPhysics* physics, const PhysicsActorDesc& dynamicDesc)
	{
		if (physics == nullptr)
			ERROR_THROW("Failed to create Dynamic Actor. (No PxPhysics.)");

		_actor = physics->createRigidDynamic(ConvertTransform(dynamicDesc._transform));

		if (_actor == nullptr)
			ERROR_THROW("Failed to create Dynamic Actor.");

		_actor->userData = nullptr;

		return _actor;
	}

	PxRigidActor* PhysicsDynamicActor::Impl::GetActor() const
	{
		return _actor;
	}

	PxRigidDynamic* PhysicsDynamicActor::Impl::GetDynamicActor() const
	{
		return _actor;
	}

	bool PhysicsDynamicActor::Impl::GetGravityEnable() const
	{
		if (_actor == nullptr)
			ERROR_THROW("Failed to get Gravity Enable.");

		return !(_actor->getActorFlags() & PxActorFlag::eDISABLE_GRAVITY);
	}

	DUOLMath::Vector3 PhysicsDynamicActor::Impl::GetCenterOfMass() const
	{
		if (_actor == nullptr)
			ERROR_THROW("Failed to get Center of mass.");

		const auto& cmassPose = _actor->getCMassLocalPose();

		return ConvertVector3(cmassPose.p);
	}

	void PhysicsDynamicActor::Impl::SetCenterOfMass(const DUOLMath::Vector3& localPose)
	{
		if (_actor == nullptr)
			ERROR_THROW("Failed to set Center of mass.");

		_actor->setCMassLocalPose(PxTransform(ConvertVector3(localPose)));
	}

	void PhysicsDynamicActor::Impl::SetGravityEnable(bool useGravity)
	{
		if (_actor == nullptr)
			ERROR_THROW("Failed to set Gravity Enable.");

		_actor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, !useGravity);
	}

	void PhysicsDynamicActor::Impl::AddForce(const DUOLMath::Vector3& force)
	{
		if (_actor == nullptr)
			ERROR_THROW("Failed to Add Force.");

		_actor->addForce(ConvertVector3(force));
	}

	void PhysicsDynamicActor::Impl::AddImpulse(const DUOLMath::Vector3& impulse)
	{
		if (_actor == nullptr)
			ERROR_THROW("Failed to Add Impulse.");

		_actor->addForce(ConvertVector3(impulse), PxForceMode::Enum::eIMPULSE);
	}

	void PhysicsDynamicActor::Impl::ResetForce()
	{
		if (_actor == nullptr)
			ERROR_THROW("Failed to Reset Force.");

		_actor->clearForce();
	}

	void PhysicsDynamicActor::Impl::AddTorque(const DUOLMath::Vector3& torque)
	{
		if (_actor == nullptr)
			ERROR_THROW("Failed to Add Torque.");

		_actor->addTorque(ConvertVector3(torque));
	}

	void PhysicsDynamicActor::Impl::ResetTorque()
	{
		if (_actor == nullptr)
			ERROR_THROW("Failed to Reset Torque.");

		_actor->clearTorque();
	}

	bool PhysicsDynamicActor::Impl::GetAxisLockState(AxisLock targetAxis) const
	{
		if (_actor == nullptr)
			ERROR_THROW("Failed to get Axis Lock.");

		return _actor->getRigidDynamicLockFlags() & static_cast<PxRigidDynamicLockFlag::Enum>(targetAxis);
	}

	AxisLockFlags PhysicsDynamicActor::Impl::GetAxesLockState() const
	{
		if (_actor == nullptr)
			ERROR_THROW("Failed to get Axis Lock.");

		return static_cast<AxisLockFlags>(_actor->getRigidDynamicLockFlags());
	}

	void PhysicsDynamicActor::Impl::SetAxisLock(AxisLock targetAxis, bool isLock)
	{
		if (_actor == nullptr)
			ERROR_THROW("Failed to set Axis Lock.");

		_actor->setRigidDynamicLockFlag(static_cast<PxRigidDynamicLockFlag::Enum>(targetAxis), isLock);
	}

	void PhysicsDynamicActor::Impl::SetAxesLock(AxisLockFlags targetAxes)
	{
		if (_actor == nullptr)
			ERROR_THROW("Failed to set Axes Lock.");

		_actor->setRigidDynamicLockFlags(static_cast<PxRigidDynamicLockFlags>(targetAxes.GetBitMask()));
	}

	DUOLMath::Vector3 PhysicsDynamicActor::Impl::GetLinearVelocity() const
	{
		if (_actor == nullptr)
			ERROR_THROW("Failed to get Linear Velocity.");

		return ConvertVector3(_actor->getLinearVelocity());
	}

	void PhysicsDynamicActor::Impl::SetLinearVelocity(const DUOLMath::Vector3& velocity)
	{
		if (_actor == nullptr)
			ERROR_THROW("Failed to set Linear Velocity.");

		_actor->setLinearVelocity(ConvertVector3(velocity));
	}

	DUOLMath::Vector3 PhysicsDynamicActor::Impl::GetAngularVelocity() const
	{
		if (_actor == nullptr)
			ERROR_THROW("Failed to get Angular Velocity.");

		return ConvertVector3(_actor->getAngularVelocity());
	}

	void PhysicsDynamicActor::Impl::SetAngularVelocity(const DUOLMath::Vector3& velocity)
	{
		if (_actor == nullptr)
			ERROR_THROW("Failed to set Angular Velocity.");

		_actor->setAngularVelocity(ConvertVector3(velocity));
	}

	float PhysicsDynamicActor::Impl::GetMass() const
	{
		if (_actor == nullptr)
			ERROR_THROW("Failed to get Mass.");

		return _actor->getMass();
	}

	float PhysicsDynamicActor::Impl::GetInverseMass() const
	{
		if (_actor == nullptr)
			ERROR_THROW("Failed to get Inverse Mass.");

		return _actor->getInvMass();
	}

	void PhysicsDynamicActor::Impl::SetMass(float mass)
	{
		if (_actor == nullptr)
			ERROR_THROW("Failed to set Mass.");

		_actor->setMass(mass);
	}

	void PhysicsDynamicActor::Impl::SetMassAndInertia(float density, const DUOLMath::Vector3& massLocalPose, bool isIncludedNonSimShape)
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