#include "PhysicsSceneImpl.h"

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

			auto realScene = newPlane->_impl->Create(_impl->_physics, mat->_impl->GetMaterial(), planeDesc);

			_impl->_scene->addActor(*realScene);
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

	void PhysicsScene::Release()
	{
		if (_impl != nullptr)
			_impl = nullptr;
	}
}