#include "DUOLPhysics/PhysicsSystem.h"

/* Scene */
#include "DUOLPhysics/PhysicsScene.h"

/* Material */
#include "DUOLPhysics/PhysicsMaterial.h"

/* Plane */
#include "DUOLPhysics/PhysicsPlane.h"

#include <iostream>

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

#pragma region Physics System Implementation Part
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

	class PhysicsSystemImpl
	{
		friend PhysicsSystem;

	private:
		PhysicsSystemImpl();

	public:
		~PhysicsSystemImpl();

	private:
		PxDefaultAllocator* _allocator;

		PxDefaultErrorCallback* _errorCallback;

		PxFoundation* _foundation;

		PxPhysics* _physics;

		PxDefaultCpuDispatcher* _cpuDispatcher;

		PxPvd* _pvd;

		PxCudaContextManager* _cudaContextManager;

	private:
		void InitDefault();

		void InitPhysics();

		void InitPvd();

		void InitCudaContextManager(PxCudaInteropMode::Enum interop, void* graphicsDevice);

		void Release();
	};

	PhysicsSystemImpl::PhysicsSystemImpl() :
		_allocator(nullptr)
		, _errorCallback(nullptr)
		, _foundation(nullptr)
		, _physics(nullptr)
		, _cpuDispatcher(nullptr)
		, _pvd(nullptr)
		, _cudaContextManager(nullptr)
	{

	}

	PhysicsSystemImpl::~PhysicsSystemImpl()
	{
		Release();
	}

	void PhysicsSystemImpl::InitDefault()
	{
		/* Allocator */
		if ((_allocator = new PxDefaultAllocator) == nullptr)
			ERROR_THROW("Failed to create PxDefaultAllocator.");

		/* Error Callback */
		if ((_errorCallback = new PxDefaultErrorCallback) == nullptr)
			ERROR_THROW("Failed to create PxDefaultErrorCallback.");

		/* CPU Dispatcher */
		_cpuDispatcher = PxDefaultCpuDispatcherCreate(2);

		if (_cpuDispatcher == nullptr)
			ERROR_THROW("Failed to create PxDefaultCpuDispatcher.");
	}

	void PhysicsSystemImpl::InitPhysics()
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

	void PhysicsSystemImpl::InitPvd()
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

	void PhysicsSystemImpl::InitCudaContextManager(PxCudaInteropMode::Enum interop, void* graphicsDevice)
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

	void PhysicsSystemImpl::Release()
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
#pragma endregion

	PhysicsSystem::PhysicsSystem() :
		_impl(nullptr)
	{

	}

	bool PhysicsSystem::Init(const PhysicsSystemDesc& desc)
	{
		_impl = std::shared_ptr<PhysicsSystemImpl>(new PhysicsSystemImpl);

		if (_impl == nullptr)
			return false;

		try
		{
			_impl->InitDefault();

			if (desc._usePvd == true)
				_impl->InitPvd();

			_impl->InitPhysics();

			return true;
		}
		catch (const std::string& errStr)
		{
			Release();

			std::cerr << errStr << std::endl;
		}
		catch (...)
		{
			Release();

			std::cerr << "Unknown Error." << std::endl;
		}

		return false;
	}

	void PhysicsSystem::Release()
	{
		if (_impl != nullptr)
		{
			_impl->Release();
			_impl = nullptr;
		}

		for (auto& iter : _scenes)
			iter.second.Release();

		_scenes.clear();
	}

	void PhysicsSystem::CreateScene(const DUOLCommon::tstring& keyName, const PhysicsSceneDesc& sceneDesc)
	{
		if (_impl == nullptr)
			return;

		auto result = _scenes.find(keyName);

		if (result != _scenes.end())
			return;

		try
		{
			HidedPhysicsSceneDesc hidedDesc;
			hidedDesc._default = sceneDesc;
			hidedDesc._physics = _impl->_physics;
			hidedDesc._cpuDispatcher = _impl->_cpuDispatcher;
			hidedDesc._cudaContextManager = _impl->_cudaContextManager;

			_scenes[keyName].CreateScene(hidedDesc);
		}
		catch (const std::string& errStr)
		{
			_scenes[keyName].Release();
			_scenes.erase(keyName);

			std::cerr << errStr << std::endl;
		}
		catch (...)
		{
			_scenes[keyName].Release();
			_scenes.erase(keyName);

			std::cerr << "Unknown Error." << std::endl;
		}
	}

	void PhysicsSystem::CreateMaterial(const DUOLCommon::tstring& keyName, const PhysicsMaterialDesc& materialDesc)
	{
		if (_impl == nullptr)
			return;

		auto result = _materials.find(keyName);

		if (result != _materials.end())
			return;

		try
		{
			HidedPhysicsMaterialDesc hidedDesc;
			hidedDesc._default = materialDesc;
			hidedDesc._physics = _impl->_physics;

			_materials[keyName].CreateMaterial(hidedDesc);
		}
		catch (const std::string& errStr)
		{
			_materials[keyName].Release();
			_materials.erase(keyName);

			std::cerr << errStr << std::endl;
		}
		catch (...)
		{
			_materials[keyName].Release();
			_materials.erase(keyName);

			std::cerr << "Unknown Error." << std::endl;
		}
	}

	void PhysicsSystem::CreatePlane(const DUOLCommon::tstring& keyName, const DUOLCommon::tstring& sceneName, const DUOLCommon::tstring& materialName, const PhysicsPlaneDesc& planeDesc)
	{
		if (_impl == nullptr)
			return;

		auto scene = _scenes.find(sceneName);

		if (scene != _scenes.end())
			return;

		auto material = _materials.find(materialName);

		if (material != _materials.end())
			return;

		try
		{
			HidedPhysicsPlaneDesc hidedDesc;
			hidedDesc._default = planeDesc;
			hidedDesc._physics = _impl->_physics;
			hidedDesc._material = material->second.GetMaterial();

			scene->second.AddActor(_planes[keyName].CreatePlane(hidedDesc));
		}
		catch (const std::string& errStr)
		{
			_planes[keyName].Release();
			_planes.erase(keyName);

			std::cerr << errStr << std::endl;
		}
		catch (...)
		{
			_planes[keyName].Release();
			_planes.erase(keyName);

			std::cerr << "Unknown Error." << std::endl;
		}
	}
}