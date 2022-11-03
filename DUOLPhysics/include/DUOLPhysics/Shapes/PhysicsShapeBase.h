/**

	@file    PhysicsShapeBase.h
	@brief	 Shape들의 부모 클래스
	@details Shape들을 묶어서 관리하고 다형적 생성을 위해 상속 구조 사용
	@author  JKim
	@date    31.10.2022

**/
#pragma once
#include "../PhysicsDescriptions.h"

#include <memory>

namespace DUOLPhysics
{
	class PhysicsSystem;
	class PhysicsScene;
	class PhysicsDynamicActor;

	/**

		@class   PhysicsShapeBase
		@brief	 Shape들의 부모 클래스
		@details -

	**/
	class PhysicsShapeBase
	{
	protected:
		class Impl;

	private:
		friend PhysicsSystem;
		friend PhysicsDynamicActor;

	protected:
		/**
			@brief   PhysicsShapeBase 클래스 생성자
			@details 생성자 호출시 Impl 생성
		**/
		PhysicsShapeBase();

		/**
			@brief   PhysicsShapeBase 클래스 소멸자
			@details -
		**/
		~PhysicsShapeBase();

	protected:
		std::shared_ptr<Impl> _impl;

	public:
		/**
			@brief	 Shape 생성
			@details -
			@param   system    - System에 포함된 Physics를 쓰기 위해 전달
			@param   shapeDesc - Shape 생성에 필요한 값
		**/
		virtual void Create(PhysicsSystem* system, const PhysicsShapeDesc& shapeDesc) = 0;

		/**
			@brief	 Shape 생성
			@details Scene에서 생성하고 바로 Actor에 부착 후 해제됨
			@param   scene    - Scene에 포함된 Physics를 쓰기 위해 전달
			@param   shapeDesc - Shape 생성에 필요한 값
		**/
		virtual void Create(PhysicsScene* scene, const PhysicsShapeDesc& shapeDesc) = 0;

		/**
			@brief	 Actor에 부착
			@details -
			@param   actor - 부착할 Actor
		**/
		virtual void Attachment(PhysicsDynamicActor* actor) final;

		/**
			@brief	 Actor에서 탈착
			@details -
			@param   actor             - 탈착할 Actor
			@param   isWakeOnLostTouch -
		**/
		virtual void Detachment(PhysicsDynamicActor* actor, bool isWakeOnLostTouch) final;

		/**
			@brief	 Shape 할당 해제
			@details -
		**/
		virtual void Release() final;
	};
}