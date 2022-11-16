#include "DUOLGameEngine/Manager/PhysicsManager.h"

#include "DUOLPhysics/System/PhysicsSystem.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/BoxCollider.h"
#include "DUOLGameEngine/ECS/Component/ColliderBase.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"
#include "DUOLGameEngine/ECS/Object/PhysicsMaterial.h"
#include "DUOLGameEngine/Manager/ResourceManager.h"
#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"


namespace DUOLGameEngine
{
	PhysicsManager::PhysicsManager() :
		_fixedTimeStep(0.02f)
	{
		
	}

	PhysicsManager::~PhysicsManager()
	{
		_physicsSystem.reset();
	}

	void PhysicsManager::Initialize()
	{
		// 0. Physics engine on.
		DUOLPhysics::PhysicsSystemDesc physicsDesc {false };

		_physicsSystem = std::make_shared<DUOLPhysics::PhysicsSystem>();

		_physicsSystem->Init(physicsDesc);

		// 1. Create physics scenes.
		DUOLPhysics::PhysicsSceneDesc physicsSceneDesc{ DUOLMath::Vector3(0.f, -9.8f, 0.f)};

		_physicsScene = _physicsSystem->CreateScene(TEXT("DUOL_PHYSICS"), physicsSceneDesc);
	}

	void PhysicsManager::UnInitialize()
	{
		_fixedUpdateEventHandlers.RemoveAllListeners();

		_physicsSystem->Release();
	}

	void PhysicsManager::InitializePhysicsCollider(const std::shared_ptr<DUOLGameEngine::ColliderBase>& collider)
	{
		std::shared_ptr<DUOLGameEngine::BoxCollider> isBox = std::dynamic_pointer_cast<DUOLGameEngine::BoxCollider>(collider);

		const DUOLCommon::tstring uuidStr = DUOLCommon::StringHelper::ToTString(collider->GetUUID());

		// 1. Shape Desc => �ϴ� Size ���� ..?
		// TODO : Flag, Material ���� ������ �ʿ��ϴ� ..
		DUOLPhysics::PhysicsShapeDesc shapeDesc;

		// ������ ���� Default Material ..?
		shapeDesc._material = DUOLGameEngine::ResourceManager::GetInstance()->GetPhysicsMaterial(TEXT("Default"))->GetPhysicsMaterial();

		shapeDesc._flag = DUOLPhysics::ShapeFlag::COLLIDER | DUOLPhysics::ShapeFlag::SCENE_QUERY;

		// shape�� local pose�� �ٲٱ� ���ؼ� ���������� exclusive ..!
		shapeDesc._isExclusive = true;

		// Box Collider
		if (isBox != nullptr)
		{
			// 2. �ݶ��̴��� �����Ǿ� �ִ� ��� �ʱ�ȭ���ݴϴ�.
			const DUOLMath::Vector3 boxCenter = isBox->_center;

			const DUOLMath::Vector3 boxExtents = isBox->_size;

			shapeDesc._box._halfExtentX = boxExtents.x / 2.f;
			shapeDesc._box._halfExtentY = boxExtents.y / 2.f;
			shapeDesc._box._halfExtentZ = boxExtents.z / 2.f;

			isBox->_physicsBox = _physicsSystem->CreateShape<DUOLPhysics::PhysicsBox>(uuidStr, shapeDesc);

			isBox->_physicsBox.lock()->SetLocalPose(boxCenter);

			isBox->_physicsActor = isBox->GetGameObject()->_physicsActor;
		}
		// Sphere Collider
		else
		{
			
		}
	}

	void PhysicsManager::InitializePhysicsGameObject(const std::shared_ptr<DUOLGameEngine::GameObject>& gameObject)
	{
		const std::vector<std::shared_ptr<DUOLGameEngine::ColliderBase>> hasCols
			= gameObject->GetComponents<DUOLGameEngine::ColliderBase>();

		const std::shared_ptr<DUOLGameEngine::Rigidbody> hasRigid
			= gameObject->GetComponent<DUOLGameEngine::Rigidbody>();

		const DUOLCommon::tstring uuidStr = DUOLCommon::StringHelper::ToTString(gameObject->GetUUID());

		// 1. Actor Desc�� ���� ���� ����.
		DUOLPhysics::PhysicsActorDesc actorDesc;

		actorDesc._transform = Matrix::Identity;

		// ���� �ùķ��̼ǰ� ������ ���� ���� ������Ʈ�Դϴ�.
		if ((hasCols.size() == 0) && (hasRigid == nullptr))
			return;
		// Static ���� �ùķ��̼� ���� ������Ʈ�Դϴ�.
		else if ((hasCols.size() != 0) && (hasRigid == nullptr))
		{
			const std::weak_ptr<DUOLPhysics::PhysicsStaticActor> sActor =
				_physicsScene.lock()->CreateStaticActor(uuidStr, actorDesc);

			gameObject->_physicsActor = sActor;

			_physicsStaticActors.insert({ uuidStr, { gameObject->GetTransform(), sActor } });

			// �ݶ��̴��� ���� ������ Shape ����
			for (auto& col : hasCols)
				InitializePhysicsCollider(col);
		}
		// Dynamic ���� �ùķ��̼� ���� ������Ʈ�Դϴ�.
		else if (hasRigid != nullptr)
		{
			// dynamic actor
			const std::weak_ptr<DUOLPhysics::PhysicsDynamicActor> dActor =
				_physicsScene.lock()->CreateDynamicActor(uuidStr, actorDesc);

			gameObject->_physicsActor = dActor;

			_physicsDynamicActors.insert({ uuidStr, { gameObject->GetTransform(), dActor } });

			// caching actor in rigidbody and initialize.
			hasRigid->OnInitializeDynamicActor(dActor);

			// �ݶ��̴��� ���� ������ Shape ����
			for (auto& col : hasCols)
				InitializePhysicsCollider(col);
		}
	}

