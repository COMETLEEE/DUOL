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

#include "DUOLPhysics/System/PhysicsSystem.h"
#include "PxPhysicsAPI.h"

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
		
		friend PhysicsBox;

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

		PxDefaultCpuDispatcher* _cpuDispatcher;

		PxPvd* _pvd;

		PxCudaContextManager* _cudaContextManager;

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