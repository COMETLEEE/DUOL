/**

	@file    PhysicsMaterialImpl.h
	@brief	 Physics Material의 Implementaion 클래스
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
		@brief	 Physics Material의 Implementaion 클래스
		@details -

	**/
	class PhysicsMaterial::Impl
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
		PxMaterial* _material;

	public:
		/**
			@brief	 Material 생성
			@details -
			@param   physics      - Physics 객체를 통해 생성
			@param   materialDesc - Material 생성에 필요한 값
		**/
		void Create(PxPhysics* physics, const PhysicsMaterialDesc& materialDesc);

		/**
			@brief	 Material Getter
			@details -
			@retval  생성된 Material 객체
		**/
		PxMaterial* GetMaterial();

		/**
			@brief   Material 할당 해제
			@details -
		**/
		void Release();
	};
}