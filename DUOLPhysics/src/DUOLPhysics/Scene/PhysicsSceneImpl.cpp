#include "PhysicsSceneImpl.h"

/* etc */
#include "../Util/PhysicsTypeConverter.h"

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

	PxFilterFlags FilterShader(PxFilterObjectAttributes attributes0, PxFilterData filterData0, PxFilterObjectAttributes attributes1, PxFilterData filterData1, PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
	{
		// https://gameworksdocs.nvidia.com/PhysX/4.1/documentation/physxguide/Manual/RigidBodyCollision.html#broad-phase-callback

		// 둘 중 하나의 객체가 트리거인지 확인해서 조건이 맞을 경우 트리거 충돌 처리
		if (PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
		{
			pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
			return PxFilterFlag::eDEFAULT;
		}

		// 위에서 필터되지 않은 모든 충돌 포함
		pairFlags = PxPairFlag::eCONTACT_DEFAULT;

		// 두 객체에 대한 충돌 Callback
		if ((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
			pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_PERSISTS | PxPairFlag::eNOTIFY_TOUCH_LOST | PxPairFlag::eNOTIFY_CONTACT_POINTS;

		return PxFilterFlag::eDEFAULT;
	}

	PhysicsScene::Impl::Impl() :
		_scene(nullptr),
		_physics(nullptr)
	{

	}

	PhysicsScene::Impl::~Impl()
	{
		Release();
	}

	void PhysicsScene::Impl::Create(PxPhysics* physics, PxCpuDispatcher* dispatcher, PxCudaContextManager* cudaContextManager, const PhysicsSceneDesc& sceneDesc)
	{
		if (physics == nullptr)
			ERROR_THROW("Failed to create PxScene. (No PxPhysics.)");

		_physics = physics;

		/* Scene */
		PxSceneDesc pxsceneDesc = physics->getTolerancesScale();
		pxsceneDesc.gravity = ConvertVector3(sceneDesc._gravity);
		pxsceneDesc.cpuDispatcher = dispatcher;
		pxsceneDesc.filterShader = FilterShader;
		pxsceneDesc.cudaContextManager = cudaContextManager;

		pxsceneDesc.flags |= PxSceneFlag::eENABLE_GPU_DYNAMICS;
		pxsceneDesc.flags |= PxSceneFlag::eENABLE_PCM;
		pxsceneDesc.flags |= PxSceneFlag::eENABLE_STABILIZATION;
		//pxsceneDesc.broadPhaseType = PxBroadPhaseType::eGPU;
		//pxsceneDesc.gpuMaxNumPartitions = 8;

		_scene = physics->createScene(pxsceneDesc);

		if (_scene == nullptr)
			ERROR_THROW("Failed to create PxScene.");

		/* Scene Client */
		PxPvdSceneClient* pvdClient = _scene->getScenePvdClient();

		if (pvdClient != nullptr)
		{
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
		}
	}

	void PhysicsScene::Impl::Release()
	{
		if (_scene != nullptr)
		{
			_scene->release();
			_scene = nullptr;
		}
	}
}