#include "DUOLGameEngine/Manager/PhysicsManager.h"

#include "DUOLPhysics/System/PhysicsSystem.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/BoxCollider.h"
#include "DUOLGameEngine/ECS/Component/CapsuleCollider.h"
#include "DUOLGameEngine/ECS/Component/ColliderBase.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"
#include "DUOLGameEngine/ECS/Component/SphereCollider.h"
#include "DUOLGameEngine/ECS/Component/MeshCollider.h"
#include "DUOLGameEngine/ECS/Object/PhysicsMaterial.h"
#include "DUOLGameEngine/Manager/GraphicsManager.h"
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
		DUOLPhysics::PhysicsSystemDesc physicsDesc {true };

		_physicsSystem = std::make_shared<DUOLPhysics::PhysicsSystem>();

		_physicsSystem->Init(physicsDesc);

		// 1. Create physics scenes.
		DUOLPhysics::PhysicsSceneDesc physicsSceneDesc{ DUOLMath::Vector3(0.f, -9.8f, 0.f)};

		_physicsScene = _physicsSystem->CreateScene(TEXT("DUOL_PHYSICS"), physicsSceneDesc);

		_physicsScene.lock()->SetRenderBufferOption(DUOLPhysics::RenderBufferOption::SCALE, 1.f);
		_physicsScene.lock()->SetRenderBufferOption(DUOLPhysics::RenderBufferOption::COLLISION_SHAPES, 2.f);
	}

	void PhysicsManager::UnInitialize()
	{
		_fixedUpdateEventHandlers.RemoveAllListeners();

		_physicsSystem->Release();
	}

	void PhysicsManager::InitializePhysicsCollider(DUOLGameEngine::ColliderBase* collider) const
	{
		// �ƾƾ� ���÷����� ������ ���ڴ� .. �׳� enum �� ��
		BoxCollider* isBox = dynamic_cast<BoxCollider*>(collider);

		CapsuleCollider* isCapsule = dynamic_cast<CapsuleCollider*>(collider);

		SphereCollider* isSphere = dynamic_cast<SphereCollider*>(collider);

		MeshCollider* isMesh = dynamic_cast<MeshCollider*>(collider);

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
			const DUOLMath::Vector3& boxCenter = isBox->_center;

			const DUOLMath::Vector3& boxExtents = isBox->_size;

			shapeDesc._box._halfExtentX = boxExtents.x / 2.f;
			shapeDesc._box._halfExtentY = boxExtents.y / 2.f;
			shapeDesc._box._halfExtentZ = boxExtents.z / 2.f;

			isBox->_physicsBox = _physicsSystem->CreateShape<DUOLPhysics::PhysicsBox>(uuidStr, shapeDesc);

			isBox->_physicsBox.lock()->SetLocalPose(boxCenter);

			isBox->_physicsActor = isBox->GetGameObject()->_physicsActor;

			isBox->_physicsShapeBase = isBox->_physicsBox;
		}
		// Capsule Collider
		else if (isCapsule != nullptr)
		{
			// 2. �ݶ��̴��� �����Ǿ� �ִ� ��� �ʱ�ȭ���ݴϴ�.
			const DUOLMath::Vector3& capsuleCenter = isCapsule->GetCenter();

			shapeDesc._capsule._halfHeight = isCapsule->GetHeight() / 2.f;

			shapeDesc._capsule._radius = isCapsule->GetRadius();

			isCapsule->_physicsCapsule = _physicsSystem->CreateShape<DUOLPhysics::PhysicsCapsule>(uuidStr, shapeDesc);

			DUOLPhysics::PhysicsPose pose;

			pose._position = capsuleCenter;

			// �̰� ������ ����Ʈ �����̶� �ٸ��� �����ε� ..? ���� �⺻ ������ ��������� ..
			pose._quaternion = DUOLMath::Quaternion::CreateFromYawPitchRoll(0.f, 0.f, DUOLMath::MathHelper::DegreeToRadian(90.f));

			isCapsule->_physicsCapsule.lock()->SetLocalPose(pose);

			isCapsule->_physicsActor = isCapsule->GetGameObject()->_physicsActor;

			isCapsule->_physicsShapeBase = isCapsule->_physicsCapsule;
		}
		// Sphere Collider
		else if (isSphere != nullptr)
		{
			const DUOLMath::Vector3& sphereCenter = isSphere->GetCenter();

			shapeDesc._sphere._radius = isSphere->GetRadius();

			isSphere->_physicsSphere = _physicsSystem->CreateShape<DUOLPhysics::PhysicsSphere>(uuidStr, shapeDesc);

			DUOLPhysics::PhysicsPose pose;

			pose._position = sphereCenter;

			pose._quaternion = DUOLMath::Quaternion::Identity;

			isSphere->_physicsSphere.lock()->SetLocalPose(pose);

			isSphere->_physicsActor = isSphere->GetGameObject()->_physicsActor;

			isSphere->_physicsShapeBase = isSphere->_physicsSphere;
		}
		// Mesh Collider
		else if (isMesh != nullptr)
		{
			const DUOLMath::Vector3& meshCenter = isMesh->GetCenter();

			shapeDesc._mesh._vertex._buffer = isMesh->GetVertexBuffer()._buffer;
			shapeDesc._mesh._vertex._count = isMesh->GetVertexBuffer()._count;
			shapeDesc._mesh._vertex._stride = isMesh->GetVertexBuffer()._stride;

			shapeDesc._mesh._index._buffer = isMesh->GetIndexBuffer()._buffer;
			shapeDesc._mesh._index._count = isMesh->GetIndexBuffer()._count;
			shapeDesc._mesh._index._stride = isMesh->GetIndexBuffer()._stride;

			isMesh->_physicsMesh = _physicsSystem->CreateShape<DUOLPhysics::PhysicsMesh>(uuidStr, shapeDesc);

			isMesh->_physicsMesh.lock()->SetLocalPose(meshCenter);

			isMesh->_physicsActor = isMesh->GetGameObject()->_physicsActor;

			isMesh->_physicsShapeBase = isMesh->_physicsMesh;
		}
	}

	void PhysicsManager::InitializePhysicsGameObject(DUOLGameEngine::GameObject* gameObject, bool recursively)
	{
		const std::vector<ColliderBase*> hasCols
			= gameObject->GetComponents<DUOLGameEngine::ColliderBase>();

		DUOLGameEngine::Rigidbody* hasRigid
			= gameObject->GetComponent<DUOLGameEngine::Rigidbody>();

		const DUOLCommon::tstring uuidStr = DUOLCommon::StringHelper::ToTString(gameObject->GetUUID());

		// 1. Actor Desc�� ���� ���� ����.
		DUOLPhysics::PhysicsActorDesc actorDesc;

		actorDesc._transform = Matrix::Identity;

		// ���� �ùķ��̼ǰ� ������ ���� ���� ������Ʈ�Դϴ�.
		if ((hasCols.size() == 0) && (hasRigid == nullptr))
		{
			// �ƹ��͵� ���� ������
		}
		// Static ���� �ùķ��̼� ���� ������Ʈ�Դϴ�.
		else if ((hasCols.size() != 0) && (hasRigid == nullptr))
		{
			const std::weak_ptr<DUOLPhysics::PhysicsStaticActor> sActor =
				_physicsScene.lock()->CreateStaticActor(uuidStr, actorDesc);

			gameObject->_physicsActor = sActor;

			// Event ���� ���ۿ� ���� user data�� ���� ������Ʈ�� �ּҷ� ����
			sActor.lock()->SetUserData(gameObject);

			_physicsStaticActors.insert({ uuidStr, { gameObject->_transform, sActor } });

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

			// Event ���� ���ۿ� ���� user data�� ���� ������Ʈ�� �ּҷ� ����
			dActor.lock()->SetUserData(gameObject);

			_physicsDynamicActors.insert({ uuidStr, { gameObject->_transform, dActor } });

			// caching actor in rigidbody and initialize.
			hasRigid->OnInitializeDynamicActor(dActor);

			// �ݶ��̴��� ���� ������ Shape ����
			for (auto& col : hasCols)
			{
				// Rigidbody �ݶ��̴��鿡�� ���
				col->SetAttachedRigidbody(hasRigid);

				InitializePhysicsCollider(col);
			}
		}

		if (recursively)
		{
			// �ٷ� �Ʒ��� �ڽ� ������Ʈ�鿡 ���ؼ� ���� �ǽ��Ͽ��ݴϴ�.
			DUOLGameEngine::Transform* transform = gameObject->GetTransform();

			auto&& children = transform->GetChildGameObjects();

			for (auto& child : children)
				InitializePhysicsGameObject(child);
		}
	}

	void PhysicsManager::InitializeCurrentGameScene(const std::list<std::shared_ptr<DUOLGameEngine::GameObject>>& rootObjectsInScene)
	{
		// 1. before PhysX scene clear.
		for (auto& [key, value] : _physicsStaticActors)
			_physicsScene.lock()->DestroyStaticActor(key);

		for (auto& [key, value] : _physicsDynamicActors)
			_physicsScene.lock()->DestroyDynamicActor(key);

		_physicsStaticActors.clear();

		_physicsDynamicActors.clear();

		// 2. sync with current game scene.
		for (auto& rootObject : rootObjectsInScene)
			InitializePhysicsGameObject(rootObject.get(), true);
	}

	void PhysicsManager::UnInitializePhysicsGameObject(DUOLGameEngine::GameObject* gameObject, bool recursively)
	{
		const DUOLCommon::tstring uuidStr = DUOLCommon::StringHelper::ToTString(gameObject->GetUUID());

		for (auto& [key, value] : _physicsStaticActors)
		{
			// Ű �߰��ϸ�
			if (key == uuidStr)
			{
				// �ı��մϴ�.
				_physicsScene.lock()->DestroyStaticActor(key);

				_physicsStaticActors.erase(key);

				return;
			}
		}

		for (auto& [key, value] : _physicsDynamicActors)
		{
			// Ű �߰��ϸ�
			if (key == uuidStr)
			{
				// �ı��մϴ�.
				_physicsScene.lock()->DestroyDynamicActor(key);

				_physicsDynamicActors.erase(key);

				return;
			}
		}

		if (recursively)
		{
			// �ٷ� �Ʒ��� �ڽ� ������Ʈ�鿡 ���ؼ� ���� �ǽ��Ͽ��ݴϴ�.
			DUOLGameEngine::Transform* transform = gameObject->GetTransform();

			auto&& children = transform->GetChildGameObjects();

			for (auto& child : children)
				UnInitializePhysicsGameObject(child);
		}
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
			}

			accumTime = accumTime - static_cast<float>(physicsUpdateCount) * _fixedTimeStep;
		}
	}

	void PhysicsManager::ApplyPhysicsTransformBeforeSimulate()
	{
		// Static Actor �鿡 ���ؼ� ����
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

				DUOLPhysics::PhysicsPose pose;

				// ���⿡ Shape�� Center���� �־��־�� �մϴ�..
				pose._position = worldPos;

				pose._quaternion = worldRot;

				actor->SetGlobalPose(pose);
			}
		}

		// Dynamic Actor �鿡 ���ؼ� ����
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

				// ���⿡ Shape�� Center���� �־��־�� �մϴ�..
				pose._position = worldPos;

				pose._quaternion = worldRot;

				actor->SetGlobalPose(pose);
			}
		}
	}

	void PhysicsManager::ApplyPhysicsSimulateResult()
	{
		// ���� �ùķ��̼��� Dynamic actor �鸸�� ���ؽ�Ʈ�� ������ ��ģ��.
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

	DUOLCommon::EventListenerID PhysicsManager::AddFixedUpdateEventHandler(std::function<void(float)> functor)
	{
		return _fixedUpdateEventHandlers += std::move(functor);
	}

	bool PhysicsManager::RemoveFixedUpdateEventHandler(DUOLCommon::EventListenerID id)
	{
		return _fixedUpdateEventHandlers -= id;
	}
}