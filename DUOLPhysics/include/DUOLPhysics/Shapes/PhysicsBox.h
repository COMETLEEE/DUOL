/**

	@file    PhysicsBox.h
	@brief	 Physics Box 도형
	@details -
	@author  JKim
	@date    31.10.2022

**/
#pragma once
#include "PhysicsShapeBase.h"

#include <memory>

namespace DUOLPhysics
{
	class PhysicsSystem;
	class PhysicsScene;

	/**

		@class   PhysicsBox
		@brief	 Physics Box 도형
		@details -

	**/
	class PhysicsBox : public PhysicsShapeBase
	{
		class Impl;

		friend PhysicsSystem;
		friend PhysicsScene;

	private:
		/**
			@brief   PhysicsBox 클래스 생성자
			@details -
		**/
		PhysicsBox();

	public:
		/**
			@brief   PhysicsBox 클래스 소멸자
			@details -
		**/
		~PhysicsBox();

	public:
		/**
			@brief	 Physics Box 생성
			@details -
			@param   system    - Box 생성에 필요한 Physics를 건내받을 클래스
			@param   shapeDesc - Box 생성에 필요한 값
		**/
		void Create(PhysicsSystem* system, const PhysicsShapeDesc& shapeDesc) override;

		/**
			@brief	 Physics Box 생성
			@details -
			@param   scene     - Box 생성에 필요한 Physics를 건내받을 클래스
			@param   shapeDesc - Box 생성에 필요한 값
		**/
		void Create(PhysicsScene* scene, const PhysicsShapeDesc& shapeDesc) override;
	};
}