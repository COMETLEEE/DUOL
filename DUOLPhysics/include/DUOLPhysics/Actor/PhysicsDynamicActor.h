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
		void SetMassAndInertia(float density, DUOLMath::Vector3 massLocalPose = DUOLMath::Vector3::Zero, bool isIncludedNonSimShape = false);

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