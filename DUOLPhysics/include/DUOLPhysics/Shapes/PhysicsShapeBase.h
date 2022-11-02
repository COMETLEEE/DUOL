/**

	@file    PhysicsShapeBase.h
	@brief	 Shape들의 부모 클래스
	@details Shape들을 묶어서 관리하고 다형적 생성을 위해 상속 구조 사용
	@author  JKim
	@date    31.10.2022

**/
#pragma once
#include "../PhysicsDescriptions.h"

namespace DUOLPhysics
{
	class PhysicsSystem;
	class PhysicsDynamicActor;

	/**

		@class   PhysicsShapeBase
		@brief	 Shape들의 부모 클래스
		@details -

	**/
	class PhysicsShapeBase
	{
		friend PhysicsSystem;

	public:
		/**
			@brief	 Shape 생성
			@details -
			@param   system    - System에 포함된 Physics를 쓰기 위해 전달
			@param   shapeDesc - Shape 생성에 필요한 값
		**/
		virtual void Create(PhysicsSystem* system, const PhysicsShapeDesc& shapeDesc) = 0;

		/**
			@brief
			@details -
			@param   actor -
		**/
		virtual void Attachment(PhysicsDynamicActor* actor) = 0;

		/**
			@brief	 Shape 할당 해제
			@details -
		**/
		virtual void Release() = 0;
	};
}