#include "PhysicsSystemImpl.h"

/* Scene */
#include "../Scene/PhysicsSceneImpl.h"

/* Material */
#include "../PhysicsMaterialImpl.h"

/* etc */
#include "../Util/PhysicsTypeConverter.h"

namespace DUOLPhysics
{
	PhysicsSystem::PhysicsSystem() :
		_impl(std::make_shared<Impl>())
	{

	}

	PhysicsSystem::~PhysicsSystem()
	{
		Release();
	}

	bool PhysicsSystem::Init(const PhysicsSystemDesc& desc)
	{
		if (_impl == nullptr)
			return false;

		try
		{
			_impl->InitDefault();

			if (desc._usePvd == true)
				_impl->InitPvd();

			_impl->InitPhysics();

			_impl->InitCudaContextManager(PxCudaInteropMode::NO_INTEROP, nullptr);

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
		for (auto& iter : _scenes)
			iter.second->Release();

		_scenes.clear();

		for (auto& iter : _materials)
			iter.second->Release();

		_materials.clear();

		if (_impl != nullptr)
		{
			_impl->Release();
			_impl = nullptr;
		}
	}

	std::weak_ptr<PhysicsScene> PhysicsSystem::CreateScene(const DUOLCommon::tstring& keyName, const PhysicsSceneDesc& sceneDesc)
	{
		if (_impl == nullptr)
			return {};

		auto result = _scenes.find(keyName);

		if (result != _scenes.end())
			return result->second;

		try
		{
			auto newScene = std::make_shared<PhysicsScene>();
			_scenes[keyName] = newScene;

			newScene->_impl->Create(_impl->_physics, _impl->_cpuDispatcher, _impl->_cudaContextManager, sceneDesc);

			return _scenes[keyName];
		}
		catch (const std::string& errStr)
		{
			_scenes[keyName]->Release();
			_scenes.erase(keyName);

			std::cerr << errStr << std::endl;
		}
		catch (...)
		{
			_scenes[keyName]->Release();
			_scenes.erase(keyName);

			std::cerr << "Unknown Error." << std::endl;
		}

		return {};
	}

	std::weak_ptr<PhysicsMaterial> PhysicsSystem::CreateMaterial(const DUOLCommon::tstring& keyName, const PhysicsMaterialDesc& materialDesc)
	{
		if (_impl == nullptr)
			return {};

		auto result = _materials.find(keyName);

		if (result != _materials.end())
			return result->second;

		try
		{
			auto newMaterial = std::make_shared<PhysicsMaterial>();
			_materials[keyName] = newMaterial;

			newMaterial->_impl->Create(_impl->_physics, materialDesc);

			return newMaterial;
		}
		catch (const std::string& errStr)
		{
			_materials[keyName]->Release();
			_materials.erase(keyName);

			std::cerr << errStr << std::endl;
		}
		catch (...)
		{
			_materials[keyName]->Release();
			_materials.erase(keyName);

			std::cerr << "Unknown Error." << std::endl;
		}

		return {};
	}
}