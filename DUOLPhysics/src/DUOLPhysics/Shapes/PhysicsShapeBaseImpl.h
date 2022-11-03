/**

	@file    PhysicsShapeBaseImpl.h
	@brief   Physics Shape Base의 Implementaion 클래스
	@details -
	@author  JKim
	@date    3.11.2022

**/
#pragma once
#ifndef _SILENCE_CXX20_CISO646_REMOVED_WARNING
#define _SILENCE_CXX20_CISO646_REMOVED_WARNING
#endif

#include "PxPhysicsAPI.h"
#include "DUOLPhysics/Shapes/PhysicsShapeBase.h"

namespace DUOLPhysics
{
	using namespace physx;

	/**

		@class   Impl
		@brief   Physics Shape Base의 Implementaion 클래스
		@details -

	**/
	class PhysicsShapeBase::Impl
	{
	public:
		/**
			@brief   Impl 클래스 생성자
			@details -
		**/
		Impl();

		/**
			@brief   Impl 클래스 소멸자
			@details -
		**/
		~Impl();

	private:
		PxShape* _shape;

	public:
		/**
			@brief	 Shape 생성
			@details -
			@param   physics   - Physics 객체를 통해 생성
			@param   shapeDesc - Shape 생성에 필요한 값
		**/
		void Create(PxPhysics* physics, const PhysicsShapeDesc& shapeDesc);

		/**
			@brief
			@details -
			@retval   -
		**/
		PxShape* GetShape();

		/**
			@brief	 Shape 할당 해제
			@details -
		**/
		void Release();
	};
}