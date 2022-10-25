/**

	@file    PhysicsPlane.h
	@brief	 Physics Space의 무한한 크기의 평면
	@details -
	@author  JKim
	@date    25.10.2022

**/
#pragma once
#ifndef _SILENCE_CXX20_CISO646_REMOVED_WARNING
#define _SILENCE_CXX20_CISO646_REMOVED_WARNING
#endif

#include "PxPhysicsAPI.h"
#include "PhysicsDescriptions.h"

namespace DUOLPhysics
{
	using namespace physx;

	/**
		@struct  HidedPhysicsPlaneDesc
		@brief	 Physics Space의 무한한 크기의 평면
		@details -
	**/
	struct HidedPhysicsPlaneDesc
	{
		PhysicsPlaneDesc _default;

		PxPhysics* _physics;

		PxMaterial* _material;
	};

	class PhysicsPlane
	{
	public:
		/**
			@brief   PhysicsPlane 클래스 생성자
			@details -
		**/
		PhysicsPlane();

		/**
			@brief   PhysicsPlane 클래스 default 소멸자
			@details -
		**/
		~PhysicsPlane() = default;

	private:
		PxRigidStatic* _plane;

	public:
		/**
			@brief	 Plane 생성
			@details -
			@param   planeDesc - Plane 생성에 필요한 값
			@retval  생성된 Plane의 Pointer
		**/
		PxRigidStatic* CreatePlane(HidedPhysicsPlaneDesc& planeDesc);

		/**
			@brief	 Plane 할당 해제
			@details -
		**/
		void Release();
	};
}