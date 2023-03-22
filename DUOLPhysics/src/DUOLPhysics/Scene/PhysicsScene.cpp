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
#include "DUOLPhysics/Util/PhysicsDefines.h"

#include <iostream>

namespace DUOLPhysics
{
	PhysicsScene::PhysicsScene() :
		_impl(std::make_shared<Impl>())
	{

	}

	void PhysicsScene::SetRenderBufferOption(RenderBufferOption option, float value)
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("No Implementation was generated.");

			_impl->_scene->setVisualizationParameter(static_cast<PxVisualizationParameter::Enum>(option), value);
		}
		catch (const std::string& errStr)
		{
			DUOL_ENGINE_ERROR(errStr.c_str());
		}
		catch (...)
		{
			DUOL_ENGINE_ERROR("Unknown Error.");
		}
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
			DUOL_ENGINE_ERROR(errStr.c_str());
		}
		catch (...)
		{
			DUOL_ENGINE_ERROR("Unknown Error.");
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
			DUOL_ENGINE_ERROR(errStr.c_str());
		}
		catch (...)
		{
			DUOL_ENGINE_ERROR("Unknown Error.");
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
			DUOL_ENGINE_ERROR(errStr.c_str());
		}
		catch (...)
		{
			DUOL_ENGINE_ERROR("Unknown Error.");
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
			DUOL_ENGINE_ERROR(errStr.c_str());
		}
		catch (...)
		{
			DUOL_ENGINE_ERROR("Unknown Error.");
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

			// 트리거 스테이 상태에 있을 수 있으니 확인 후 있다면 삭제.
			_impl->_eventDispatcher->DeleteTriggerStayUserData(static_cast<PhysicsUserData*>(actor->userData));

			// 충돌 이벤트 함수 호출을 막기 위해서 유저 데이터 초기화.
			actor->userData = nullptr;

			_impl->_scene->removeActor(*actor);

			_staticActors.erase(result);

			return true;
		}
		catch (const std::string& errStr)
		{
			DUOL_ENGINE_ERROR(errStr.c_str());
		}
		catch (...)
		{
			DUOL_ENGINE_ERROR("Unknown Error.");
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

			// 트리거 스테이 상태에 있을 수 있으니 확인 후 있다면 삭제.
			_impl->_eventDispatcher->DeleteTriggerStayUserData(static_cast<PhysicsUserData*>(actor->userData));

			// 충돌 이벤트 함수 호출을 막기 위해서 유저 데이터 초기화.
			actor->userData = nullptr;

			_impl->_scene->removeActor(*actor, false);

			_dynamicActors.erase(result);

			return true;
		}
		catch (const std::string& errStr)
		{
			DUOL_ENGINE_ERROR(errStr.c_str());
		}
		catch (...)
		{
			DUOL_ENGINE_ERROR("Unknown Error.");
		}

		return false;
	}

	const SceneDebugData PhysicsScene::GetRenderBuffer()
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("No Implementation was generated.");

			if (_impl->_scene->getVisualizationParameter(PxVisualizationParameter::eSCALE) == 0.0f)
				ERROR_THROW("Failure to get Render Buffer. (SCALE value not set. Please set render buffer option.)");
			
			auto* rb = &_impl->GetRenderBuffer();

			return SceneDebugData
			{
				reinterpret_cast<const SceneDebugData::VertexData*>(rb->getLines()),
				rb->getNbLines()
			};
		}
		catch (const std::string& errStr)
		{
			DUOL_ENGINE_ERROR(errStr.c_str());
		}
		catch (...)
		{
			DUOL_ENGINE_ERROR("Unknown Error.");
		}

		return SceneDebugData{ nullptr, 0 };
	}

	bool PhysicsScene::Raycast(const DUOLMath::Vector3& position, const DUOLMath::Vector3& direction, float maxDistance,
		DUOLPhysics::RaycastHit& outRaycastHit)
	{
		PxRaycastBuffer pxHit;

		PxVec3 dir = ConvertVector3(direction);

		dir.normalize();

		_impl->_scene->raycast(ConvertVector3(position), dir, maxDistance, pxHit);

		if (pxHit.hasBlock == false)
		{
			outRaycastHit._isBlocking = false;

			return false;
		}

		outRaycastHit._isBlocking = true;
		outRaycastHit._hitPosition = ConvertVector3(pxHit.block.position);
		outRaycastHit._hitNormal = ConvertVector3(pxHit.block.normal);
		outRaycastHit._hitDistance = pxHit.block.distance;

		if (pxHit.block.actor->userData != nullptr)
			outRaycastHit._userData = reinterpret_cast<PhysicsUserData*>(pxHit.block.actor->userData)->GetUserData();

		return true;
	}

	bool PhysicsScene::RaycastAll(const DUOLMath::Vector3& position, const DUOLMath::Vector3& direction,
		float maxDistance, std::vector<DUOLPhysics::RaycastHit>& outRaycastHits)
	{
		PxRaycastBuffer pxHit;

		PxVec3 dir = ConvertVector3(direction);

		dir.normalize();

		_impl->_scene->raycast(ConvertVector3(position), dir, maxDistance, pxHit);

		// 블로킹 없으면 빠이
		if (pxHit.hasBlock == false)
		{
			return false;
		}

		// Block 넣어주기
		RaycastHit block;

		block._isBlocking = true;
		block._hitPosition = ConvertVector3(pxHit.block.position);
		block._hitNormal = ConvertVector3(pxHit.block.normal);
		block._hitDistance = pxHit.block.distance;

		if (pxHit.block.actor->userData != nullptr)
			block._userData = reinterpret_cast<PhysicsUserData*>(pxHit.block.actor->userData)->GetUserData();

		outRaycastHits.push_back(std::move(block));

		// Hit 넣어주기
		const PxRaycastHit* hits = pxHit.getTouches();

		for (uint32_t i = 1 ; i < pxHit.getNbTouches() ; i++)
		{
			RaycastHit hit;

			hit._isBlocking = false;
			hit._hitPosition = ConvertVector3(hits[i].position);
			hit._hitNormal = ConvertVector3(hits[i].normal);
			hit._hitDistance = hits[i].distance;

			if (pxHit.block.actor->userData != nullptr)
				hit._userData = reinterpret_cast<PhysicsUserData*>(pxHit.block.actor->userData)->GetUserData();

			outRaycastHits.push_back(std::move(hit));
		}

		return true;
	}

	bool PhysicsScene::Spherecast(const DUOLMath::Vector3& origin, float radius, const DUOLMath::Vector3& direction,
	                              float maxDistance, DUOLPhysics::RaycastHit& outSpherecastHit)
	{
		PxSweepBuffer pxHit;

		PxVec3 dir = ConvertVector3(direction);

		dir.normalize();

		PxSphereGeometry sphereGeometry(radius);

		PxTransform transform;

		transform.p = ConvertVector3(origin);

		transform.q = ConvertQuaternion(DUOLMath::Quaternion::Identity);

		_impl->_scene->sweep(sphereGeometry, transform, dir, maxDistance, pxHit);

		if (pxHit.hasBlock == false)
		{
			outSpherecastHit._isBlocking = false;

			return false;
		}

		outSpherecastHit._isBlocking = true;
		outSpherecastHit._hitPosition = ConvertVector3(pxHit.block.position);
		outSpherecastHit._hitNormal = ConvertVector3(pxHit.block.normal);
		outSpherecastHit._hitDistance = pxHit.block.distance;

		if (pxHit.block.actor->userData != nullptr)
			outSpherecastHit._userData = reinterpret_cast<PhysicsUserData*>(pxHit.block.actor->userData)->GetUserData();

		return true;
	}

	bool PhysicsScene::SpherecastAll(const DUOLMath::Vector3& origin, float radius, const DUOLMath::Vector3& direction,
		float maxDistance, std::vector<DUOLPhysics::RaycastHit>& outSpherecastHit)
	{
		PxSweepBuffer pxHit;

		PxVec3 dir = ConvertVector3(direction);

		dir.normalize();

		PxSphereGeometry sphereGeometry(radius);

		PxTransform transform;

		transform.p = ConvertVector3(origin);

		transform.q = ConvertQuaternion(DUOLMath::Quaternion::Identity);

		_impl->_scene->sweep(sphereGeometry, transform, dir, maxDistance, pxHit, PxHitFlag::eDEFAULT);

		if (pxHit.hasBlock == false)
			return false;

		RaycastHit block;

		block._isBlocking = true;
		block._hitPosition = ConvertVector3(pxHit.block.position);
		block._hitNormal = ConvertVector3(pxHit.block.normal);
		block._hitDistance = pxHit.block.distance;

		if (pxHit.block.actor->userData != nullptr)
			block._userData = reinterpret_cast<PhysicsUserData*>(pxHit.block.actor->userData)->GetUserData();

		outSpherecastHit.push_back(block);

		const PxSweepHit* hits = pxHit.getTouches();

		for (uint32_t i = 1; i < pxHit.getNbTouches(); i++)
		{
			RaycastHit hit;

			hit._isBlocking = false;
			hit._hitPosition = ConvertVector3(hits[i].position);
			hit._hitNormal = ConvertVector3(hits[i].normal);
			hit._hitDistance = hits[i].distance;

			if (pxHit.block.actor->userData != nullptr)
				hit._userData = reinterpret_cast<PhysicsUserData*>(pxHit.block.actor->userData)->GetUserData();

			outSpherecastHit.push_back(hit);
		}

		return true;
	}

	DUOLMath::Vector3 PhysicsScene::GetGravity()
	{
		if (_impl != nullptr)
			return ConvertVector3(_impl->_scene->getGravity());
		else
			return DUOLMath::Vector3::Zero;
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