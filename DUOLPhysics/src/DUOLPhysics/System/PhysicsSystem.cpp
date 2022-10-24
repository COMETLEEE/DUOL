#include "DUOLPhysics/System/PhysicsSystem.h"
#include "PxPhysicsAPI.h"
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

#pragma region Physics Implementation Parts
	class PhysicsImplementation
	{
		friend PhysicsSystem;

	private:
		PhysicsImplementation();

		~PhysicsImplementation();

	private:
		PxDefaultAllocator* _allocator;

		PxDefaultErrorCallback* _errorCallback;

		PxFoundation* _foundation;

		PxPhysics* _physics;

		PxDefaultCpuDispatcher* _cpuDispatcher;

		PxScene* _scene;

		PxMaterial* _material;

		PxPvd* _pvd;

		PxCudaContextManager* _cudaContextManager;

	private:
		void InitDefault();

		void InitPhysics(float staticFriction, float dynamicFriction, float restitution, float gravity);

		void InitPvd();

		void InitCudaContextManager(PxCudaInteropMode::Enum interop, void* graphicsDevice);

		void Release();
	};

	PhysicsImplementation::PhysicsImplementation() :
		_allocator(nullptr)
		, _errorCallback(nullptr)
		, _foundation(nullptr)
		, _physics(nullptr)
		, _cpuDispatcher(nullptr)
		, _scene(nullptr)
		, _material(nullptr)
		, _pvd(nullptr)
		, _cudaContextManager(nullptr)
	{

	}

	PhysicsImplementation::~PhysicsImplementation()
	{
		Release();
	}

	void PhysicsImplementation::InitDefault()
	{
		/* Allocator */
		if ((_allocator = new PxDefaultAllocator) == nullptr)
			ERROR_THROW("Failed to create PxDefaultAllocator");

		/* Error Callback */
		if ((_errorCallback = new PxDefaultErrorCallback) == nullptr)
			ERROR_THROW("Failed to create PxDefaultErrorCallback");

		/* CPU Dispatcher */
		_cpuDispatcher = PxDefaultCpuDispatcherCreate(2);

		if (_cpuDispatcher == nullptr)
			ERROR_THROW("Failed to create PxDefaultCpuDispatcher");
	}

	void PhysicsImplementation::InitPhysics(float staticFriction, float dynamicFriction, float restitution, float gravity)
	{
		/* Foundation */
		_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, *_allocator, *_errorCallback);

		if (_foundation == nullptr)
			ERROR_THROW("Failed to create PxFoundation");

		/* Physics */
		_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *_foundation, PxTolerancesScale(), true, _pvd);

		if (_physics == nullptr)
			ERROR_THROW("Failed to create PxPhysics");

		/* Scene */
		PxSceneDesc sceneDesc = _physics->getTolerancesScale();
		sceneDesc.gravity = PxVec3(0.0f, gravity, 0.0f);
		sceneDesc.cpuDispatcher = _cpuDispatcher;
		sceneDesc.filterShader = PxDefaultSimulationFilterShader;
		sceneDesc.cudaContextManager = _cudaContextManager;

		sceneDesc.flags |= PxSceneFlag::eENABLE_GPU_DYNAMICS;
		sceneDesc.flags |= PxSceneFlag::eENABLE_PCM;
		sceneDesc.flags |= PxSceneFlag::eENABLE_STABILIZATION;
		sceneDesc.broadPhaseType = PxBroadPhaseType::eGPU;
		sceneDesc.gpuMaxNumPartitions = 8;

		_scene = _physics->createScene(sceneDesc);

		if (_scene == nullptr)
			ERROR_THROW("Failed to create PxScene");

		/* Scene Client */
		PxPvdSceneClient* pvdClient = _scene->getScenePvdClient();

		if (pvdClient != nullptr)
		{
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
		}

		/* Material */
		_material = _physics->createMaterial(0.5f, 0.5f, 0.6f);
	}

	void PhysicsImplementation::InitPvd()
	{
		/* PhysX Visual Debugger */
		_pvd = PxCreatePvd(*_foundation);

		if (_pvd == nullptr)
			ERROR_THROW("Failed to create PxPvd");

		/* Transport */
		PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);

		if (transport == nullptr)
			ERROR_THROW("Failed to create PxPvdTransport");

		_pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);
	}

	void PhysicsImplementation::InitCudaContextManager(PxCudaInteropMode::Enum interop, void* graphicsDevice)
	{
		/* Cuda Context Manager */
		PxCudaContextManagerDesc ccmDesc;

		ccmDesc.interopMode = interop;
		ccmDesc.graphicsDevice = graphicsDevice;

		_cudaContextManager = PxCreateCudaContextManager(*_foundation, ccmDesc, PxGetProfilerCallback());

		if (_cudaContextManager == nullptr)
			ERROR_THROW("Failed to create PxCudaContextManager");

		if (_cudaContextManager->contextIsValid() != true)
		{
			_cudaContextManager->release();
			_cudaContextManager = nullptr;

			ERROR_THROW("PxCudaContextManager has not a valid CUDA context");
		}
	}

	void PhysicsImplementation::Release()
	{
		delete _allocator;
		_allocator = nullptr;

		delete _errorCallback;
		_errorCallback = nullptr;

		if (_scene != nullptr)
		{
			_scene->release();
			_scene = nullptr;
		}

		if (_cpuDispatcher != nullptr)
		{
			_cpuDispatcher->release();
			_cpuDispatcher = nullptr;
		}

		if (_physics != nullptr)
		{
			_physics->release();
			_physics = nullptr;
		}

		if (_pvd != nullptr)
		{
			PxPvdTransport* transport = _pvd->getTransport();
			
			_pvd->release();
			_pvd = nullptr;

			transport->release();
			transport = nullptr;
		}

		if (_cudaContextManager != nullptr)
		{
			_cudaContextManager->release();
			_cudaContextManager = nullptr;
		}

		if (_foundation != nullptr)
		{
			_foundation->release();
			_foundation = nullptr;
		}
	}
#pragma endregion

	PhysicsSystem::PhysicsSystem() :
		_impl(nullptr)
	{

	}

	PhysicsSystem::~PhysicsSystem()
	{

	}

	bool PhysicsSystem::Init(PhysicsDescription desc)
	{
		_impl = std::make_shared< PhysicsImplementation>();

		if (_impl == nullptr)
			return false;

		try
		{
			_impl->InitDefault();

			if (desc._usePvd == true)
				_impl->InitPvd();

			_impl->InitPhysics(desc._staticFriction, desc._dynamicFriction, desc._restitution, desc._gravity);

			_impl->InitCudaContextManager(desc._rendererType, desc._graphicsDevice);
		}
		catch (const std::string& errStr)
		{
			Release();
		}
		catch (...)
		{
			Release();
		}
	}

	void PhysicsSystem::Release()
	{
		if (_impl != nullptr)
		{
			_impl->Release();
			_impl = nullptr;
		}
	}
}