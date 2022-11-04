/**

	@file    PhysicsStaticActor.h
	@brief	 Physics Space의 정적 객체
	@details -
	@author  JKim
	@date    4.11.2022

**/
#pragma once
#include "PhysicsActorBase.h"

#include <memory>

namespace DUOLPhysics
{
	class PhysicsScene;
	class PhysicsShapeBase;

	/**

		@class   PhysicsStaticActor
		@brief	 Physics Space의 정적 객체
		@details -

	**/
	class PhysicsStaticActor : public PhysicsActorBase
	{
		class Impl;

		friend PhysicsScene;
		friend PhysicsShapeBase;

	public:
		/**
			@brief   PhysicsStaticActor 클래스 생성자
			@details 생성자 호출시 Impl 생성, 부모에게 Impl 포인터 전달
		**/
		PhysicsStaticActor();

		/**
			@brief   PhysicsStaticActor 클래스 소멸자
			@details -
		**/
		~PhysicsStaticActor();

	private:
		std::shared_ptr<Impl> _impl;
	};
}