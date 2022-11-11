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
#include "../Util/PhysicsDescriptions.h"
#include "DUOLPhysics/Util/PhysicsDataStructure.h"

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

	private:
		std::weak_ptr<Impl> _impl;

	protected:
		/**
			@brief	 Child Ŭ������ impl�� ������ �� ���� �ޱ� ���� �Լ�
			@details -
			@param   impl - Child Ŭ������ impl
		**/
		void SetImpl(const std::shared_ptr<Impl>& impl);

	public:
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
		GlobalPose GetGlobalPose() const;

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

		/**
			@brief	 ������ Actor ��ü�� Boungding Box Getter
			@details -
			@param   inflation - Boungding Box ũ�� ����
			@retval  3���� �������� �� ����(�ּ� ��, �ִ� ��)
		**/
		PhysicsBoundingBox GetBoundingBox(float inflation = 1.01f) const;

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
		// ActorFlag / DominanceGroup
	};
}