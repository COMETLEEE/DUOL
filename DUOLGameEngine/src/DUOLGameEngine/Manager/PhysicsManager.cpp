#include "DUOLGameEngine/Manager/PhysicsManager.h"

#include "DUOLPhysics/System/PhysicsSystem.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/BoxCollider.h"
#include "DUOLGameEngine/ECS/Component/CapsuleCollider.h"
#include "DUOLGameEngine/ECS/Component/ColliderBase.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"
#include "DUOLGameEngine/ECS/Component/SphereCollider.h"
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
		// 아아아 리플렉션이 있으면 좋겠다 .. 그냥 enum 쓸 까
		BoxCollider* isBox = dynamic_cast<BoxCollider*>(collider);

		CapsuleCollider* isCapsule = dynamic_cast<CapsuleCollider*>(collider);

		SphereCollider* isSphere = dynamic_cast<SphereCollider*>(collider);

		const DUOLCommon::tstring uuidStr = DUOLCommon::StringHelper::ToTString(collider->GetUUID());

		// 1. Shape Desc => 일단 Size 정도 ..?
		// TODO : Flag, Material 등의 지정도 필요하다 ..
		DUOLPhysics::PhysicsShapeDesc shapeDesc;

		// 생성될 때는 Default Material ..?
		shapeDesc._material = DUOLGameEngine::ResourceManager::GetInstance()->GetPhysicsMaterial(TEXT("Default"))->GetPhysicsMaterial();

		shapeDesc._flag = DUOLPhysics::ShapeFlag::COLLIDER | DUOLPhysics::ShapeFlag::SCENE_QUERY;

		// shape의 local pose를 바꾸기 위해서 구조적으로 exclusive ..!
		shapeDesc._isExclusive = true;

		// Box Collider
		if (isBox != nullptr)
		{
			// 2. 콜라이더에 설정되어 있는 대로 초기화해줍니다.
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
			// 2. 콜라이더에 설정되어 있는 대로 초기화해줍니다.
			const DUOLMath::Vector3& capsuleCenter = isCapsule->GetCenter();

			shapeDesc._capsule._halfHeight = isCapsule->GetHeight() / 2.f;

			shapeDesc._capsule._radius = isCapsule->GetRadius();

			isCapsule->_physicsCapsule = _physicsSystem->CreateShape<DUOLPhysics::PhysicsCapsule>(uuidStr, shapeDesc);

			DUOLPhysics::PhysicsPose pose;

			pose._position = capsuleCenter;

			// 이건 피직스 디폴트 셋팅이랑 다른게 문제인듯 ..? 따라서 기본 각도로 돌려줘야함 ..
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
	}

	void PhysicsManager::InitializePhysicsGameObject(DUOLGameEngine::GameObject* gameObject, bool recursively)
	{
		const std::vector<ColliderBase*> hasCols
			= gameObject->GetComponents<DUOLGameEngine::ColliderBase>();

		DUOLGameEngine::Rigidbody* hasRigid
			= gameObject->GetComponent<DUOLGameEngine::Rigidbody>();

		const DUOLCommon::tstring uuidStr = DUOLCommon::StringHelper::ToTString(gameObject->GetUUID());

		// 1. Actor Desc는 아직 별로 없다.
		DUOLPhysics::PhysicsActorDesc actorDesc;

		actorDesc._transform = Matrix::Identity;

		// 물리 시뮬레이션과 관련이 없는 게임 오브젝트입니다.
		if ((hasCols.size() == 0) && (hasRigid == nullptr))
		{
			// 아무것도 하지 마세염
		}
		// Static 물리 시뮬레이션 게임 오브젝트입니다.
		else if ((hasCols.size() != 0) && (hasRigid == nullptr))
		{
			const std::weak_ptr<DUOLPhysics::PhysicsStaticActor> sActor =
				_physicsScene.lock()->CreateStaticActor(uuidStr, actorDesc);

			gameObject->_physicsActor = sActor;

			// Event 등의 조작에 사용될 user data를 게임 오브젝트의 주소로 세팅
			sActor.lock()->SetUserData(gameObject);

			_physicsStaticActors.insert({ uuidStr, { gameObject->_transform, sActor } });

			// 콜라이더에 따른 각각의 Shape 생성
			for (auto& col : hasCols)
				InitializePhysicsCollider(col);
		}
		// Dynamic 물리 시뮬레이션 게임 오브젝트입니다.
		else if (hasRigid != nullptr)
		{
			// dynamic actor
			const std::weak_ptr<DUOLPhysics::PhysicsDynamicActor> dActor =
				_physicsScene.lock()->CreateDynamicActor(uuidStr, actorDesc);

			gameObject->_physicsActor = dActor;

			// Event 등의 조작에 사용될 user data를 게임 오브젝트의 주소로 세팅
			dActor.lock()->SetUserData(gameObject);

			_physicsDynamicActors.insert({ uuidStr, { gameObject->_transform, dActor } });

			// caching actor in rigidbody and initialize.
			hasRigid->OnInitializeDynamicActor(dActor);

			// 콜라이더에 따른 각각의 Shape 생성
			for (auto& col : hasCols)
			{
				// Rigidbody 콜라이더들에게 등록
				col->SetAttachedRigidbody(hasRigid);

				InitializePhysicsCollider(col);
			}
		}

		if (recursively)
		{
			// 바로 아래의 자식 오브젝트들에 대해서 또한 실시하여줍니다.
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
			// 키 발견하면
			if (key == uuidStr)
			{
				// 파괴합니다.
				_physicsScene.lock()->DestroyStaticActor(key);

				_physicsStaticActors.erase(key);

				return;
			}
		}

		for (auto& [key, value] : _physicsDynamicActors)
		{
			// 키 발견하면
			if (key == uuidStr)
			{
				// 파괴합니다.
				_physicsScene.lock()->DestroyDynamicActor(key);

				_physicsDynamicActors.erase(key);

				return;
			}
		}

		if (recursively)
		{
			// 바로 아래의 자식 오브젝트들에 대해서 또한 실시하여줍니다.
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
		// Static Actor 들에 대해서 적용
		for (auto& [key, value] : _physicsStaticActors)
		{
			const std::shared_ptr<DUOLGameEngine::Transform> transform = value.first.lock();

			const std::shared_ptr<DUOLPhysics::PhysicsStaticActor> actor = value.second.lock();

			// 해당 트랜스폼의 게임 오브젝트는 소멸되었습니다.
			if (transform == nullptr)
			{
				// TODO : Actor 소멸
			}
			else
			{
				const DUOLMath::Vector3& worldPos = transform->GetWorldPosition();

				const DUOLMath::Quaternion& worldRot = transform->GetWorldRotation();

				DUOLPhysics::PhysicsPose pose;

				// 여기에 Shape의 Center까지 넣어주어야 합니다..
				pose._position = worldPos;

				pose._quaternion = worldRot;

				actor->SetGlobalPose(pose);
			}
		}

		// Dynamic Actor 들에 대해서 적용
		for (auto& [key, value] : _physicsDynamicActors)
		{
			const std::shared_ptr<DUOLGameEngine::Transform> transform = value.first.lock();

			const std::shared_ptr<DUOLPhysics::PhysicsDynamicActor> actor = value.second.lock();

			// 해당 트랜스폼의 게임 오브젝트는 소멸되었습니다.
			if (transform == nullptr)
			{
				// TODO : Actor 소멸
			}
			else
			{
				const DUOLMath::Vector3& worldPos = transform->GetWorldPosition();

				const DUOLMath::Quaternion& worldRot = transform->GetWorldRotation();

				DUOLPhysics::PhysicsPose pose;

				// 여기에 Shape의 Center까지 넣어주어야 합니다..
				pose._position = worldPos;

				pose._quaternion = worldRot;

				actor->SetGlobalPose(pose);
			}
		}
	}

	void PhysicsManager::ApplyPhysicsSimulateResult()
	{
		// 물리 시뮬레이션은 Dynamic actor 들만의 컨텍스트에 영향을 미친다.
		for (auto& [key, value] : _physicsDynamicActors)
		{
			const std::shared_ptr<DUOLGameEngine::Transform> transform = value.first.lock();

			const std::shared_ptr<DUOLPhysics::PhysicsDynamicActor> actor = value.second.lock();

			if (transform == nullptr)
			{
				// TODO : 대체 무슨 일이 있었던거니 ..
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