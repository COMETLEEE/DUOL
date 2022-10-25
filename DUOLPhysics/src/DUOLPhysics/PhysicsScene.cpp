#include "DUOLPhysics/PhysicsScene.h"

#include <string>

#define ERROR_THROW(errStr)				\
{										\
	std::string errTemp = errStr;		\
	errTemp += " / File : ";			\
	errTemp += __FILE__;				\
	errTemp += ", Line : ";				\
	errTemp += std::to_string(__LINE__);\
	throw errTemp;						\
}

namespace DUOLPhysics
{
	using namespace physx;

	PhysicsScene::PhysicsScene() :
		_scene(nullptr)
	{

	}

	void PhysicsScene::CreateScene(HidedPhysicsSceneDesc& sceneDesc)
	{
		if (sceneDesc._physics == nullptr)
			ERROR_THROW("Failed to create PxScene. (No PxPhysics.)");

		/* Scene */
		PxSceneDesc pxsceneDesc = sceneDesc._physics->getTolerancesScale();
		pxsceneDesc.gravity = { sceneDesc._default._gravity.x, sceneDesc._default._gravity.y, sceneDesc._default._gravity.z };
		pxsceneDesc.cpuDispatcher = sceneDesc._cpuDispatcher;
		pxsceneDesc.filterShader = PxDefaultSimulationFilterShader;
		pxsceneDesc.cudaContextManager = sceneDesc._cudaContextManager;

		pxsceneDesc.flags |= PxSceneFlag::eENABLE_GPU_DYNAMICS;
		pxsceneDesc.flags |= PxSceneFlag::eENABLE_PCM;
		pxsceneDesc.flags |= PxSceneFlag::eENABLE_STABILIZATION;
		pxsceneDesc.broadPhaseType = PxBroadPhaseType::eGPU;
		pxsceneDesc.gpuMaxNumPartitions = 8;

		PxScene* ret = sceneDesc._physics->createScene(pxsceneDesc);

		if (ret == nullptr)
			ERROR_THROW("Failed to create PxScene.");

		/* Scene Client */
		PxPvdSceneClient* pvdClient = ret->getScenePvdClient();

		if (pvdClient != nullptr)
		{
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
		}
	}

	void PhysicsScene::Release()
	{
		if (_scene != nullptr)
		{
			_scene->release();
			_scene = nullptr;
		}
	}
}