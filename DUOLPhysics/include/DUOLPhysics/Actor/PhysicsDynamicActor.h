#pragma once
#include "PhysicsActorBase.h"

#include <memory>

namespace DUOLPhysics
{
	class PhysicsScene;

	class PhysicsBox;

	class PhysicsDynamicActor : public PhysicsActorBase
	{
		class Impl;

		friend PhysicsScene;
		friend PhysicsBox;

	public:
		PhysicsDynamicActor();

		~PhysicsDynamicActor();

	private:
		std::shared_ptr<Impl> _impl;

	public:
		void SetLinearVelocity(DUOLMath::Vector3 velocity);

		void SetMassAndInertia(float density);

		void SetAngularDamping(float angDamp);

		PhysicsBoundingBox GetBoundingBox(float inflation = 1.01f) override;

		void AttachShape(std::weak_ptr<PhysicsShapeBase> shape) override;

		void DetachShape(std::weak_ptr<PhysicsShapeBase> shape, bool isWakeOnLostTouch = true) override;

		void Release() override;
	};
}