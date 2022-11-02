#include "PhysicsDynamicActorImpl.h"

/* etc */
#include "../Util/PhysicsTypeConverter.h"

#include <iostream>

namespace DUOLPhysics
{
	PhysicsDynamicActor::PhysicsDynamicActor() :
		_impl(std::make_shared<Impl>())
	{

	}

	PhysicsDynamicActor::~PhysicsDynamicActor()
	{

	}

	void PhysicsDynamicActor::SetLinearVelocity(DUOLMath::Vector3 velocity)
	{
		auto actor = _impl->GetActor();

		if (actor != nullptr)
			actor->setLinearVelocity(ConvertVector3(velocity));
	}

	void PhysicsDynamicActor::SetMassAndInertia(float density)
	{
		auto actor = _impl->GetActor();

		if (actor != nullptr)
			PxRigidBodyExt::updateMassAndInertia(*actor, density);
	}

	void PhysicsDynamicActor::SetAngularDamping(float angDamp)
	{
		auto actor = _impl->GetActor();
		
		if (actor != nullptr)
			actor->setAngularDamping(angDamp);
	}

	PhysicsBoundingBox PhysicsDynamicActor::GetBoundingBox(float inflation)
	{
		inflation = 10;
		return PhysicsBoundingBox();
	}

	void PhysicsDynamicActor::AttachShape(std::weak_ptr<PhysicsShapeBase> shape)
	{
		try
		{
			if (shape.expired() != true)
				shape.lock()->Attachment(this);
		}
		catch (const std::string& errStr)
		{
			std::cerr << errStr << std::endl;
		}
		catch (...)
		{
			std::cerr << "Unknown Error." << std::endl;
		}
	}

	void PhysicsDynamicActor::DetachShape(std::weak_ptr<PhysicsShapeBase> shape, bool isWakeOnLostTouch)
	{
		if (isWakeOnLostTouch)
			shape = {};
	}

	void PhysicsDynamicActor::Release()
	{
		if (_impl != nullptr)
		{
			_impl->Release();
			_impl = nullptr;
		}
	}
}