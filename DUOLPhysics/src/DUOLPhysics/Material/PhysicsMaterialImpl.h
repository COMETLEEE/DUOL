/**

	@file    PhysicsMaterialImpl.h
	@brief	 Physics Material�� Implementaion Ŭ����
	@details -
	@author  JKim
	@date    28.10.2022

**/
#pragma once
#ifndef _SILENCE_CXX20_CISO646_REMOVED_WARNING
#define _SILENCE_CXX20_CISO646_REMOVED_WARNING
#endif

#include "PxPhysicsAPI.h"
#include "DUOLPhysics/Material/PhysicsMaterial.h"

namespace DUOLPhysics
{
	using namespace physx;

	/**

		@class   Impl
		@brief	 Physics Material�� Implementaion Ŭ����
		@details -

	**/
	class PhysicsMaterial::Impl
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
		PxMaterial* _material;

	public:
		/**
			@brief	 Material ����
			@details -
			@param   physics      - Physics ��ü�� ���� ����
			@param   materialDesc - Material ������ �ʿ��� ��
		**/
		void Create(PxPhysics* physics, const PhysicsMaterialDesc& materialDesc);

		/**
			@brief	 Material Getter
			@details -
			@retval  ������ Material ��ü
		**/
		PxMaterial* GetMaterial();

		/**
			@brief   Material �Ҵ� ����
			@details -
		**/
		void Release();
	};
}