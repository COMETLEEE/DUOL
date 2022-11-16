/**

	@file    PhysicsActorBaseImpl.h
	@brief   Physics Actor Base�� Implementation Ŭ����
	@details -
	@author  JKim
	@date    4.11.2022

**/
#pragma once
#ifndef _SILENCE_CXX20_CISO646_REMOVED_WARNING
#define _SILENCE_CXX20_CISO646_REMOVED_WARNING
#endif

#include "PxPhysicsAPI.h"
#include "PhysicsUserData.h"
#include "DUOLPhysics/Actor/PhysicsActorBase.h"

namespace DUOLPhysics
{
	using namespace physx;

	/**

		@class   Impl
		@brief	 Physics Actor Base�� Implementation Ŭ����
		@details -

	**/
	class PhysicsActorBase::Impl
	{
	protected:
		PhysicsUserData _userData;

	public:
		/**
			@brief	 Child Ŭ������ Actor Getter
			@details -
			@retval  PxRigidActor*
		**/
		virtual PxRigidActor* GetActor() const = 0;

		/**
			@brief	 Simulation ���� ���� üũ
			@details -
			@retval  ���� ���̸� true, �ƴϸ� false
		**/
		bool GetSimulationEnable() const;

		/**
			@brief	 Simulation ����
			@details -
			@param   useSimulation - true�� ����, false�� ����
		**/
		void SetSimulationEnable(bool useSimulation);

		/**
			@brief	 Actor�� Global Pose Getter
			@details -
			@retval  Global Pose
		**/
		PhysicsPose GetGlobalPose() const;

		/**
			@brief	 Actor�� Global Pose Setter
			@details -
			@param   worldPosition - Set�� World Position ��
		**/
		void SetGlobalPose(const DUOLMath::Vector3& worldPosition);

		/**
			@brief	 Actor�� Global Pose Setter
			@details -
			@param   quat - Set�� Quaternion ��
		**/
		void SetGlobalPose(const DUOLMath::Quaternion& quat);

		/**
			@brief	 Actor�� Global Pose Setter
			@details -
			@param   transform - Set�� transform ��
		**/
		void SetGlobalPose(const DUOLMath::Matrix& transform);

		/**
			@brief	 Actor�� Global Pose Setter
			@details -
			@param   globalPose - Set�� positon �� quaternion ��
		**/
		void SetGlobalPose(const PhysicsPose& globalPose);

		/**
			@brief	 ������ Actor ��ü�� Boungding Box Getter
			@details -
			@param   inflation - Boungding Box ũ�� ����
			@retval  3���� �������� �� ����(�ּ� ��, �ִ� ��)
		**/
		PhysicsBoundingBox GetBoundingBox(float inflation) const;

		/**
			@brief	 Actor�� ���� ����
			@details -
			@param   shape - ������ Shape
		**/
		void AttachShape(PxShape* shape);

		/**
			@brief	 Actor���� ���� Ż��
			@details -
			@param   shape             - Ż���� Shape
			@param   isWakeOnLostTouch -
		**/
		void DetachShape(PxShape* shape, bool isWakeOnLostTouch);

		/**
			@brief	 Physics Space������ Actor�� �ܺ� Object���� ����
			@details -
			@param   userData - Actor�� �����Ǵ� ��ü�� Pointer
		**/
		void SetUserData(void* userData);

		/**
			@brief	 Trigger�� Actor�� �浹 �߻� ������ ȣ��Ǵ� Event
			@details -
			@param   enter - Enter Event
		**/
		void SetTriggerEnterEvent(TriggerEvent enter);

		/**
			@brief	 Trigger�� Actor�� �浹 ���� �ÿ� ȣ��Ǵ� Event
			@details -
			@param   stay - Stay Event
		**/
		void SetTriggerStayEvent(TriggerEvent stay);

		/**
			@brief	 Trigger�� Actor�� �浹�� ������ ������ ȣ��Ǵ� Event
			@details -
			@param   exit - Exit Event
		**/
		void SetTriggerExitEvent(TriggerEvent exit);

		/**
			@brief	 Actor�� �浹 �߻� ������ ȣ��Ǵ� Event
			@details -
			@param   enter - Enter Event
		**/
		void SetCollisionEnterEvent(CollisionEvent enter);

		/**
			@brief	 Actor�� �浹 ���� �ÿ� ȣ��Ǵ� Event
			@details -
			@param   stay - Stay Event
		**/
		void SetCollisionStayEvent(CollisionEvent stay);

		/**
			@brief	 Actor�� �浹�� ������ ������ ȣ��Ǵ� Event
			@details -
			@param   exit - Exit Event
		**/
		void SetCollisionExitEvent(CollisionEvent exit);
	};
}