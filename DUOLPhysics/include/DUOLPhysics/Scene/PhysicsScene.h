/**

	@file    PhysicsScene.h
	@brief	 Physics 연산하는 Scene 공간
	@details -
	@author  JKim
	@date    24.10.2022

**/
#pragma once
#include "../PhysicsDescriptions.h"

#include <memory>

namespace DUOLPhysics
{
	class PhysicsSystem;

	/**

		@class   PhysicsScene
		@brief	 Physics 연산하는 Scene 공간
		@details -

	**/
	class PhysicsScene
	{
		class Impl;

		friend PhysicsSystem;

	public:
		/**
			@brief   PhysicsScene 클래스 생성자
			@details -
		**/
		PhysicsScene();

		/**
			@brief   PhysicsScene 클래스 default 소멸자
			@details -
		**/
		~PhysicsScene() = default;

	private:
		std::shared_ptr<Impl> _impl;

	public:
		/**
			@brief	 Scene 할당 해제
			@details -
		**/
		void Release();
	};
}