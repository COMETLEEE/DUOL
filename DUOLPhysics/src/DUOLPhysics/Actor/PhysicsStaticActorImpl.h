
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
		@brief	 Physics Dynamic Actor�� Implementation Ŭ����
		@details -

	**/
	class PhysicsStaticActor::Impl : public PhysicsActorBase::Impl
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
		PxRigidStatic* _actor;

	public:
		/**
			@brief	 ����ִ� Static Actor ����
			@details -
			@param   physics    - Physics ��ü�� ���� ����
			@param   staticDesc - Static Actor ������ �ʿ��� ��
			@retval  ������ StaticActor ��ü
		**/
		PxRigidStatic* Create(PxPhysics* physics, const PhysicsActorDesc& staticDesc);

		/**
			@brief	 ��ӵ� Actor ��ȯ �Լ�
			@details -
			@retval  PxRigidActor*
		**/
		PxRigidActor* GetActor() override;

		/**
			@brief	 Actor ��ü Getter
			@details -
			@retval  _actor
		**/
		PxRigidStatic* GetStaticActor();
	};
}