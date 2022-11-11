/**

	@file    PhysicsPlaneImpl.h
	@brief	 Physics Plane�� Implementaion Ŭ����
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
		@brief	 Physics Plane�� Implementaion Ŭ����
		@details -

	**/
	class PhysicsPlane::Impl
	{
	public:
		/**
			@brief   Impl Ŭ���� ������
			@details -
		**/
		Impl();

		/**
			@brief   Impl Ŭ���� default �Ҹ���
			@details -
		**/
		~Impl() = default;

	private:
		PxRigidStatic* _plane;

	public:
		/**
			@brief	 Plane ����
			@details -
			@param   physics   - Physics ��ü�� ���� ����
			@param   material  - Plane�� ����� Material
			@param   planeDesc - Plane ������ �ʿ��� ��
			@retval  ������ Plane ��ü
		**/
		PxRigidStatic* Create(PxPhysics* physics, PxMaterial* material, const PhysicsPlaneDesc& planeDesc);
	};
}