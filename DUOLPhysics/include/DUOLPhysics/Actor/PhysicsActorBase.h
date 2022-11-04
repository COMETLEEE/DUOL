/**

	@file    PhysicsActorBase.h
	@brief	 Dynamic과 Static Actor Base
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
		@brief	 Dynamic과 Static Actor Base
		@details -

	**/
	class PhysicsActorBase
	{
	protected:
		class Impl;

	protected:
		/**
			@brief   PhysicsActorBase 클래스 생성자
			@details -
		**/
		PhysicsActorBase();

		/**
			@brief   PhysicsActorBase 클래스 소멸자
			@details -
		**/
		~PhysicsActorBase();

	private:
		std::weak_ptr<Impl> _impl;

	protected:
		void SetImpl(const std::shared_ptr<Impl>& impl);

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
		PhysicsBoundingBox GetBoundingBox(float inflation = 1.01f);

		/**
			@brief	 Actor에 도형 부착
			@details -
			@param   shape - 부착할 Shape
		**/
		void AttachShape(std::weak_ptr<PhysicsShapeBase> shape);

		/**
			@brief	 Actor에서 도형 탈착
			@details -
			@param   shape             - 탈착할 Shape
			@param   isWakeOnLostTouch -
		**/
		void DetachShape(std::weak_ptr<PhysicsShapeBase> shape, bool isWakeOnLostTouch = true);

		// Maybe?
		// ActorFlag / DominanceGroup / userData
	};
}