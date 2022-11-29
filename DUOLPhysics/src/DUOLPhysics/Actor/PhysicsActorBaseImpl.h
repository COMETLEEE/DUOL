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
	protected:
		PhysicsUserData _userData;

	public:
		/**
			@brief	 Child 클래스의 Actor Getter
			@details -
			@retval  PxRigidActor*
		**/
		virtual PxRigidActor* GetActor() const = 0;

		/**
			@brief	 Simulation 적용 상태 체크
			@details -
			@retval  적용 중이면 true, 아니면 false
		**/
		bool GetSimulationEnable() const;

		/**
			@brief	 Simulation 적용
			@details -
			@param   useSimulation - true면 적용, false면 해제
		**/
		void SetSimulationEnable(bool useSimulation);

		/**
			@brief	 Actor의 Global Pose Getter
			@details -
			@retval  Global Pose
		**/
		PhysicsPose GetGlobalPose() const;

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
		void SetGlobalPose(const PhysicsPose& globalPose);

		/**
			@brief	 생성된 Actor 객체의 Boungding Box Getter
			@details -
			@param   inflation - Boungding Box 크기 조절
			@retval  3차원 공간상의 두 정점(최소 값, 최대 값)
		**/
		PhysicsBoundingBox GetBoundingBox(float inflation) const;

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
			@brief	 Physics Space에서의 Actor와 외부 Object간의 연동
			@details -
			@param   userData - Actor와 대응되는 객체의 Pointer
		**/
		void SetUserData(void* userData);

		/**
			@brief	 Trigger와 Actor의 충돌 발생 시점에 호출되는 Event
			@details -
			@param   enter - Enter Event
		**/
		void SetTriggerEnterEvent(TriggerEvent enter);

		/**
			@brief	 Trigger와 Actor의 충돌 지속 시에 호출되는 Event
			@details -
			@param   stay - Stay Event
		**/
		void SetTriggerStayEvent(TriggerEvent stay);

		/**
			@brief	 Trigger와 Actor의 충돌이 끝나는 시점에 호출되는 Event
			@details -
			@param   exit - Exit Event
		**/
		void SetTriggerExitEvent(TriggerEvent exit);

		/**
			@brief	 Actor간 충돌 발생 시점에 호출되는 Event
			@details -
			@param   enter - Enter Event
		**/
		void SetCollisionEnterEvent(CollisionEvent enter);

		/**
			@brief	 Actor간 충돌 지속 시에 호출되는 Event
			@details -
			@param   stay - Stay Event
		**/
		void SetCollisionStayEvent(CollisionEvent stay);

		/**
			@brief	 Actor간 충돌이 끝나는 시점에 호출되는 Event
			@details -
			@param   exit - Exit Event
		**/
		void SetCollisionExitEvent(CollisionEvent exit);
	};
}