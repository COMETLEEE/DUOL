/**

	@file    PhysicsPlaneImpl.h
	@brief	 Physics Plane의 Implementaion 클래스
	@details -
	@author  JKim
	@date    28.10.2022

**/
#pragma once
#ifndef _SILENCE_CXX20_CISO646_REMOVED_WARNING
#define _SILENCE_CXX20_CISO646_REMOVED_WARNING
#endif

#include "PxPhysicsAPI.h"
#include "DUOLPhysics/Shapes/PhysicsPlane.h"

namespace DUOLPhysics
{
	using namespace physx;

	/**

		@class   Impl
		@brief	 Physics Plane의 Implementaion 클래스
		@details -

	**/
	class PhysicsPlane::Impl
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
		PxRigidStatic* _plane;

	public:
		/**
			@brief	 Plane 생성
			@details -
			@param   physics   - Physics 객체를 통해 생성
			@param   material  - Plane에 적용될 Material
			@param   planeDesc - Plane 생성에 필요한 값
			@retval  생성된 Plane 객체
		**/
		PxRigidStatic* Create(PxPhysics* physics, PxMaterial* material, const PhysicsPlaneDesc& planeDesc);
	};
}