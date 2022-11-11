/**

	@file    PhysicsDynamicActor.h
	@brief	 Physics Space�� ���� ��ü
	@details -
	@author  JKim
	@date    3.11.2022

**/
#pragma once
#include "PhysicsActorBase.h"

#include <memory>

namespace DUOLPhysics
{
	class PhysicsScene;
	class PhysicsShapeBase;

	/**

		@class   PhysicsDynamicActor
		@brief	 Physics Space�� ���� ��ü
		@details -

	**/
	class PhysicsDynamicActor : public PhysicsActorBase
	{
		class Impl;

		friend PhysicsScene;
		friend PhysicsShapeBase;

	public:
		/**
			@brief   PhysicsDynamicActor Ŭ���� ������
			@details ������ ȣ��� Impl ����, �θ𿡰� Impl ������ ����
		**/
		PhysicsDynamicActor();

		/**
			@brief   PhysicsDynamicActor Ŭ���� default �Ҹ���
			@details -
		**/
		~PhysicsDynamicActor() = default;

	private:
		std::shared_ptr<Impl> _impl;

	public:
		/**
			@brief	 ��ü�� �������� ���� ����
			@details -
			@param   force - force ��
		**/
		void AddForce(const DUOLMath::Vector3& force);

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
			@param   Linear velocity - �ӵ� ��
		**/
		void SetLinearVelocity(const DUOLMath::Vector3& velocity);
		
		/**
			@brief	 Angular Velocity Getter
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
		void SetMassAndInertia(float density, const DUOLMath::Vector3& massLocalPose = DUOLMath::Vector3::Zero, bool isIncludedNonSimShape = false);

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