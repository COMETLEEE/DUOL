/**

	@file    PhysicsBoxImpl.h
	@brief	 Physics Box�� Implementaion Ŭ����
	@details -
	@author  JKim
	@date    31.10.2022

**/
#pragma once
#ifndef _SILENCE_CXX20_CISO646_REMOVED_WARNING
#define _SILENCE_CXX20_CISO646_REMOVED_WARNING
#endif

#include "DUOLPhysics/Shapes/PhysicsBox.h"
#include "PxPhysicsAPI.h"

namespace DUOLPhysics
{
	using namespace physx;

	/**

		@class   Impl
		@brief	 Physics Box�� Implementaion Ŭ����
		@details -

	**/
	class PhysicsBox::Impl
	{
	public:
		/**
			@brief   Impl Ŭ���� ������
			@details -
		**/
		Impl();

		/**
			@brief   Impl Ŭ���� �Ҹ���
			@details -
		**/
		~Impl();

	private:
		PxShape* _shape;

	public:
		/**
			@brief	 Shape ����
			@details -
			@param   physics   - Physics ��ü�� ���� ����
			@param   shapeDesc - Shape ������ �ʿ��� ��
		**/
		void Create(PxPhysics* physics, const PhysicsShapeDesc& shapeDesc);

		/**
			@brief	 Shape �Ҵ� ����
			@details -
		**/
		void Release();
	};
}