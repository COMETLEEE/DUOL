#include "PhysicsSceneImpl.h"

/* Actor */
#include "../Actor/PhysicsDynamicActorImpl.h"

/* Plane */
#include "../Shapes/PhysicsPlaneImpl.h"

/* Material */
#include "../PhysicsMaterialImpl.h"

#include <iostream>

namespace DUOLPhysics
{

	PhysicsScene::PhysicsScene() :
		_impl(std::make_shared<Impl>())
	{

	}

	std::weak_ptr<PhysicsPlane> PhysicsScene::CreatePlane(const tstring& keyName, std::weak_ptr<PhysicsMaterial> material, const PhysicsPlaneDesc& planeDesc)
	{
		if (_impl == nullptr)
			return {};

		auto mat = material.lock();

		if (mat == nullptr)
			return {};

		try
		{
			auto newPlane = std::make_shared<PhysicsPlane>();
			_planes[keyName] = newPlane;

			auto planeActor = newPlane->_impl->Create(_impl->_physics, mat->_impl->GetMaterial(), planeDesc);

			_impl->_scene->addActor(*planeActor);

			return newPlane;
		}
		catch (const std::string& errStr)
		{
			_planes[keyName]->Release();
			_planes.erase(keyName);

			std::cerr << errStr << std::endl;
		}
		catch (...)
		{
			_planes[keyName]->Release();
			_planes.erase(keyName);

			std::cerr << "Unknown Error." << std::endl;
		}

		return {};
	}

	std::weak_ptr<PhysicsDynamicActor> PhysicsScene::CreateDynamicActor(const tstring& keyName, const PhysicsDynamicDesc& dynamicDesc)
	{
		if (_impl == nullptr)
			return {};

		auto result = _dynamicActors.find(keyName);

		if (result != _dynamicActors.end())
			return result->second;

		try
		{
			auto newActor = std::make_shared<PhysicsDynamicActor>();
			_dynamicActors[keyName] = newActor;

			auto dynamicActor = newActor->_impl->Create(_impl->_physics, dynamicDesc);

			_impl->_scene->addActor(*dynamicActor);

			return newActor;
		}
		catch (const std::string& errStr)
		{
			_dynamicActors[keyName]->Release();
			_dynamicActors.erase(keyName);

			std::cerr << errStr << std::endl;
		}
		catch (...)
		{
			_dynamicActors[keyName]->Release();
			_dynamicActors.erase(keyName);

			std::cerr << "Unknown Error." << std::endl;
		}

		return {};
	}

	void PhysicsScene::Simulate(float deltaTime)
	{
		if (_impl != nullptr)
		{
			_impl->_scene->simulate(deltaTime);
			_impl->_scene->fetchResults(true);
		}
	}

	void PhysicsScene::Release()
	{
		if (_impl != nullptr)
			_impl = nullptr;
	}
}