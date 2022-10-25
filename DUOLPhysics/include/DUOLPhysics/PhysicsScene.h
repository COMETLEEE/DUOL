/**

	@file    PhysicsScene.h
	@brief	 Physics 연산하는 Scene 공간
	@details -
	@author  JKim
	@date    24.10.2022

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

	struct HidedPhysicsSceneDesc
	{
		PhysicsSceneDesc _default;

		PxPhysics* _physics;

		PxCpuDispatcher* _cpuDispatcher;

		PxCudaContextManager* _cudaContextManager;
	};

	/**

		@class   PhysicsScene
		@brief	 Physics 연산하는 Scene 공간
		@details -

	**/
	class PhysicsScene
	{
	public:
		/**
			@brief   PhysicsScene 클래스 생성자
			@details -
		**/
		PhysicsScene();

		/**
			@brief   PhysicsScene 클래스 default 소멸자
			@details -
		**/
		~PhysicsScene() = default;

	private:
		PxScene* _scene;

	public:
		/**
			@brief	 Scene 생성
			@details -
			@param   sceneDesc - Scene 생성에 필요한 값
		**/
		void CreateScene(HidedPhysicsSceneDesc& sceneDesc);

		/**
			@brief	 Scene 할당 해제
			@details -
		**/
		void Release();
	};
}