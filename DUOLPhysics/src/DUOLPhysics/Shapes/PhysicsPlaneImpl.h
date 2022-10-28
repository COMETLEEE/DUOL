#pragma once
#include "DUOLPhysics/Shapes/PhysicsPlane.h"
#include "PxPhysicsAPI.h"

#ifndef _SILENCE_CXX20_CISO646_REMOVED_WARNING
#define _SILENCE_CXX20_CISO646_REMOVED_WARNING
#endif

namespace DUOLPhysics
{
	using namespace physx;

	class PhysicsPlane::Impl
	{
	public:
		Impl();

		~Impl();

	private:
		PxRigidStatic* _plane;

	public:
		PxRigidStatic* Create(PxPhysics* physics, PxMaterial* material, const PhysicsPlaneDesc& planeDesc);
	};
}