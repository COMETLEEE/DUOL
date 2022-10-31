#pragma once
#include "PhysicsShapeBase.h"

#include <memory>

namespace DUOLPhysics
{
	class PhysicsSystem;

	class PhysicsBox : public PhysicsShapeBase
	{
		class Impl;

	public:
		PhysicsBox();

		~PhysicsBox();

	private:
		std::shared_ptr<Impl> _impl;

	public:
		void Create(PhysicsSystem* system, const PhysicsShapeDesc& shapeDesc) override;

		void Release() override;
	};
}  