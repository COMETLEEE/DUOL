#pragma once
#ifndef _SILENCE_CXX20_CISO646_REMOVED_WARNING
#define _SILENCE_CXX20_CISO646_REMOVED_WARNING
#endif

#include "DUOLPhysics/Shapes/PhysicsBox.h"
#include "PxPhysicsAPI.h"

namespace DUOLPhysics
{
	using namespace physx;

	class PhysicsBox::Impl
	{
	public:
		Impl();

		~Impl();

	private:
		PxShape* _shape;

	public:
		void Create(PxPhysics* physics, const PhysicsShapeDesc& shapeDesc);

		void Release();
	};
}