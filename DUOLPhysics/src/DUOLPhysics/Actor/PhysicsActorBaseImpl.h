
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
		PxRigidActor* _actor;

	protected:
		/**
			@brief	 Actor Setter
			@details Child Class�� Actor�� ������ �� ���� ��� �ִ´�.
			@param   actor - Set�� actor
		**/
		void SetActor(PxRigidActor* actor);

	public:
		/**
			@brief	 Actor�� Global Pose Getter
			@details -
			@retval  Global Pose
		**/
		GlobalPose GetGlobalPose();

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
		void SetGlobalPose(const GlobalPose& globalPose);

		/**
			@brief	 ������ Actor ��ü�� Boungding Box Getter
			@details -
			@param   inflation - Boungding Box ũ�� ����
			@retval  3���� �������� �� ����(�ּ� ��, �ִ� ��)
		**/
		PhysicsBoundingBox GetBoundingBox(float inflation);

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
	};
}