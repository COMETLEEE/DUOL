#include "PhysicsSystemImpl.h"

/* Shape */
#include "../Shapes/PhysicsShapeBaseImpl.h"

/* Etc */
#include "DUOLCommon/DeviceHelper.h"

#pragma comment(lib, "../Build/x64/Debug/DUOLCommon.lib")

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
	PhysicsSystem::Impl::Impl() :
		_allocator(nullptr)
		, _errorCallback(nullptr)
		, _foundation(nullptr)
		, _physics(nullptr)
		, _cooking(nullptr)
		, _cpuDispatcher(nullptr)
		, _pvd(nullptr)
		, _cudaContextManager(nullptr)
	{

	}

	PhysicsSystem::Impl::~Impl()
	{
		Release();
	}

	void PhysicsSystem::Impl::InitDefault()
	{
		/* Allocator */
		if ((_allocator = new PxDefaultAllocator) == nullptr)
			ERROR_THROW("Failed to create PxDefaultAllocator.");

		/* Error Callback */
		if ((_errorCallback = new PxDefaultErrorCallback) == nullptr)
			ERROR_THROW("Failed to create PxDefaultErrorCallback.");

		/* Foundation */
		_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, *_allocator, *_errorCallback);

		if (_foundation == nullptr)
			ERROR_THROW("Failed to create PxFoundation.");
	}

	void PhysicsSystem::Impl::InitPhysics()
	{
		/* Physics */
		_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *_foundation, PxTolerancesScale(), true, _pvd);

		if (_physics == nullptr)
			ERROR_THROW("Failed to create PxPhysics.");

		/* Cooking */
		_cooking = PxCreateCooking(PX_PHYSICS_VERSION, *_foundation, PxCookingParams(PxTolerancesScale()));

		if (_cooking == nullptr)
			ERROR_THROW("Failed to create PxCooking.");

		/* CPU Dispatcher */
		_cpuDispatcher = PxDefaultCpuDispatcherCreate(DUOLCommon::DeviceHelper::GetCPUInfo()._core / 2);

		if (_cpuDispatcher == nullptr)
			ERROR_THROW("Failed to create PxDefaultCpuDispatcher.");
	}

	void PhysicsSystem::Impl::InitPvd()
	{
		PhysicsShapeBase::Impl::_usePVD = true;

		/* PhysX Visual Debugger */
		_pvd = PxCreatePvd(*_foundation);

		if (_pvd == nullptr)
			ERROR_THROW("Failed to create PxPvd.");

		/* Transport */
		PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);

		if (transport == nullptr)
			ERROR_THROW("Failed to create PxPvdTransport.");

		_pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);
	}

	void PhysicsSystem::Impl::InitCudaContextManager(PxCudaInteropMode::Enum interop, void* graphicsDevice)
	{
		/* Cuda Context Manager */
		PxCudaContextManagerDesc ccmDesc;

		ccmDesc.interopMode = interop;
		ccmDesc.graphicsDevice = graphicsDevice;

		_cudaContextManager = PxCreateCudaContextManager(*_foundation, ccmDesc, PxGetProfilerCallback());

		if (_cudaContextManager == nullptr)
			ERROR_THROW("Failed to create PxCudaContextManager.");

		if (_cudaContextManager->contextIsValid() != true)
		{
			_cudaContextManager->release();
			_cudaContextManager = nullptr;

			ERROR_THROW("PxCudaContextManager has not a valid CUDA context.");
		}
	}

	void PhysicsSystem::Impl::Release()
	{
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

		if (_cooking != nullptr)
		{
			_cooking->release();
			_cooking = nullptr;
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

		delete _errorCallback;
		_errorCallback = nullptr;

		delete _allocator;
		_allocator = nullptr;
	}
}