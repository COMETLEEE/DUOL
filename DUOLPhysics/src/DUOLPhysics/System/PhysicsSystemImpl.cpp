#include "PhysicsSystemImpl.h"

/* Etc */
#include "DUOLCommon/DeviceHelper.h"

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
	PxCudaInteropMode::Enum ConvertInteropMode(RendererType rendererType)
	{
		switch (rendererType)
		{
		case RendererType::OPENGL: return PxCudaInteropMode::OGL_INTEROP;
		case RendererType::DIRECTX10: return PxCudaInteropMode::D3D10_INTEROP;
		case RendererType::DIRECTX11: return PxCudaInteropMode::D3D11_INTEROP;
		}

		return PxCudaInteropMode::NO_INTEROP;
	}

	PhysicsSystem::Impl::Impl() :
		_allocator(nullptr)
		, _errorCallback(nullptr)
		, _foundation(nullptr)
		, _physics(nullptr)
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

		/* CPU Dispatcher */
		_cpuDispatcher = PxDefaultCpuDispatcherCreate(DUOLCommon::DeviceHelper::GetCPUInfo()._core / 2);

		if (_cpuDispatcher == nullptr)
			ERROR_THROW("Failed to create PxDefaultCpuDispatcher.");
	}

	void PhysicsSystem::Impl::InitPhysics()
	{
		/* Foundation */
		_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, *_allocator, *_errorCallback);

		if (_foundation == nullptr)
			ERROR_THROW("Failed to create PxFoundation.");

		/* Physics */
		_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *_foundation, PxTolerancesScale(), true, _pvd);

		if (_physics == nullptr)
			ERROR_THROW("Failed to create PxPhysics.");
	}

	void PhysicsSystem::Impl::InitPvd()
	{
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
		delete _allocator;
		_allocator = nullptr;

		delete _errorCallback;
		_errorCallback = nullptr;

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
}