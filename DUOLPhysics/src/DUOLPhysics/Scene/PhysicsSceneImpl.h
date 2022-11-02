/**

	@file    PhysicsSceneImpl.h
	@brief	 Physics Scene�� Implementaion Ŭ����
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

namespace DUOLPhysics
{
	using namespace physx;

	/**

		@class   Impl
		@brief	 Physics Scene�� Implementaion Ŭ����
		@details -

	**/
	class PhysicsScene::Impl
	{
		friend PhysicsScene;

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
		PxScene* _scene;

		PxPhysics* _physics;

	public:
		/**
			@brief	 Scene ����
			@details -
			@param   physics            - Physics ��ü�� ���� ����
			@param   dispatcher         - �۾��� CPU Thread
			@param   cudaContextManager - CUDA ������ CUDA ���
			@param   sceneDesc          - Scene ������ �ʿ��� ��
		**/
		void Create(PxPhysics* physics, PxCpuDispatcher* dispatcher, PxCudaContextManager* cudaContextManager, const PhysicsSceneDesc& sceneDesc);

		/**
			@brief	 Scene �Ҵ� ����
			@details -
		**/
		void Release();
	};
}