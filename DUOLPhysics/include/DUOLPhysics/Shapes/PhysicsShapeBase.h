/**

	@file    PhysicsShapeBase.h
	@brief	 Shape들의 부모 클래스
	@details Shape들을 묶어서 관리하고 다형적 생성을 위해 상속 구조 사용
	@author  JKim
	@date    31.10.2022

**/
#pragma once
#include "../Util/PhysicsDescriptions.h"

#include <memory>

namespace DUOLPhysics
{
	class PhysicsSystem;
	class PhysicsScene;
	class PhysicsActorBase;

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
		friend PhysicsActorBase;

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
			@brief	 Shape 할당 해제
			@details -
		**/
		virtual void Release() final;
	};
}