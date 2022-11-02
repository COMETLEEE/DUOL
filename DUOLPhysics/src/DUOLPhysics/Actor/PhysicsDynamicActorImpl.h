#pragma once
#ifndef _SILENCE_CXX20_CISO646_REMOVED_WARNING
#define _SILENCE_CXX20_CISO646_REMOVED_WARNING
#endif

#include "PxPhysicsAPI.h"
#include "DUOLPhysics/Actor/PhysicsDynamicActor.h"

namespace DUOLPhysics
{
	using namespace physx;

	class PhysicsDynamicActor::Impl
	{
	public:
		/**
			@brief   Impl Ŭ���� ������
			@details -
		**/
		Impl();

		/**
			@brief   Impl Ŭ���� �Ҹ���
			@details -
		**/
		~Impl();

	private:
		PxRigidDynamic* _actor;

	public:
		PxRigidDynamic* Create(PxPhysics* physics, const PhysicsDynamicDesc& dynamicDesc);

		PxRigidDynamic* GetActor();

		void Release();
	};
}