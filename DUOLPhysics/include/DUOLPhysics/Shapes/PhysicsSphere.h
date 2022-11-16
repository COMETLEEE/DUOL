/**

	@file    PhysicsSphere.h
	@brief	 Physics Sphere 도형
	@details -
	@author  JKim
	@date    10.11.2022

**/
#pragma once
#include "PhysicsShapeBase.h"

namespace DUOLPhysics
{
	/**

		@class   PhysicsSphere
		@brief	 Physics Sphere 도형
		@details -

	**/
	class PhysicsSphere : public PhysicsShapeBase
	{
	public:
		/**
			@brief   PhysicsSphere 클래스 생성자
			@details -
		**/
		PhysicsSphere();

		/**
			@brief   PhysicsSphere 클래스 default 소멸자
			@details -
		**/
		~PhysicsSphere() = default;

	public:
		/**
			@brief	 Physics Sphere 생성
			@details -
			@param   system    - Sphere 생성에 필요한 Physics를 건내받을 클래스
			@param   shapeDesc - Sphere 생성에 필요한 값
		**/
		void Create(PhysicsSystem* system, const PhysicsShapeDesc& shapeDesc) override;

		/**
			@brief	 Physics Sphere 생성
			@details -
			@param   scene     - Sphere 생성에 필요한 Physics를 건내받을 클래스
			@param   shapeDesc - Sphere 생성에 필요한 값
		**/
		void Create(PhysicsScene* scene, const PhysicsShapeDesc& shapeDesc) override;

		/**
			@brief	 Sphere의 크기 변경
			@details Exclusive한 도형이거나 Attach되지 않은 때만 사용 가능
			@param   radius - 반지름 값
		**/
		void SetScale(float radius);
	};
}