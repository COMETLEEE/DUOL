/**

	@file    PhysicsPlane.h
	@brief	 Physics Space�� ������ ũ���� ���
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
		@brief	 Physics Space�� ������ ũ���� ���
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
			@brief   PhysicsPlane Ŭ���� ������
			@details -
		**/
		PhysicsPlane();

		/**
			@brief   PhysicsPlane Ŭ���� default �Ҹ���
			@details -
		**/
		~PhysicsPlane() = default;

	private:
		PxRigidStatic* _plane;

	public:
		/**
			@brief	 Plane ����
			@details -
			@param   planeDesc - Plane ������ �ʿ��� ��
			@retval  ������ Plane�� Pointer
		**/
		PxRigidStatic* CreatePlane(HidedPhysicsPlaneDesc& planeDesc);

		/**
			@brief	 Plane �Ҵ� ����
			@details -
		**/
		void Release();
	};
}