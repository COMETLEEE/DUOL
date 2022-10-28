#include "DUOLPhysics/System/PhysicsSystem.h"
#include "../src/DUOLPhysics/System/PhysicsSystemImpl.h"

/* Scene */
#include "../src/DUOLPhysics/Scene/PhysicsSceneImpl.h"

/* Material */
#include "DUOLPhysics/PhysicsMaterial.h"

#include <iostream>

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
			return {};

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

	void PhysicsSystem::CreateDynamic(const tstring& keyName, const tstring& materialName, const tstring& shapeName)
	{

	}
}