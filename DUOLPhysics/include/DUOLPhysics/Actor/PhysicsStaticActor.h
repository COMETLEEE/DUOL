
#pragma once
#include "PhysicsActorBase.h"

#include <memory>

namespace DUOLPhysics
{
	class PhysicsScene;
	class PhysicsShapeBase;

	class PhysicsStaticActor : public PhysicsActorBase
	{
		class Impl;

		friend PhysicsScene;
		friend PhysicsShapeBase;

	public:
		PhysicsStaticActor();

		~PhysicsStaticActor();

	private:
		std::shared_ptr<Impl> _impl;
	};
}