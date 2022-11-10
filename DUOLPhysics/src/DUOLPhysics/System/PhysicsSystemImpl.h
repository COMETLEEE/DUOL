/**

	@file    PhysicsSystemImpl.h
	@brief   Physics System의 Implementaion 클래스
	@details -
	@author  JKim
	@date    28.10.2022

**/
#pragma once
#ifndef _SILENCE_CXX20_CISO646_REMOVED_WARNING
#define _SILENCE_CXX20_CISO646_REMOVED_WARNING
#endif

#include "PxPhysicsAPI.h"
#include "DUOLPhysics/System/PhysicsSystem.h"

namespace DUOLPhysics
{
	using namespace physx;

	/**

		@class   Impl
		@brief	 Physics System의 Implementaion 클래스
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

		PxCooking* _cooking;

		PxDefaultCpuDispatcher* _cpuDispatcher;

		PxPvd* _pvd;

		PxCudaContextManager* _cudaContextManager;

	public:
		/**
			@brief	 Physics Getter
			@details -
			@retval  _physics
		**/
		PxPhysics* GetPhysics() { return _physics; }

		/**
			@brief	 Cooking Getter
			@details -
			@retval  _cooking
		**/
		PxCooking* GetCooking() { return _cooking; }

	private:
		/**
			@brief	 PhysX의 Core 클래스 초기화
			@details PxDefaultAllocator, PxDefaultErrorCallback, PxDefaultCpuDispatcher
		**/
		void InitDefault();

		/**
			@brief	 PhysX의 Manager 클래스 초기화
			@details PxFoundation, PxPhysics
		**/
		void InitPhysics();

		/**
			@brief	 PhysX Visual Debugger 클래스 초기화
			@details PxPvd, PxPvdTransport, Socket("127.0.0.1", 5425) 사용
		**/
		void InitPvd();

		/**
			@brief	 NVIDIA CUDA 클래스 초기화
			@details RendererType 과 CUDA 지원 여부를 확인
			@param   interop        - Renderer Type
			@param   graphicsDevice - D3D인 경우에 GraphicsDevice Pointer가 필요함
		**/
		void InitCudaContextManager(PxCudaInteropMode::Enum interop, void* graphicsDevice);

		/**
			@brief	 PhysX 할당 해제
			@details -
		**/
		void Release();
	};
}