	void PhysicsManager::InitializeCurrentGameScene(const std::list<std::shared_ptr<DUOLGameEngine::GameObject>>& gameObjectsInScene)
	{
		// 1. before PhysX scene clear.
		for (auto& [key, value] : _physicsStaticActors)
			_physicsScene.lock()->DestroyStaticActor(key);

		for (auto& [key, value] : _physicsDynamicActors)
			_physicsScene.lock()->DestroyDynamicActor(key);

		_physicsStaticActors.clear();

		_physicsDynamicActors.clear();

		// 2. sync with current game scene.
		for (auto& gameObject : gameObjectsInScene)
			InitializePhysicsGameObject(gameObject);
	}

	void PhysicsManager::Update(float deltaTime)
	{
		static float accumTime = 0.f;

		accumTime += deltaTime;

		if (accumTime > _fixedTimeStep)
		{
			const int physicsUpdateCount = static_cast<int>(accumTime / _fixedTimeStep);

			for (int i = 0; i < physicsUpdateCount; i++)
			{
				ApplyPhysicsTransformBeforeSimulate();

				_physicsScene.lock()->Simulate(_fixedTimeStep);

				ApplyPhysicsSimulateResult();

				FixedUpdate();

				TriggerUpdate();

				CollisionUpdate();
			}

			accumTime = accumTime - static_cast<float>(physicsUpdateCount) * _fixedTimeStep;
		}
	}

	void PhysicsManager::ApplyPhysicsTransformBeforeSimulate()
	{
		for (auto& [key, value] : _physicsStaticActors)
		{
			const std::shared_ptr<DUOLGameEngine::Transform> transform = value.first.lock();

			const std::shared_ptr<DUOLPhysics::PhysicsStaticActor> actor = value.second.lock();

			// �ش� Ʈ�������� ���� ������Ʈ�� �Ҹ�Ǿ����ϴ�.
			if (transform == nullptr)
			{
				// TODO : Actor �Ҹ�
			}
			else
			{
				const DUOLMath::Vector3& worldPos = transform->GetWorldPosition();

				const DUOLMath::Quaternion& worldRot = transform->GetWorldRotation();

				actor->SetGlobalPose(worldPos);

				actor->SetGlobalPose(worldRot);
			}
		}

		for (auto& [key, value] : _physicsDynamicActors)
		{
			const std::shared_ptr<DUOLGameEngine::Transform> transform = value.first.lock();

			const std::shared_ptr<DUOLPhysics::PhysicsDynamicActor> actor = value.second.lock();

			// �ش� Ʈ�������� ���� ������Ʈ�� �Ҹ�Ǿ����ϴ�.
			if (transform == nullptr)
			{
				// TODO : Actor �Ҹ�
			}
			else
			{
				const DUOLMath::Vector3& worldPos = transform->GetWorldPosition();

				const DUOLMath::Quaternion& worldRot = transform->GetWorldRotation();

				DUOLPhysics::PhysicsPose pose;

				pose._position = worldPos;

				pose._quaternion = worldRot;

				actor->SetGlobalPose(pose);
			}
		}
	}

	void PhysicsManager::ApplyPhysicsSimulateResult()
	{
		// �ùķ��̼��� Dynamic actor�鸸�� Global pose�� ������ ��ģ��.
		for (auto& [key, value] : _physicsDynamicActors)
		{
			const std::shared_ptr<DUOLGameEngine::Transform> transform = value.first.lock();

			const std::shared_ptr<DUOLPhysics::PhysicsDynamicActor> actor = value.second.lock();

			if (transform == nullptr)
			{
				// TODO : ��ü ���� ���� �־����Ŵ� ..
			}
			else
			{
				const DUOLPhysics::PhysicsPose& globalPose = actor->GetGlobalPose();

				transform->SetPosition(globalPose._position);

				transform->SetRotation(globalPose._quaternion);
			}
		}
	}

	void PhysicsManager::FixedUpdate()
	{
		// Invoke event handlers.
		_fixedUpdateEventHandlers.Invoke(_fixedTimeStep);
	}

	DUOLCommon::EventHandlerID PhysicsManager::AddFixedUpdateEventHandler(std::function<void(float)> functor)
	{
		return _fixedUpdateEventHandlers += std::move(functor);
	}

	bool PhysicsManager::RemoveFixedUpdateEventHandler(DUOLCommon::EventHandlerID id)
	{
		return _fixedUpdateEventHandlers -= id;
	}

	void PhysicsManager::TriggerUpdate()
	{

	}

	void PhysicsManager::CollisionUpdate()
	{

	}
}
