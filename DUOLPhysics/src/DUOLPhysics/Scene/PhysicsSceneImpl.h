/**

	@file    PhysicsSceneImpl.h
	@brief	 Physics Scene의 Implementaion 클래스
	@details -
	@author  JKim
	@date    28.10.2022

**/
#pragma once
#ifndef _SILENCE_CXX20_CISO646_REMOVED_WARNING
#define _SILENCE_CXX20_CISO646_REMOVED_WARNING
#endif

#include "PxPhysicsAPI.h"
#include "DUOLPhysics/Scene/PhysicsScene.h"
#include "../System/PhysicsEventDispatcher.h"

namespace DUOLPhysics
{
	using namespace physx;

	/**

		@class   Impl
		@brief	 Physics Scene의 Implementaion 클래스
		@details -

	**/
	class PhysicsScene::Impl
	{
		friend PhysicsScene;

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
		PxScene* _scene;

		PxPhysics* _physics;

		PxCooking* _cooking;

		std::shared_ptr<PhysicsEventDispatcher> _eventDispatcher;

	public:
		/**
			@brief	 Scene 생성
			@details -
			@param   physics            - Physics 객체를 통해 생성
			@param   dispatcher         - 작업할 CPU Thread
			@param   cudaContextManager - CUDA 지원시 CUDA 사용
			@param   sceneDesc          - Scene 생성에 필요한 값
		**/
		void Create(PxPhysics* physics, PxCooking* cooking, PxCpuDispatcher* dispatcher, PxCudaContextManager* cudaContextManager, const PhysicsSceneDesc& sceneDesc);

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

		/**
			@brief	 Scene 할당 해제
			@details -
		**/
		void Release();
	};
}