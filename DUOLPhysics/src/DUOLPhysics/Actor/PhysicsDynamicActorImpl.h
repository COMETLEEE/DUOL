/**

	@file    PhysicsDynamicActorImpl.h
	@brief	 Physics Dynamic Actor�� Implementation Ŭ����
	@details -
	@author  JKim
	@date    3.11.2022

**/
#pragma once
#ifndef _SILENCE_CXX20_CISO646_REMOVED_WARNING
#define _SILENCE_CXX20_CISO646_REMOVED_WARNING
#endif

#include "PxPhysicsAPI.h"
#include "PhysicsActorBaseImpl.h"
#include "DUOLPhysics/Actor/PhysicsDynamicActor.h"

namespace DUOLPhysics
{
	using namespace physx;

	/**

		@class   Impl
		@brief	 Physics Dynamic Actor�� Implementation Ŭ����
		@details -

	**/
	class PhysicsDynamicActor::Impl : public PhysicsActorBase::Impl
	{
	public:
		/**
			@brief   Impl Ŭ���� ������
			@details -
		**/
		Impl();

		/**
			@brief   Impl Ŭ���� default �Ҹ���
			@details -
		**/
		~Impl() = default;

	private:
		PxRigidDynamic* _actor;

	public:
		/**
			@brief	 ����ִ� Dynamic Actor ����
			@details -
			@param   physics     - Physics ��ü�� ���� ����
			@param   dynamicDesc - Dynamic Actor ������ �ʿ��� ��
			@retval  ������ DynamicActor ��ü
		**/
		PxRigidDynamic* Create(PxPhysics* physics, const PhysicsActorDesc& dynamicDesc);

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
		PxRigidDynamic* GetDynamicActor();

		/**
			@brief	 Velocity��ŭ ���� ����
			@details -
			@param   velocity - ���� ���� ��
		**/
		void SetLinearVelocity(DUOLMath::Vector3 velocity);

		/**
			@brief	 Actor ��ü�� �߽�, �е� ���� ����
			@details -
			@param   density               - �е�
			@param   massLocalPose         - ������ �߽�
			@param   isIncludedNonSimShape - Simulation flag�� �ƴ� ������ ������ ���ΰ��� ���� ��
		**/
		void SetMassAndInertia(float density, DUOLMath::Vector3 massLocalPose, bool isIncludedNonSimShape);

		/**
			@brief	 �̵��� ���� ���׷�(���� ����) ����
			@details ������ 0���� ó����.
			@param   linDamp - ���� ���� ��
		**/
		void SetLinearDamping(float linDamp);

		/**
			@brief	 ȸ���� ���� ���׷�(���� ����) ����
			@details ������ 0���� ó����.
			@param   angDamp - ���� ���� ��
		**/
		void SetAngularDamping(float angDamp);
	};
}