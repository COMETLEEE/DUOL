/**

	@file    PhysicsSystemImpl.h
	@brief   Physics System의 Core 클래스
	@details -
	@author  JKim
	@date    28.10.2022

**/
#pragma once
#include "DUOLPhysics/System/PhysicsSystem.h"
#include "PxPhysicsAPI.h"

#ifndef _SILENCE_CXX20_CISO646_REMOVED_WARNING
#define _SILENCE_CXX20_CISO646_REMOVED_WARNING
#endif

namespace DUOLPhysics
{
	using namespace physx;

	/**

		@class   Impl
		@brief	 Physics System의 Core 클래스
		@details -

	**/
	class PhysicsSystem::Impl
	{
		friend PhysicsSystem;

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
		PxDefaultAllocator* _allocator;

		PxDefaultErrorCallback* _errorCallback;

		PxFoundation* _foundation;

		PxPhysics* _physics;

		PxDefaultCpuDispatcher* _cpuDispatcher;

		PxPvd* _pvd;

		PxCudaContextManager* _cudaContextManager;

	private:
		/**
			@brief
			@details -
		**/
		void InitDefault();

		/**
			@brief
			@details -
		**/
		void InitPhysics();

		/**
			@brief
			@details -
		**/
		void InitPvd();

		/**
			@brief
			@details -
			@param   interop        -
			@param   graphicsDevice -
		**/
		void InitCudaContextManager(PxCudaInteropMode::Enum interop, void* graphicsDevice);

		/**
			@brief
			@details -
		**/
		void Release();
	};
}