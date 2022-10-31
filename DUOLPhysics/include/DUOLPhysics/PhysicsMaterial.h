/**

	@file    PhysicsMaterial.h
	@brief   Physics 연산에 사용되는 객체의 Material
	@details -
	@author  JKim
	@date    25.10.2022

**/
#pragma once
/* etc */
#include "PhysicsDescriptions.h"

#include <memory>

namespace DUOLPhysics
{
	class PhysicsSystem;
	class PhysicsScene;
	class PhysicsBox;

	/**

		@class   PhysicsMaterial
		@brief   Physics 연산에 사용되는 객체의 Material
		@details -

	**/
	class PhysicsMaterial
	{
		class Impl;

		friend PhysicsSystem;
		friend PhysicsScene;
		friend PhysicsBox;

	public:
		/**
			@brief   PhysicsMaterial 클래스 생성자
			@details -
		**/
		PhysicsMaterial();

		/**
			@brief   PhysicsMaterial 클래스 default 소멸자
			@details -
		**/
		~PhysicsMaterial() = default;

	private:
		std::shared_ptr<Impl> _impl;

	public:
		/**
			@brief   Material 할당 해제
			@details -
		**/
		void Release();
	};
}