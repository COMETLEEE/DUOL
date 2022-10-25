/**

	@file    PhysicsScene.h
	@brief	 Physics �����ϴ� Scene ����
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
		@brief	 Physics �����ϴ� Scene ����
		@details -

	**/
	class PhysicsScene
	{
	public:
		/**
			@brief   PhysicsScene Ŭ���� ������
			@details -
		**/
		PhysicsScene();

		/**
			@brief   PhysicsScene Ŭ���� default �Ҹ���
			@details -
		**/
		~PhysicsScene() = default;

	private:
		PxScene* _scene;

	public:
		/**
			@brief	 Scene ����
			@details -
			@param   sceneDesc - Scene ������ �ʿ��� ��
		**/
		void CreateScene(HidedPhysicsSceneDesc& sceneDesc);

		/**
			@brief	 Scene �Ҵ� ����
			@details -
		**/
		void Release();
	};
}