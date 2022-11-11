/**

	@file    PhysicsDynamicActor.h
	@brief	 Physics Space의 동적 객체
	@details -
	@author  JKim
	@date    3.11.2022

**/
#pragma once
#include "PhysicsActorBase.h"

#include <memory>

namespace DUOLPhysics
{
	class PhysicsScene;
	class PhysicsShapeBase;

	/**

		@class   PhysicsDynamicActor
		@brief	 Physics Space의 동적 객체
		@details -

	**/
	class PhysicsDynamicActor : public PhysicsActorBase
	{
		class Impl;

		friend PhysicsScene;
		friend PhysicsShapeBase;

	public:
		/**
			@brief   PhysicsDynamicActor 클래스 생성자
			@details 생성자 호출시 Impl 생성, 부모에게 Impl 포인터 전달
		**/
		PhysicsDynamicActor();

		/**
			@brief   PhysicsDynamicActor 클래스 default 소멸자
			@details -
		**/
		~PhysicsDynamicActor() = default;

	private:
		std::shared_ptr<Impl> _impl;

	public:
		/**
			@brief	 Velocity만큼 힘을 가함
			@details -
			@param   velocity - 가할 힘의 값
		**/
		void SetLinearVelocity(DUOLMath::Vector3 velocity);

		/**
			@brief	 Actor 객체의 중심, 밀도 등을 변경
			@details -
			@param   density               - 밀도
			@param   massLocalPose         - 질량의 중심
			@param   isIncludedNonSimShape - Simulation flag가 아닌 도형도 포함할 것인가에 대한 값
		**/
		void SetMassAndInertia(float density, DUOLMath::Vector3 massLocalPose = DUOLMath::Vector3::Zero, bool isIncludedNonSimShape = false);

		/**
			@brief	 이동에 대한 저항력(선형 제동) 변경
			@details 음수는 0으로 처리됨.
			@param   linDamp - 선형 저항 값
		**/
		void SetLinearDamping(float linDamp);

		/**
			@brief	 회전에 대한 저항력(각형 제동) 변경
			@details 음수는 0으로 처리됨.
			@param   angDamp - 각형 저항 값
		**/
		void SetAngularDamping(float angDamp);
	};
}