/**

	@file    PhysicsSystemImpl.h
	@brief   Physics System�� Implementaion Ŭ����
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
		@brief	 Physics System�� Implementaion Ŭ����
		@details -

	**/
	class PhysicsSystem::Impl
	{
		friend PhysicsSystem;

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
			@brief	 PhysX�� Core Ŭ���� �ʱ�ȭ
			@details PxDefaultAllocator, PxDefaultErrorCallback, PxDefaultCpuDispatcher
		**/
		void InitDefault();

		/**
			@brief	 PhysX�� Manager Ŭ���� �ʱ�ȭ
			@details PxFoundation, PxPhysics
		**/
		void InitPhysics();

		/**
			@brief	 PhysX Visual Debugger Ŭ���� �ʱ�ȭ
			@details PxPvd, PxPvdTransport, Socket("127.0.0.1", 5425) ���
		**/
		void InitPvd();

		/**
			@brief	 NVIDIA CUDA Ŭ���� �ʱ�ȭ
			@details RendererType �� CUDA ���� ���θ� Ȯ��
			@param   interop        - Renderer Type
			@param   graphicsDevice - D3D�� ��쿡 GraphicsDevice Pointer�� �ʿ���
		**/
		void InitCudaContextManager(PxCudaInteropMode::Enum interop, void* graphicsDevice);

		/**
			@brief	 PhysX �Ҵ� ����
			@details -
		**/
		void Release();
	};
}