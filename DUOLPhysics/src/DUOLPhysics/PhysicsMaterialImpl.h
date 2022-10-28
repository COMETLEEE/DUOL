#pragma once
#include "DUOLPhysics/PhysicsMaterial.h"
#include "PxPhysicsAPI.h"

#ifndef _SILENCE_CXX20_CISO646_REMOVED_WARNING
#define _SILENCE_CXX20_CISO646_REMOVED_WARNING
#endif

namespace DUOLPhysics
{
	using namespace physx;

	class PhysicsMaterial::Impl
	{
	public:
		Impl();

		~Impl();

	private:
		PxMaterial* _material;

	public:
		void Create(PxPhysics* physics, const PhysicsMaterialDesc& materialDesc);

		PxMaterial* GetMaterial();

		void Release();
	};
}