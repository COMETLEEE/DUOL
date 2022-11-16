#include "PhysicsSceneImpl.h"

/* Actor */
#include "../Actor/PhysicsStaticActorImpl.h"
#include "../Actor/PhysicsDynamicActorImpl.h"

/* Plane */
#include "../Shapes/PhysicsPlaneImpl.h"

/* Material */
#include "../Material/PhysicsMaterialImpl.h"

/* etc */
#include "../Util/PhysicsTypeConverter.h"

#include <iostream>

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

	PhysicsScene::PhysicsScene() :
		_impl(std::make_shared<Impl>())
	{

	}

	std::weak_ptr<PhysicsPlane> PhysicsScene::CreatePlane(const tstring& keyName, std::weak_ptr<PhysicsMaterial> material, const PhysicsPlaneDesc& planeDesc)
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("No Implementation was generated.");

			auto mat = material.lock();

			if (mat == nullptr)
				return {};

			auto newPlane = std::make_shared<PhysicsPlane>();
			auto planeActor = newPlane->_impl->Create(_impl->_physics, mat->_impl->GetMaterial(), planeDesc);

			_impl->_scene->addActor(*planeActor);

			_planes[keyName] = newPlane;

			return newPlane;
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

	std::weak_ptr<PhysicsStaticActor> PhysicsScene::CreateStaticActor(const tstring& keyName, const PhysicsActorDesc& staticDesc)
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("No Implementation was generated.");

			auto result = _staticActors.find(keyName);

			if (result != _staticActors.end())
				return result->second;

			auto newActor = std::make_shared<PhysicsStaticActor>();
			auto staticActor = newActor->_impl->Create(_impl->_physics, staticDesc);

			_impl->_scene->addActor(*staticActor);

			_staticActors[keyName] = newActor;

			return newActor;
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

	std::weak_ptr<PhysicsDynamicActor> PhysicsScene::CreateDynamicActor(const tstring& keyName, const PhysicsActorDesc& dynamicDesc)
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("No Implementation was generated.");

			auto result = _dynamicActors.find(keyName);

			if (result != _dynamicActors.end())
				return result->second;

			auto newActor = std::make_shared<PhysicsDynamicActor>();
			auto dynamicActor = newActor->_impl->Create(_impl->_physics, dynamicDesc);

			_impl->_scene->addActor(*dynamicActor);

			_dynamicActors[keyName] = newActor;

			return newActor;
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

	bool PhysicsScene::DestroyPlane(const tstring& keyName)
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("No Implementation was generated.");

			auto result = _planes.find(keyName);

			if (result == _planes.end())
				return false;

			_planes.erase(result);

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

	bool PhysicsScene::DestroyStaticActor(const tstring& keyName)
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("No Implementation was generated.");

			auto result = _staticActors.find(keyName);

			if (result == _staticActors.end())
				return false;

			auto* actor = result->second->_impl->GetActor();

			if (actor == nullptr)
				ERROR_THROW("Failure to Destroy Actor.");

			_impl->_scene->removeActor(*actor);

			_staticActors.erase(result);

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

	bool PhysicsScene::DestroyDynamicActor(const tstring& keyName)
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("No Implementation was generated.");

			auto result = _dynamicActors.find(keyName);

			if (result == _dynamicActors.end())
				return false;

			auto* actor = result->second->_impl->GetActor();

			if (actor == nullptr)
				ERROR_THROW("Failure to Destroy Actor.");

			_impl->_scene->removeActor(*actor);

			_dynamicActors.erase(result);

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

	const SceneDebugData PhysicsScene::GetRenderBuffer()
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("No Implementation was generated.");

			auto* rb = &_impl->GetRenderBuffer();

			return SceneDebugData
			{
				reinterpret_cast<const SceneDebugData::VertexData*>(rb->getLines()),
				rb->getNbLines()
			};
		}
		catch (const std::string& errStr)
		{
			std::cerr << errStr << std::endl;
		}
		catch (...)
		{
			std::cerr << "Unknown Error." << std::endl;
		}

		return SceneDebugData{ nullptr, 0 };
	}

	RaycastHit PhysicsScene::Raycast(const DUOLMath::Vector3& position, const DUOLMath::Vector3& direction, float maxDistance)
	{
		PxRaycastBuffer pxHit;

		_impl->_scene->raycast(ConvertVector3(position), ConvertVector3(direction), maxDistance, pxHit);

		RaycastHit hit = {};

		hit._isBlocking = pxHit.hasBlock;
		
		if (pxHit.hasBlock == true)
		{
			hit._hitPosition = ConvertVector3(pxHit.block.position);
			hit._hitNormal = ConvertVector3(pxHit.block.normal);
			hit._hitDistance = pxHit.block.distance;

			if (pxHit.block.actor->userData != nullptr)
				hit._userData = reinterpret_cast<PhysicsUserData*>(pxHit.block.actor->userData)->GetUserData();
		}

		return hit;
	}

	void PhysicsScene::Simulate(float deltaTime)
	{
		if (_impl != nullptr)
		{
			_impl->_scene->simulate(deltaTime);
			_impl->_scene->fetchResults(true);

			_impl->_eventDispatcher->SendTriggerStayEvent();
		}
	}

	void PhysicsScene::Release()
	{
		if (_impl != nullptr)
			_impl->Release();
	}
}