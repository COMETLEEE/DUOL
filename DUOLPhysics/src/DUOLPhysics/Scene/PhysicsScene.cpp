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

			// Ʈ���� ������ ���¿� ���� �� ������ Ȯ�� �� �ִٸ� ����.
			_impl->_eventDispatcher->DeleteTriggerStayUserData(static_cast<PhysicsUserData*>(actor->userData));

			// �浹 �̺�Ʈ �Լ� ȣ���� ���� ���ؼ� ���� ������ �ʱ�ȭ.
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

			// Ʈ���� ������ ���¿� ���� �� ������ Ȯ�� �� �ִٸ� ����.
			_impl->_eventDispatcher->DeleteTriggerStayUserData(static_cast<PhysicsUserData*>(actor->userData));

			// �浹 �̺�Ʈ �Լ� ȣ���� ���� ���ؼ� ���� ������ �ʱ�ȭ.
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
		PxRaycastBufferN<100> pxHit;

		PxVec3 dir = ConvertVector3(direction);

		dir.normalize();

		_impl->_scene->raycast(ConvertVector3(position), dir, maxDistance, pxHit);

		if (pxHit.getNbTouches() == 0)
			return false;

		// Hit �־��ֱ�
		const PxRaycastHit* hits = pxHit.getTouches();

		for (uint32_t i = 0 ; i < pxHit.getNbTouches() ; i++)
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
		// outSpherecastHit._hitPosition = ConvertVector3(pxHit.block.position);
		outSpherecastHit._hitNormal = ConvertVector3(pxHit.block.normal);
		// outSpherecastHit._hitDistance = pxHit.block.distance;

		if (pxHit.block.actor->userData != nullptr)
			outSpherecastHit._userData = reinterpret_cast<PhysicsUserData*>(pxHit.block.actor->userData)->GetUserData();

		return true;
	}

	bool PhysicsScene::SpherecastAll(const DUOLMath::Vector3& origin, float radius, const DUOLMath::Vector3& direction,
		float maxDistance, std::vector<DUOLPhysics::RaycastHit>& outSpherecastHit)
	{
		PxSweepBufferN<100> pxHit;

		PxVec3 dir = ConvertVector3(direction);

		dir.normalize();

		PxSphereGeometry sphereGeometry(radius);

		PxTransform transform;

		transform.p = ConvertVector3(origin);

		transform.q = ConvertQuaternion(DUOLMath::Quaternion::Identity);

		PxQueryFilterData fd;

		_impl->_scene->sweep(sphereGeometry, transform, dir, maxDistance, pxHit, PxHitFlag::ePOSITION | PxHitFlag::eNORMAL);

		if (pxHit.getNbTouches() == 0)
			return false;

		const PxSweepHit* hits = pxHit.getTouches();

		for (uint32_t i = 0; i < pxHit.getNbTouches(); i++)
		{
			RaycastHit hit;

			hit._isBlocking = false;
			hit._hitPosition = ConvertVector3(hits[i].position);
			hit._hitNormal = ConvertVector3(hits[i].normal);
			hit._hitDistance = hits[i].distance;

			if (hits[i].actor != nullptr && hits[i].actor->userData != nullptr)
				hit._userData = reinterpret_cast<PhysicsUserData*>(hits[i].actor->userData)->GetUserData();

			outSpherecastHit.push_back(std::move(hit));
		}

		return true;
	}

	bool PhysicsScene::Boxcast(const DUOLMath::Vector3& center, const DUOLMath::Vector3& halfExtents, const DUOLMath::Quaternion& rotation,
		const DUOLMath::Vector3& direction, float maxDistance, DUOLPhysics::RaycastHit& outBoxcastHit)
	{
		PxSweepBuffer pxHit;

		PxVec3 dir = ConvertVector3(direction);

		dir.normalize();

		PxBoxGeometry boxGeometry;

		boxGeometry.halfExtents = ConvertVector3(halfExtents);

		PxTransform transform;

		transform.p = ConvertVector3(center);

		transform.q = ConvertQuaternion(rotation);

		_impl->_scene->sweep(boxGeometry, transform, dir, maxDistance, pxHit);

		if (pxHit.hasBlock == false)
		{
			outBoxcastHit._isBlocking = false;

			return false;
		}

		outBoxcastHit._isBlocking = true;
		outBoxcastHit._hitPosition = ConvertVector3(pxHit.block.position);
		outBoxcastHit._hitNormal = ConvertVector3(pxHit.block.normal);
		outBoxcastHit._hitDistance = pxHit.block.distance;

		if (pxHit.block.actor->userData != nullptr)
			outBoxcastHit._userData = reinterpret_cast<PhysicsUserData*>(pxHit.block.actor->userData)->GetUserData();

		return true;
	}

	bool PhysicsScene::BoxcastAll(const DUOLMath::Vector3& center, const DUOLMath::Vector3& halfExtents,
		const DUOLMath::Quaternion& rotation, const DUOLMath::Vector3& direction, float maxDistance,
		std::vector<DUOLPhysics::RaycastHit>& outBoxcastHit)
	{
		PxSweepBufferN<100> pxHit;

		PxVec3 dir = ConvertVector3(direction);

		dir.normalize();
		
		PxBoxGeometry boxGeometry;

		boxGeometry.halfExtents = ConvertVector3(halfExtents);

		PxTransform transform;

		transform.p = ConvertVector3(center);

		transform.q = ConvertQuaternion(rotation);

		_impl->_scene->sweep(boxGeometry, transform, dir, maxDistance, pxHit);

		if (pxHit.getNbTouches() == 0)
			return false;

		const PxSweepHit* hits = pxHit.getTouches();

		for (uint32_t i = 0; i < pxHit.getNbTouches(); i++)
		{
			RaycastHit hit;

			hit._isBlocking = false;
			// hit._hitPosition = ConvertVector3(hits[i].position); => sweep ������ default flag �� �������� ����.
			hit._hitNormal = ConvertVector3(hits[i].normal);
			// hit._hitDistance = hits[i].distance; => sweep ������ default flag �� �������� ����.

			if (hits[i].actor != nullptr && hits[i].actor->userData != nullptr)
				hit._userData = reinterpret_cast<PhysicsUserData*>(hits[i].actor->userData)->GetUserData();

			outBoxcastHit.push_back(std::move(hit));
		}

		return true;
	}

	bool PhysicsScene::CheckBox(const DUOLMath::Vector3& center, const DUOLMath::Vector3& halfExtents, const DUOLMath::Quaternion& rotation)
	{
		PxOverlapBuffer buf;

		PxBoxGeometry boxGeometry;

		boxGeometry.halfExtents = ConvertVector3(halfExtents);

		PxTransform transform(ConvertVector3(center), ConvertQuaternion(rotation));

		if (!_impl->_scene->overlap(boxGeometry, transform, buf))
			return false;

		return buf.hasAnyHits();
	}

	bool PhysicsScene::CheckSphere(const DUOLMath::Vector3& center, float radius)
	{
		PxOverlapBuffer buf;

		PxSphereGeometry sphereGeometry;

		sphereGeometry.radius = radius;

		PxTransform transform(ConvertVector3(center), ConvertQuaternion(DUOLMath::Quaternion::Identity));

		if (!_impl->_scene->overlap(sphereGeometry, transform, buf))
			return false;

		return buf.hasAnyHits();
	}

	bool PhysicsScene::OverlapBoxAll(const DUOLMath::Vector3& center, const DUOLMath::Vector3& halfExtents,
		const DUOLMath::Quaternion& rotation, std::vector<DUOLPhysics::RaycastHit>& outOverlapBox)
	{
		PxOverlapBufferN<100> buf;

		PxBoxGeometry boxGeometry;

		boxGeometry.halfExtents = ConvertVector3(halfExtents);

		PxTransform transform(ConvertVector3(center), ConvertQuaternion(rotation));

		if (!_impl->_scene->overlap(boxGeometry, transform, buf))
			return false;

		if (buf.getNbTouches() == 0)
			return false;

		const PxOverlapHit* hits = buf.getTouches();

		for (uint32_t i = 0; i < buf.getNbTouches(); i++)
		{
			RaycastHit hit;

			if (hits[i].actor != nullptr && hits[i].actor->userData != nullptr)
				hit._userData = reinterpret_cast<PhysicsUserData*>(hits[i].actor->userData)->GetUserData();

			outOverlapBox.push_back(std::move(hit));
		}

		return true;
	}

	bool PhysicsScene::OverlapSphereAll(const DUOLMath::Vector3& center, float radius,
		std::vector<DUOLPhysics::RaycastHit>& outOverlapSphere)
	{
		PxOverlapBufferN<100> buf;

		PxSphereGeometry sphereGeometry;

		sphereGeometry.radius = radius;

		PxTransform transform(ConvertVector3(center), ConvertQuaternion(DUOLMath::Quaternion::Identity));

		if (!_impl->_scene->overlap(sphereGeometry, transform, buf))
			return false;

		if (buf.getNbTouches() == 0)
			return false;

		const PxOverlapHit* hits = buf.getTouches();

		for (uint32_t i = 0; i < buf.getNbTouches(); i++)
		{
			RaycastHit hit;

			if (hits[i].actor != nullptr && hits[i].actor->userData != nullptr)
				hit._userData = reinterpret_cast<PhysicsUserData*>(hits[i].actor->userData)->GetUserData();

			outOverlapSphere.push_back(std::move(hit));
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