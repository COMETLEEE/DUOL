/**

	@file    PhysicsPlane.h
	@brief	 Physics Space의 무한한 크기의 평면
	@details -
	@author  JKim
	@date    25.10.2022

**/
#pragma once
/* etc */
#include "../Util/PhysicsDescriptions.h"

#include <memory>

namespace DUOLPhysics
{
	class PhysicsScene;

	/**

		@class   PhysicsPlane
		@brief	 Physics Space의 무한한 크기의 평면
		@details -

	**/
	class PhysicsPlane
	{
		class Impl;

		friend PhysicsScene;

	public:
		/**
			@brief   PhysicsPlane 클래스 생성자
			@details 생성자 호출시 Impl 생성
		**/
		PhysicsPlane();

		/**
			@brief   PhysicsPlane 클래스 default 소멸자
			@details -
		**/
		~PhysicsPlane() = default;

	private:
		std::shared_ptr<Impl> _impl;
	};
}