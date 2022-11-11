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
			@brief	 객체에 선형으로 힘을 가함
			@details -
			@param   force - force 값
		**/
		void AddForce(const DUOLMath::Vector3& force);

		/**
			@brief	 Force 초기화
			@details -
		**/
		void ResetForce();

		/**
			@brief	 객체에 각형으로 힘을 가함
			@details -
			@param   torque - torque 값
		**/
		void AddTorque(const DUOLMath::Vector3& torque);

		/**
			@brief	 Torque 초기화
			@details -
		**/
		void ResetTorque();

		/**
			@brief	 Axis Lock 상태 확인
			@details -
			@param   targetAxis - 상태 확인할 Axis
			@retval  Lock되었으면 true, 아니면 false
		**/
		bool GetAxisLockState(AxisLock targetAxis) const;

		/**
			@brief	 Axis 잠금 / 해제
			@details -
			@param   targetAxis - 대상 Axis
			@param   isLock     - true면 잠금, false면 해제
		**/
		void SetAxisLock(AxisLock targetAxis, bool isLock);

		/**
			@brief	 Linear Velocity Getter
			@details -
			@retval  Linear Velocity
		**/
		DUOLMath::Vector3 GetLinearVelocity() const;

		/**
			@brief	 Linear Velocity Setter
			@details -
			@param   Linear velocity - 속도 값
		**/
		void SetLinearVelocity(const DUOLMath::Vector3& velocity);
		
		/**
			@brief	 Angular Velocity Getter
			@details -
			@retval  Angular Velocity
		**/
		DUOLMath::Vector3 GetAngularVelocity() const;

		/**
			@brief	 Angular Velocity Setter
			@details -
			@param   Angular velocity - 속도 값
		**/
		void SetAngularVelocity(const DUOLMath::Vector3& velocity);

		/**
			@brief	 Mass Getter
			@details -
			@retval  Mass
		**/
		float GetMass() const;

		/**
			@brief	 Inverse Mass Getter
			@details -
			@retval  Inverse Mass
		**/
		float GetInverseMass() const;

		/**
			@brief	 Mass Setter
			@details -
			@param   mass - 질량 값
		**/
		void SetMass(float mass);

		/**
			@brief	 Actor 객체의 중심, 밀도 등을 변경
			@details -
			@param   density               - 밀도
			@param   massLocalPose         - 질량의 중심
			@param   isIncludedNonSimShape - Simulation flag가 아닌 도형도 포함할 것인가에 대한 값
		**/
		void SetMassAndInertia(float density, const DUOLMath::Vector3& massLocalPose = DUOLMath::Vector3::Zero, bool isIncludedNonSimShape = false);

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