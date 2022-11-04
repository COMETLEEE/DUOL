
#pragma once
#ifndef _SILENCE_CXX20_CISO646_REMOVED_WARNING
#define _SILENCE_CXX20_CISO646_REMOVED_WARNING
#endif

#include "PxPhysicsAPI.h"
#include "PhysicsActorBaseImpl.h"
#include "DUOLPhysics/Actor/PhysicsStaticActor.h"

namespace DUOLPhysics
{
	using namespace physx;

	/**

		@class   Impl
		@brief	 Physics Dynamic Actor의 Implementation 클래스
		@details -

	**/
	class PhysicsStaticActor::Impl : public PhysicsActorBase::Impl
	{
	public:
		/**
			@brief   Impl 클래스 생성자
			@details -
		**/
		Impl();

		/**
			@brief   Impl 클래스 소멸자
			@details -
		**/
		~Impl();

	private:
		PxRigidStatic* _actor;

	public:
		/**
			@brief	 비어있는 Static Actor 생성
			@details -
			@param   physics    - Physics 객체를 통해 생성
			@param   staticDesc - Static Actor 생성에 필요한 값
			@retval  생성된 StaticActor 객체
		**/
		PxRigidStatic* Create(PxPhysics* physics, const PhysicsActorDesc& staticDesc);

		/**
			@brief	 상속된 Actor 반환 함수
			@details -
			@retval  PxRigidActor*
		**/
		PxRigidActor* GetActor() override;

		/**
			@brief	 Actor 객체 Getter
			@details -
			@retval  _actor
		**/
		PxRigidStatic* GetStaticActor();
	};
}