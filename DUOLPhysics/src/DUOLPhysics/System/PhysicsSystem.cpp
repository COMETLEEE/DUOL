#include "PhysicsSystemImpl.h"

/* Scene */
#include "../Scene/PhysicsSceneImpl.h"

/* Material */
#include "../Material/PhysicsMaterialImpl.h"

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
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("No Implementation was generated.");

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

		for (auto& iter : _shapes)
			iter.second->Release();

		_shapes.clear();

		if (_impl != nullptr)
			_impl->Release();
	}

	std::weak_ptr<PhysicsScene> PhysicsSystem::CreateScene(const DUOLCommon::tstring& keyName, const PhysicsSceneDesc& sceneDesc)
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("No Implementation was generated.");

			auto result = _scenes.find(keyName);

			if (result != _scenes.end())
				return result->second;

			auto newScene = std::make_shared<PhysicsScene>();
			newScene->_impl->Create(_impl->_physics, _impl->_cooking, _impl->_cpuDispatcher, _impl->_cudaContextManager, sceneDesc);
			
			_scenes[keyName] = newScene;

			return _scenes[keyName];
		}
		catch (const std::string& errStr)
		{
			std::cerr << errStr << std::endl;
		}
		catch (...)
		{
			std::cerr << "Unknown Error." << std::endl;
		}

		return {};
	}

	std::weak_ptr<PhysicsMaterial> PhysicsSystem::CreateMaterial(const DUOLCommon::tstring& keyName, const PhysicsMaterialDesc& materialDesc)
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("No Implementation was generated.");

			auto result = _materials.find(keyName);

			if (result != _materials.end())
				return result->second;

			auto newMaterial = std::make_shared<PhysicsMaterial>();
			newMaterial->_impl->Create(_impl->_physics, materialDesc);

			_materials[keyName] = newMaterial;

			return newMaterial;
		}
		catch (const std::string& errStr)
		{
			std::cerr << errStr << std::endl;
		}
		catch (...)
		{
			std::cerr << "Unknown Error." << std::endl;
		}

		return {};
	}

	bool PhysicsSystem::DestroyScene(const tstring& keyName)
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("No Implementation was generated.");

			auto result = _scenes.find(keyName);

			if (result == _scenes.end())
				return false;

			_scenes.erase(result);

			return true;
		}
		catch (const std::string& errStr)
		{
			std::cerr << errStr << std::endl;
		}
		catch (...)
		{
			std::cerr << "Unknown Error." << std::endl;
		}

		return false;
	}

	bool PhysicsSystem::DestroyMaterial(const tstring& keyName)
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("No Implementation was generated.");

			auto result = _materials.find(keyName);

			if (result == _materials.end())
				return false;

			_materials.erase(result);

			return true;
		}
		catch (const std::string& errStr)
		{
			std::cerr << errStr << std::endl;
		}
		catch (...)
		{
			std::cerr << "Unknown Error." << std::endl;
		}

		return false;
	}

	bool PhysicsSystem::DestroyShape(const tstring& keyName)
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("No Implementation was generated.");

			auto result = _shapes.find(keyName);

			if (result == _shapes.end())
				return false;

			_shapes.erase(result);

			return true;
		}
		catch (const std::string& errStr)
		{
			std::cerr << errStr << std::endl;
		}
		catch (...)
		{
			std::cerr << "Unknown Error." << std::endl;
		}

		return false;
	}
}