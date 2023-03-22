#include "PhysicsSceneImpl.h"

/* etc */
#include "../Util/PhysicsTypeConverter.h"
#include "DUOLPhysics/Util/PhysicsDefines.h"

namespace DUOLPhysics
{
	using namespace physx;

	PxFilterFlags FilterShader(PxFilterObjectAttributes attributes0, PxFilterData filterData0, PxFilterObjectAttributes attributes1, PxFilterData filterData1, PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
	{
		// https://gameworksdocs.nvidia.com/PhysX/4.1/documentation/physxguide/Manual/RigidBodyCollision.html#broad-phase-callback

		// ������
		PX_UNUSED(constantBlock);
		PX_UNUSED(constantBlockSize);

		// �� �� �ϳ��� ��ü�� Ʈ�������� Ȯ���ؼ� ������ ���� ��� Ʈ���� �浹 ó��
		if (PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
		{
			pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
			return PxFilterFlag::eDEFAULT;
		}

		// ������ ���͵��� ���� ��� �浹 ����
		pairFlags = PxPairFlag::eCONTACT_DEFAULT;

		// ������
		PX_UNUSED(filterData0);
		PX_UNUSED(filterData1);

		// ��ü�� Filter�� �ʿ��� �� ���� �ڵ�, bitmask�� ���ϰ� shape �����ÿ�
		// _shape->setSimulationFilterData�� ȣ���ؼ� filter Setting�� ��
		{
			//// �� ��ü�� ���� �浹 Callback
			//if ((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
			//	pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_PERSISTS | PxPairFlag::eNOTIFY_TOUCH_LOST | PxPairFlag::eNOTIFY_CONTACT_POINTS;

			pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_PERSISTS | PxPairFlag::eNOTIFY_TOUCH_LOST | PxPairFlag::eNOTIFY_CONTACT_POINTS;
		}

		return PxFilterFlag::eDEFAULT;
	}

	PhysicsScene::Impl::Impl() :
		_scene(nullptr),
		_physics(nullptr),
		_cooking(nullptr)
	{

	}

	PhysicsScene::Impl::~Impl()
	{
		Release();
	}

	void PhysicsScene::Impl::Create(PxPhysics* physics, PxCooking* cooking, PxCpuDispatcher* dispatcher, PxCudaContextManager* cudaContextManager, const PhysicsSceneDesc& sceneDesc)
	{
		if (physics == nullptr)
			ERROR_THROW("Failed to create PxScene. (No PxPhysics.)");

		_physics = physics;
		_cooking = cooking;

		_eventDispatcher = std::make_shared<PhysicsEventDispatcher>();

		/* Scene */
		PxSceneDesc pxsceneDesc = physics->getTolerancesScale();
		pxsceneDesc.gravity = ConvertVector3(sceneDesc._gravity);
		pxsceneDesc.cpuDispatcher = dispatcher;
		pxsceneDesc.filterShader = FilterShader;
		pxsceneDesc.cudaContextManager = cudaContextManager;
		pxsceneDesc.simulationEventCallback = _eventDispatcher.get();

		// pxsceneDesc.flags |= PxSceneFlag::eENABLE_GPU_DYNAMICS;
		pxsceneDesc.flags |= PxSceneFlag::eENABLE_CCD;
		pxsceneDesc.flags |= PxSceneFlag::eENABLE_PCM;
		pxsceneDesc.flags |= PxSceneFlag::eENABLE_STABILIZATION;
		pxsceneDesc.broadPhaseType = PxBroadPhaseType::eSAP;
		pxsceneDesc.gpuMaxNumPartitions = 8;

		_scene = physics->createScene(pxsceneDesc);

		if (_scene == nullptr)
			ERROR_THROW("Failed to create PxScene.");

		_scene->userData = nullptr;

		/* Scene Client */
		PxPvdSceneClient* pvdClient = _scene->getScenePvdClient();

		if (pvdClient != nullptr)
		{
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
		}
	}

	const PxRenderBuffer& PhysicsScene::Impl::GetRenderBuffer() const
	{
		if (_scene == nullptr)
			ERROR_THROW("Failed to get Render Buffer.");

		return _scene->getRenderBuffer();
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