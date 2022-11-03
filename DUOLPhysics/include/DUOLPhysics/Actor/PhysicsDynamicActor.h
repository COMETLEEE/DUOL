#pragma once
#include "PhysicsActorBase.h"

#include <memory>

namespace DUOLPhysics
{
	class PhysicsScene;
	class PhysicsShapeBase;

	class PhysicsDynamicActor : public PhysicsActorBase
	{
		class Impl;

		friend PhysicsScene;
		friend PhysicsShapeBase;

	public:
		PhysicsDynamicActor();

		~PhysicsDynamicActor();

	private:
		std::shared_ptr<Impl> _impl;

	public:
		void SetLinearVelocity(DUOLMath::Vector3 velocity);

		void SetMassAndInertia(float density, DUOLMath::Vector3 massLocalPose = DUOLMath::Vector3::Zero, bool isIncludedNonSimShape = false);

		void SetLinearDamping(float linDamp);

		void SetAngularDamping(float angDamp);

		PhysicsBoundingBox GetBoundingBox(float inflation = 1.01f) override;

		void AttachShape(std::weak_ptr<PhysicsShapeBase> shape) override;

		void DetachShape(std::weak_ptr<PhysicsShapeBase> shape, bool isWakeOnLostTouch = true) override;

		void Release() override;
	};
}