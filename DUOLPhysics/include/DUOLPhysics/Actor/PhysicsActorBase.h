/**

	@file    PhysicsActorBase.h
	@brief	 Dynamic�� Static Actor Base
	@details -
	@author  JKim
	@date    31.10.2022

**/
#pragma once
/* Shape */
#include "../Shapes/PhysicsShapeBase.h"

/* etc */
#include "../PhysicsDescriptions.h"
#include "DUOLMath/DUOLMath.h"

#include <memory>

namespace DUOLPhysics
{
	/**

		@class   PhysicsActorBase
		@brief	 Dynamic�� Static Actor Base
		@details -

	**/
	class PhysicsActorBase
	{
	protected:
		class Impl;

	protected:
		/**
			@brief   PhysicsActorBase Ŭ���� ������
			@details -
		**/
		PhysicsActorBase();

		/**
			@brief   PhysicsActorBase Ŭ���� �Ҹ���
			@details -
		**/
		~PhysicsActorBase();

	private:
		std::weak_ptr<Impl> _impl;

	protected:
		void SetImpl(const std::shared_ptr<Impl>& impl);

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
		PhysicsBoundingBox GetBoundingBox(float inflation = 1.01f);

		/**
			@brief	 Actor�� ���� ����
			@details -
			@param   shape - ������ Shape
		**/
		void AttachShape(std::weak_ptr<PhysicsShapeBase> shape);

		/**
			@brief	 Actor���� ���� Ż��
			@details -
			@param   shape             - Ż���� Shape
			@param   isWakeOnLostTouch -
		**/
		void DetachShape(std::weak_ptr<PhysicsShapeBase> shape, bool isWakeOnLostTouch = true);

		// Maybe?
		// ActorFlag / DominanceGroup / userData
	};
}