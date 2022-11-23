/**

	@file    PhysicsDynamicActorImpl.h
	@brief	 Physics Dynamic Actor의 Implementation 클래스
	@details -
	@author  JKim
	@date    3.11.2022

**/
#pragma once
#ifndef _SILENCE_CXX20_CISO646_REMOVED_WARNING
#define _SILENCE_CXX20_CISO646_REMOVED_WARNING
#endif

#include "PxPhysicsAPI.h"
#include "PhysicsActorBaseImpl.h"
#include "DUOLPhysics/Actor/PhysicsDynamicActor.h"

namespace DUOLPhysics
{
	using namespace physx;

	/**

		@class   Impl
		@brief	 Physics Dynamic Actor의 Implementation 클래스
		@details -

	**/
	class PhysicsDynamicActor::Impl : public PhysicsActorBase::Impl
	{
	public:
		/**
			@brief   Impl 클래스 생성자
			@details -
		**/
		Impl();

		/**
			@brief   Impl 클래스 default 소멸자
			@details -
		**/
		~Impl() = default;

	private:
		PxRigidDynamic* _actor;

	public:
		/**
			@brief	 비어있는 Dynamic Actor 생성
			@details -
			@param   physics     - Physics 객체를 통해 생성
			@param   dynamicDesc - Dynamic Actor 생성에 필요한 값
			@retval  생성된 DynamicActor 객체
		**/
		PxRigidDynamic* Create(PxPhysics* physics, const PhysicsActorDesc& dynamicDesc);

		/**
			@brief	 상속된 Actor 반환 함수
			@details -
			@retval  PxRigidActor*
		**/
		PxRigidActor* GetActor() const override;

		/**
			@brief	 Actor 객체 Getter
			@details -
			@retval  _actor
		**/
		PxRigidDynamic* GetDynamicActor() const;

		/**
			@brief	 중력 적용 상태 체크
			@details -
			@retval  적용 중이면 true, 아니면 false
		**/
		bool GetGravityEnable() const;

		/**
			@brief	 Mass의 중심 Getter
			@details -
			@retval  Mass의 중심 Local 좌표
		**/
		const DUOLMath::Vector3& GetCenterOfMass() const;

		/**
			@brief	 Mass의 중심 Setter
			@details -
			@param   localPose - Mass의 중심 Local 좌표
		**/
		void SetCenterOfMass(const DUOLMath::Vector3& localPose);

		/**
			@brief	 중력 적용 / 해제
			@details -
			@param   useGravity - true면 적용, false면 해제
		**/
		void SetGravityEnable(bool useGravity);

		/**
			@brief	 객체에 선형으로 힘을 가함
			@details -
			@param   force - force 값
		**/
		void AddForce(const DUOLMath::Vector3& force);

		/**
			@brief	 객체에 선형으로 힘을 가함
			@details -
			@param   impulse - impulse 값
		**/
		void AddImpulse(const DUOLMath::Vector3& impulse);

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
			@param   velocity - 속도 값
		**/
		void SetLinearVelocity(const DUOLMath::Vector3& velocity);

		/**
			@brief`	 Angular Velocity Getter
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
		void SetMassAndInertia(float density, const DUOLMath::Vector3& massLocalPose, bool isIncludedNonSimShape);

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