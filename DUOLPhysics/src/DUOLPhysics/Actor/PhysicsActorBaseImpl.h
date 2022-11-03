
#pragma once
#ifndef _SILENCE_CXX20_CISO646_REMOVED_WARNING
#define _SILENCE_CXX20_CISO646_REMOVED_WARNING
#endif

#include "PxPhysicsAPI.h"
#include "DUOLPhysics/Actor/PhysicsActorBase.h"

namespace DUOLPhysics
{
	using namespace physx;

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

	private:
		PxRigidActor* _actor;

	protected:
		/**
			@brief	 Actor Setter
			@details Child Class의 Actor가 생성될 때 같이 들고 있는다.
			@param   actor - Set할 actor
		**/
		void SetActor(PxRigidActor* actor);

	public:
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
	};
}