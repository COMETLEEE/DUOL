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
		PxRigidActor* GetActor() const override;

		/**
			@brief	 Actor ��ü Getter
			@details -
			@retval  _actor
		**/
		PxRigidDynamic* GetDynamicActor() const;

		/**
			@brief	 �߷� ���� ���� üũ
			@details -
			@retval  ���� ���̸� true, �ƴϸ� false
		**/
		bool GetGravityEnable() const;

		/**
			@brief	 Mass�� �߽� Getter
			@details -
			@retval  Mass�� �߽� Local ��ǥ
		**/
		const DUOLMath::Vector3& GetCenterOfMass() const;

		/**
			@brief	 Mass�� �߽� Setter
			@details -
			@param   localPose - Mass�� �߽� Local ��ǥ
		**/
		void SetCenterOfMass(const DUOLMath::Vector3& localPose);

		/**
			@brief	 �߷� ���� / ����
			@details -
			@param   useGravity - true�� ����, false�� ����
		**/
		void SetGravityEnable(bool useGravity);

		/**
			@brief	 ��ü�� �������� ���� ����
			@details -
			@param   force - force ��
		**/
		void AddForce(const DUOLMath::Vector3& force);

		/**
			@brief	 ��ü�� �������� ���� ����
			@details -
			@param   impulse - impulse ��
		**/
		void AddImpulse(const DUOLMath::Vector3& impulse);

		/**
			@brief	 Force �ʱ�ȭ
			@details -
		**/
		void ResetForce();

		/**
			@brief	 ��ü�� �������� ���� ����
			@details -
			@param   torque - torque ��
		**/
		void AddTorque(const DUOLMath::Vector3& torque);

		/**
			@brief	 Torque �ʱ�ȭ
			@details -
		**/
		void ResetTorque();

		/**
			@brief	 Axis Lock ���� Ȯ��
			@details -
			@param   targetAxis - ���� Ȯ���� Axis
			@retval  Lock�Ǿ����� true, �ƴϸ� false
		**/
		bool GetAxisLockState(AxisLock targetAxis) const;

		/**
			@brief	 Axis ��� / ����
			@details -
			@param   targetAxis - ��� Axis 
			@param   isLock     - true�� ���, false�� ����
		**/
		void SetAxisLock(AxisLock targetAxis, bool isLock);

		/**
			@brief	 Linear Velocity Getter
			@details -
			@retval  Linear Velocity
		**/
		DUOLMath::Vector3 GetLinearVelocity() const;

		/**
			@brief	 Linear Velocity Setter
			@details -
			@param   velocity - �ӵ� ��
		**/
		void SetLinearVelocity(const DUOLMath::Vector3& velocity);

		/**
			@brief`	 Angular Velocity Getter
			@details -
			@retval  Angular Velocity
		**/
		DUOLMath::Vector3 GetAngularVelocity() const;

		/**
			@brief	 Angular Velocity Setter
			@details -
			@param   Angular velocity - �ӵ� ��
		**/
		void SetAngularVelocity(const DUOLMath::Vector3& velocity);

		/**
			@brief	 Mass Getter
			@details -
			@retval  Mass
		**/
		float GetMass() const;

		/**
			@brief	 Inverse Mass Getter
			@details -
			@retval  Inverse Mass
		**/
		float GetInverseMass() const;

		/**
			@brief	 Mass Setter
			@details -
			@param   mass - ���� ��
		**/
		void SetMass(float mass);

		/**
			@brief	 Actor ��ü�� �߽�, �е� ���� ����
			@details -
			@param   density               - �е�
			@param   massLocalPose         - ������ �߽�
			@param   isIncludedNonSimShape - Simulation flag�� �ƴ� ������ ������ ���ΰ��� ���� ��
		**/
		void SetMassAndInertia(float density, const DUOLMath::Vector3& massLocalPose, bool isIncludedNonSimShape);

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