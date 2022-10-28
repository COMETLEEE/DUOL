#pragma once
#include "DUOLPhysics/Scene/PhysicsScene.h"
#include "PxPhysicsAPI.h"

#ifndef _SILENCE_CXX20_CISO646_REMOVED_WARNING
#define _SILENCE_CXX20_CISO646_REMOVED_WARNING
#endif

namespace DUOLPhysics
{
	using namespace physx;

	class PhysicsScene::Impl
	{
		friend PhysicsScene;
	public:
		Impl();

		~Impl();

	private:
		PxScene* _scene;

		PxPhysics* _physics;

	public:
		void Create(PxPhysics* physics, PxCpuDispatcher* dispatcher, PxCudaContextManager* cudaContextManager, const PhysicsSceneDesc& sceneDesc);
	
		void Release();
	};
}