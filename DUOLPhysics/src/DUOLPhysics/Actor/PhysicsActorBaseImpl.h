/**

	@file    PhysicsActorBaseImpl.h
	@brief   Physics Actor Base의 Implementation 클래스
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
		@brief	 Physics Actor Base의 Implementation 클래스
		@details -

	**/
	class PhysicsActorBase::Impl
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

	protected:
		PhysicsUserData _userData;

	public:
		/**
			@brief	 Child 클래스의 Actor Getter
			@details -
			@retval  PxRigidActor*
		**/
		virtual PxRigidActor* GetActor() = 0;

		/**
			@brief	 Actor의 Global Pose Getter
			@details -
			@retval  Global Pose
		**/
		GlobalPose GetGlobalPose();

		/**
			@brief	 Actor의 Global Pose Setter
			@details -
			@param   worldPosition - Set할 World Position 값
		**/
		void SetGlobalPose(const DUOLMath::Vector3& worldPosition);

		/**
			@brief	 Actor의 Global Pose Setter
			@details -
			@param   quat - Set할 Quaternion 값
		**/
		void SetGlobalPose(const DUOLMath::Quaternion& quat);

		/**
			@brief	 Actor의 Global Pose Setter
			@details -
			@param   transform - Set할 transform 값
		**/
		void SetGlobalPose(const DUOLMath::Matrix& transform);

		/**
			@brief	 Actor의 Global Pose Setter
			@details -
			@param   globalPose - Set할 positon 및 quaternion 값
		**/
		void SetGlobalPose(const GlobalPose& globalPose);

		/**
			@brief	 생성된 Actor 객체의 Boungding Box Getter
			@details -
			@param   inflation - Boungding Box 크기 조절
			@retval  3차원 공간상의 두 정점(최소 값, 최대 값)
		**/
		PhysicsBoundingBox GetBoundingBox(float inflation);

		/**
			@brief	 Actor에 도형 부착
			@details -
			@param   shape - 부착할 Shape
		**/
		void AttachShape(PxShape* shape);

		/**
			@brief	 Actor에서 도형 탈착
			@details -
			@param   shape             - 탈착할 Shape
			@param   isWakeOnLostTouch -
		**/
		void DetachShape(PxShape* shape, bool isWakeOnLostTouch);

		/**
			@brief
			@details -
			@param   userData -
		**/
		void SetUserData(void* userData);

		/**
			@brief
			@details -
			@param   enter -
		**/
		void SetTriggerEnterEvent(TriggerEvent enter);

		/**
			@brief
			@details -
			@param   stay -
		**/
		void SetTriggerStayEvent(TriggerEvent stay);

		/**
			@brief
			@details -
			@param   exit -
		**/
		void SetTriggerExitEvent(TriggerEvent exit);

		/**
			@brief
			@details -
			@param   enter -
		**/
		void SetCollisionEnterEvent(CollisionEvent enter);

		/**
			@brief
			@details -
			@param   stay -
		**/
		void SetCollisionStayEvent(CollisionEvent stay);

		/**
			@brief
			@details -
			@param   exit -
		**/
		void SetCollisionExitEvent(CollisionEvent exit);
	};
}