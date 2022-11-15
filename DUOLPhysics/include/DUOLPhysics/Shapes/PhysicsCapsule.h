/**

	@file    PhysicsCapsule.h
	@brief	 Physics Capsule 도형
	@details -
	@author  JKim
	@date    10.11.2022

**/
#pragma once
#include "PhysicsShapeBase.h"

namespace DUOLPhysics
{
	/**

		@class   PhysicsCapsule
		@brief	 Physics Capsule 도형
		@details -

	**/
	class PhysicsCapsule : public PhysicsShapeBase
	{
	public:
		/**
			@brief   PhysicsCapsule 클래스 생성자
			@details -
		**/
		PhysicsCapsule();

		/**
			@brief   PhysicsCapsule 클래스 default 소멸자
			@details -
		**/
		~PhysicsCapsule() = default;

	public:
		/**
			@brief	 Physics Capsule 생성
			@details -
			@param   system    - Capsule 생성에 필요한 Physics를 건내받을 클래스
			@param   shapeDesc - Capsule 생성에 필요한 값
		**/
		void Create(PhysicsSystem* system, const PhysicsShapeDesc& shapeDesc) override;

		/**
			@brief	 Physics Capsule 생성
			@details -
			@param   scene     - Capsule 생성에 필요한 Physics를 건내받을 클래스
			@param   shapeDesc - Capsule 생성에 필요한 값
		**/
		void Create(PhysicsScene* scene, const PhysicsShapeDesc& shapeDesc) override;

		void SetScale(float radius, float halfHeight);
	};
}