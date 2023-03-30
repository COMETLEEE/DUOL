#include "DUOLGameEngine/Manager/PhysicsManager.h"

#include "DUOLPhysics/System/PhysicsSystem.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/BoxCollider.h"
#include "DUOLGameEngine/ECS/Component/CapsuleCollider.h"
#include "DUOLGameEngine/ECS/Component/ColliderBase.h"
#include "DUOLGameEngine/ECS/Component/MeshCollider.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"
#include "DUOLGameEngine/ECS/Component/SphereCollider.h"
#include "DUOLGameEngine/ECS/Object/PhysicsMaterial.h"
#include "DUOLGameEngine/Manager/GraphicsManager.h"
#include "DUOLGameEngine/Manager/ResourceManager.h"
#include "DUOLGameEngine/Manager/TimeManager.h"
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
		// DUOLPhysics::PhysicsSystemDesc physicsDesc {false, DUOLGameEngine::GraphicsManager::GetInstance()->GetGraphicsDevice() };
		DUOLPhysics::PhysicsSystemDesc physicsDesc {true, nullptr };

		_physicsSystem = std::make_shared<DUOLPhysics::PhysicsSystem>();

		_physicsSystem->Init(physicsDesc);

		DUOL_INFO(DUOL_FILE, "PhysicsManager Initialize Success !");
	}

	 void PhysicsManager::UnInitialize()
	{		_fixedUpdateEventHandlers.RemoveAllListeners();

		_physicsSystem->Release();
	}

	void PhysicsManager::InitializePhysicsCollider(DUOLGameEngine::ColliderBase* collider)
	{
		const DUOLMath::Vector3& scale = collider->GetTransform()->GetLocalScale();

		// 아아아 리플렉션이 있으면 좋겠다 .. 그냥 enum 쓸 까
		BoxCollider* isBox = dynamic_cast<BoxCollider*>(collider);

		CapsuleCollider* isCapsule = dynamic_cast<CapsuleCollider*>(collider);

		SphereCollider* isSphere = dynamic_cast<SphereCollider*>(collider);

		MeshCollider* isMesh = dynamic_cast<MeshCollider*>(collider);

		const DUOLCommon::tstring uuidStr = DUOLCommon::StringHelper::ToTString(collider->GetUUID());

		// 1. Shape Desc => 일단 Size 정도 ..?
		// TODO : Flag, Material 등의 지정도 필요하다 ..
		DUOLPhysics::PhysicsShapeDesc shapeDesc;

		// 생성될 때는 Default Material ..?
		shapeDesc._material = DUOLGameEngine::ResourceManager::GetInstance()->GetPhysicsMaterial(TEXT("Default"))->GetPhysicsMaterial();

		shapeDesc._flag = DUOLPhysics::ShapeFlag::COLLIDER | DUOLPhysics::ShapeFlag::SCENE_QUERY;
		
		// shape의 local pose를 바꾸기 위해서 구조적으로 exclusive ..!
		shapeDesc._isExclusive = true;

		bool isTrigger = collider->GetIsTrigger();

		// Box Collider
		if (isBox != nullptr)
		{
			// 2. 콜라이더에 설정되어 있는 대로 초기화해줍니다.
			const DUOLMath::Vector3& boxCenter = isBox->_center;

			const DUOLMath::Vector3& boxExtents = isBox->_size;

			shapeDesc._box._halfExtentX = scale.x * boxExtents.x / 2.f;
			shapeDesc._box._halfExtentY = scale.y * boxExtents.y / 2.f;
			shapeDesc._box._halfExtentZ = scale.z * boxExtents.z / 2.f;

			isBox->_physicsBox = _physicsSystem->CreateShape<DUOLPhysics::PhysicsBox>(uuidStr, shapeDesc);

			isBox->_physicsBox.lock()->SetLocalPose(Vector3(boxCenter.x * scale.x, boxCenter.y * scale.y, boxCenter.z * scale.z));

			isBox->_physicsActor = isBox->GetGameObject()->_physicsActor;

			isBox->_physicsShapeBase = isBox->_physicsBox;

			// Trigger 여부 적용
			isBox->SetIsTrigger(isTrigger);

			_physicsShapes.insert({ uuidStr, isBox->_physicsShapeBase});
		}
		// Capsule Collider
		else if (isCapsule != nullptr)
		{
			// 2. 콜라이더에 설정되어 있는 대로 초기화해줍니다.
			const DUOLMath::Vector3& capsuleCenter = isCapsule->GetCenter();

			shapeDesc._capsule._halfHeight = isCapsule->GetHeight() / 2.f;

			shapeDesc._capsule._radius = isCapsule->GetRadius();

			isCapsule->_physicsCapsule = _physicsSystem->CreateShape<DUOLPhysics::PhysicsCapsule>(uuidStr, shapeDesc);

			// Pose 및 Center
			isCapsule->SetCenter(capsuleCenter);

			isCapsule->_physicsActor = isCapsule->GetGameObject()->_physicsActor;

			isCapsule->_physicsShapeBase = isCapsule->_physicsCapsule;

			// Trigger 여부 적용
			isCapsule->SetIsTrigger(isTrigger);

			_physicsShapes.insert({ uuidStr, isCapsule->_physicsShapeBase });
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

			// Trigger 여부 적용
			isSphere->SetIsTrigger(isTrigger);

			_physicsShapes.insert({ uuidStr, isSphere->_physicsShapeBase });
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

			// Trigger 여부 적용
			isMesh->SetIsTrigger(isTrigger);

			_physicsShapes.insert({ uuidStr, isMesh->_physicsShapeBase });
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

			auto& children = transform->GetChildGameObjects();

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

		for (auto& [key, value] : _physicsShapes)
			_physicsSystem->DestroyShape(key);

		_physicsStaticActors.clear();

		_physicsDynamicActors.clear();

		_physicsShapes.clear();

		// TODO : 이벤트 핸들러 개체가 남아있는 것 같아서 .. 다시 씬을 만들자 ..!
		if (!_physicsScene.expired())
		{
			_physicsSystem->DestroyScene(TEXT("DUOL_PHYSICS"));
		}

		// 1. Create physics scenes.
		DUOLPhysics::PhysicsSceneDesc physicsSceneDesc{ DUOLMath::Vector3(0.f, -9.8f, 0.f) };

		_physicsScene = _physicsSystem->CreateScene(TEXT("DUOL_PHYSICS"), physicsSceneDesc);

		// 1-2. Physics scene debug infomation.
		_physicsScene.lock()->SetRenderBufferOption(DUOLPhysics::RenderBufferOption::SCALE, 1.f);
		_physicsScene.lock()->SetRenderBufferOption(DUOLPhysics::RenderBufferOption::COLLISION_SHAPES, 2.f);
		_physicsScene.lock()->SetRenderBufferOption(DUOLPhysics::RenderBufferOption::COLLISION_SHAPES, 2.f);
		_physicsScene.lock()->SetRenderBufferOption(DUOLPhysics::RenderBufferOption::BODY_LINEAR_VELOCITY, 1.f);
		_physicsScene.lock()->SetRenderBufferOption(DUOLPhysics::RenderBufferOption::BODY_MASS_AXES, 1.f);

		// 2. sync with current game scene.
		for (auto& rootObject : rootObjectsInScene)
			InitializePhysicsGameObject(rootObject.get(), true);
	}

	void DUOLGameEngine::PhysicsManager::AttachPhysicsDynamicActor(DUOLGameEngine::GameObject* gameObject, DUOLGameEngine::Rigidbody* rigidbody)
	{
		// Shape 탈착
		const std::vector<ColliderBase*> hasCols
			= gameObject->GetComponents<DUOLGameEngine::ColliderBase>();

		// 먼저 때줘야 의도대로 작동한다.
		for (auto col : hasCols)
		{
			if (!col->_physicsActor.expired() && col->GetIsEnabled())
				col->_physicsActor.lock()->DetachShape(col->_physicsShapeBase);
		}

		const DUOLCommon::tstring uuidStr = DUOLCommon::StringHelper::ToTString(gameObject->GetUUID());

		// 1. Actor Desc는 아직 별로 없다.
		DUOLPhysics::PhysicsActorDesc actorDesc;

		actorDesc._transform = Matrix::Identity;

		// 만약 스태틱 액터로 만들어져 있다면 ..
		if (_physicsStaticActors.contains(uuidStr))
		{
			_physicsScene.lock()->DestroyStaticActor(uuidStr);

			_physicsStaticActors.erase(uuidStr);
		}
		// 이미 다이나믹 액터가 붙어 있다면 ..
		else if (_physicsDynamicActors.contains(uuidStr))
		{
			// TODO : 이것은 Add 차원에서 막아야 하는 문제이다 ..! (리지드 바디가 한 개만 달릴 수 있도록)

			return;
		}
		
		if (rigidbody != nullptr)
		{
			// dynamic actor
			const std::weak_ptr<DUOLPhysics::PhysicsDynamicActor> dActor =
				_physicsScene.lock()->CreateDynamicActor(uuidStr, actorDesc);

			gameObject->_physicsActor = dActor;

			// Event 등의 조작에 사용될 user data를 게임 오브젝트의 주소로 세팅
			dActor.lock()->SetUserData(gameObject);

			_physicsDynamicActors.insert({ uuidStr, { gameObject->_transform, dActor } });

			// caching actor in rigidbody and initialize.
			rigidbody->OnInitializeDynamicActor(dActor);

			for (auto& col : hasCols)
			{
				// Rigidbody 콜라이더들에게 등록
				col->SetAttachedRigidbody(rigidbody);

				// 콜라이더에게 액터 알려주기
				col->_physicsActor = dActor;

				// Enable 상태이면 Attach
				if (col->GetIsEnabled())
					col->_physicsActor.lock()->AttachShape(col->_physicsShapeBase);
			}
		}
	}

	void PhysicsManager::AttachPhysicsStaticActor(DUOLGameEngine::GameObject* gameObject)
	{
		const DUOLCommon::tstring uuidStr = DUOLCommon::StringHelper::ToTString(gameObject->GetUUID());

		// Shape 탈착
		const std::vector<ColliderBase*> hasCols
			= gameObject->GetComponents<DUOLGameEngine::ColliderBase>();

		// 먼저 때줘야 의도대로 작동한다.
		for (auto col : hasCols)
		{
			if (!col->_physicsActor.expired() && col->GetIsEnabled())
				col->_physicsActor.lock()->DetachShape(col->_physicsShapeBase);
		}

		// 1. Actor Desc는 아직 별로 없다.
		DUOLPhysics::PhysicsActorDesc actorDesc;

		actorDesc._transform = Matrix::Identity;

		// 만약 스태틱 액터로 만들어져 있다면 ..
		if (_physicsStaticActors.contains(uuidStr))
		{
			return;
		}
		// 이미 다이나믹 액터가 붙어 있다면 ..
		else if (_physicsDynamicActors.contains(uuidStr))
		{
			_physicsScene.lock()->DestroyDynamicActor(uuidStr);

			_physicsDynamicActors.erase(uuidStr);

			return;
		}

		const std::weak_ptr<DUOLPhysics::PhysicsStaticActor> sActor =
			_physicsScene.lock()->CreateStaticActor(uuidStr, actorDesc);

		gameObject->_physicsActor = sActor;

		// Event 등의 조작에 사용될 user data를 게임 오브젝트의 주소로 세팅
		sActor.lock()->SetUserData(gameObject);

		_physicsStaticActors.insert({ uuidStr, { gameObject->_transform, sActor } });

		for (auto& col : hasCols)
		{
			// 콜라이더에게 액터 알려주기
			col->_physicsActor = sActor;

			// Enable 상태이면 Attach
			if (col->GetIsEnabled())
				sActor.lock()->AttachShape(col->_physicsShapeBase);
		}
	}

	void PhysicsManager::AttachPhysicsCollider(DUOLGameEngine::GameObject* gameObject, DUOLGameEngine::ColliderBase* collider)
	{
		const DUOLCommon::tstring uuidStr = DUOLCommon::StringHelper::ToTString(gameObject->GetUUID());

		// 처음으로 물리 오브젝트가 되는 경우입니다.
		if (!_physicsStaticActors.contains(uuidStr) && !_physicsDynamicActors.contains(uuidStr))
		{
			DUOLPhysics::PhysicsActorDesc actorDesc;

			actorDesc._transform = Matrix::Identity;

			// static actor
			const std::weak_ptr<DUOLPhysics::PhysicsStaticActor> sActor =
				_physicsScene.lock()->CreateStaticActor(uuidStr, actorDesc);

			gameObject->_physicsActor = sActor;

			// Event 등의 조작에 사용될 user data를 게임 오브젝트의 주소로 세팅
			sActor.lock()->SetUserData(gameObject);

			_physicsStaticActors.insert({ uuidStr, { gameObject->_transform, sActor } });
		}

		DUOLGameEngine::Rigidbody* rigid = gameObject->GetComponent<DUOLGameEngine::Rigidbody>();

		if (rigid != nullptr)
			collider->SetAttachedRigidbody(rigid);

		InitializePhysicsCollider(collider);
	}

	void PhysicsManager::DetachPhysicsActor(DUOLGameEngine::GameObject* gameObject)
	{
		const DUOLCommon::tstring uuidStr = DUOLCommon::StringHelper::ToTString(gameObject->GetUUID());

		// Shape 탈착
		const std::vector<ColliderBase*> hasCols
			= gameObject->GetComponents<DUOLGameEngine::ColliderBase>();

		// 먼저 때줘야 의도대로 작동한다.
		for (auto col : hasCols)
		{
			if (!col->_physicsActor.expired() && col->GetIsEnabled())
				col->_physicsActor.lock()->DetachShape(col->_physicsShapeBase);
		}

		if (_physicsDynamicActors.contains(uuidStr))
		{
			_physicsScene.lock()->DestroyDynamicActor(uuidStr);

			_physicsDynamicActors.erase(uuidStr);

			// TODO : 스태틱 액터 만들어주기 ?
			if (_physicsInterpolateDatas.contains(uuidStr))
				_physicsInterpolateDatas.erase(uuidStr);
		}
		else if (_physicsStaticActors.contains(uuidStr))
		{
			_physicsScene.lock()->DestroyStaticActor(uuidStr);

			_physicsStaticActors.erase(uuidStr);
		}

		// nullptr
		gameObject->_physicsActor = std::weak_ptr<DUOLPhysics::PhysicsActorBase>();
	}

	void PhysicsManager::DetachPhysicsCollider(DUOLGameEngine::GameObject* gameObject,
		DUOLGameEngine::ColliderBase* collider)
	{
		// 일단 때줍니다. TODO : OnDisable 에서 이미 때어져 있습니다 ..!
		/*if (!gameObject->_physicsActor.expired())
		{
			gameObject->_physicsActor.lock()->DetachShape(collider->_physicsShapeBase);
		}*/

		const DUOLCommon::tstring uuidStr = DUOLCommon::StringHelper::ToTString(collider->GetUUID());

		_physicsSystem->DestroyShape(uuidStr);

		if (_physicsShapes.contains(uuidStr))
			_physicsShapes.erase(uuidStr);
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

		std::vector<DUOLGameEngine::ColliderBase*> colliders = gameObject->GetComponents<DUOLGameEngine::ColliderBase>();

		for (auto collider :colliders)
		{
			const DUOLCommon::tstring uuidStr = DUOLCommon::StringHelper::ToTString(collider->GetUUID());

			if (_physicsShapes.contains(uuidStr))
			{
				_physicsSystem->DestroyShape(uuidStr);

				_physicsShapes.erase(uuidStr);
			}
		}

		if (recursively)
		{
			// 바로 아래의 자식 오브젝트들에 대해서 또한 실시하여줍니다.
			DUOLGameEngine::Transform* transform = gameObject->GetTransform();

			auto& children = transform->GetChildGameObjects();

			for (auto& child : children)
				UnInitializePhysicsGameObject(child);
		}
	}

	void PhysicsManager::Update(float deltaTime)
	{
		if (_physicsScene.expired())
			return;

		static float accumTime = 0.f;

		accumTime += deltaTime;

		if (accumTime > _fixedTimeStep)
		{
			const int physicsUpdateCount = static_cast<int>(accumTime / _fixedTimeStep);

			for (int i = 0; i < physicsUpdateCount; i++)
			{
				FixedUpdate();

				ApplyPhysicsTransformBeforeSimulate();

				_physicsScene.lock()->Simulate(_fixedTimeStep);

				ApplyPhysicsSimulateResult();
			}

			accumTime = accumTime - static_cast<float>(physicsUpdateCount) * _fixedTimeStep;
		}

		ApplyPhysicsInterpolate();
	}

	void PhysicsManager::ApplyPhysicsInterpolate()
	{
		for (auto [key, value] : _physicsInterpolateDatas)
		{
			auto transform = value.first;

			auto& interpolateData = value.second;

			float currentTime = DUOLGameEngine::TimeManager::GetInstance()->GetRealtimeSinceStartup();

			float prevTime = interpolateData.first._timePoint;

			float nextTime = interpolateData.second._timePoint;

			if (nextTime == prevTime)
				continue;

			float coefficient = (currentTime - prevTime) / (nextTime - prevTime) ;

			transform->SetPosition(DUOLMath::Vector3::Lerp(interpolateData.first._position,
				interpolateData.second._position, coefficient));

			// TODO : 일단 로테이션은 막자. 해당 프레임에 피직스 업데이트가 이루어지지 않으면 .. 문제가 생긴다
			/*transform->SetRotation(DUOLMath::Quaternion::Slerp(interpolateData.first._rotation,
				interpolateData.second._rotation, coefficient));*/
		}
	}

	void PhysicsManager::UpdateEditAndPauseMode(float deltaTime)
	{
		if (_physicsScene.expired())
			return;

		ApplyPhysicsTransformBeforeSimulate();

		_physicsScene.lock()->Simulate(0.000000000000001f);
	}

	void PhysicsManager::ApplyPhysicsTransformBeforeSimulate()
	{
		// Static Actor 들에 대해서 적용 (해줘야하나 ..?)
		for (auto& [key, value] : _physicsStaticActors)
		{
			DUOLGameEngine::Transform* transform = value.first;

			const std::shared_ptr<DUOLPhysics::PhysicsStaticActor> actor = value.second.lock();

			// 해당 트랜스폼의 게임 오브젝트는 소멸되었습니다.
			if (transform == nullptr)
			{
				// TODO : Actor 소멸
			}
			else
			{
				// TODO : GameObject => IsStatic 이면 Call X.

				// TODO : FixedUpdate에서 위치를 바꿨을 수도 있기 때문에 .. Get 함수로 받아온다.
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
			DUOLGameEngine::Transform* transform = value.first;

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
			DUOLGameEngine::Transform* transform = value.first;

			const std::shared_ptr<DUOLPhysics::PhysicsDynamicActor> actor = value.second.lock();

			if (transform == nullptr)
			{
				// TODO : 대체 무슨 일이 있었던거니 ..
			}
			else
			{
				const DUOLPhysics::PhysicsPose& globalPose = actor->GetGlobalPose();

				transform->SetPosition(globalPose._position, Space::World);

				transform->SetRotation(globalPose._quaternion, Space::World);

				// Interpolate Option checked.
				if (_physicsInterpolateDatas.contains(key))
				{
					std::pair<PhysicsInterpolateData, PhysicsInterpolateData>& interpolateDatas = _physicsInterpolateDatas.at(key).second;

					// prev physics tick data.
					interpolateDatas.first._position = interpolateDatas.second._position;

					interpolateDatas.first._rotation = interpolateDatas.second._rotation;

					interpolateDatas.first._timePoint = interpolateDatas.second._timePoint;

					// current physics tick data (예측샷)
					interpolateDatas.second._position = globalPose._position;

					interpolateDatas.second._rotation = globalPose._quaternion;

					interpolateDatas.second._timePoint = DUOLGameEngine::TimeManager::GetInstance()->GetRealtimeSinceStartup();
				}
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

	DUOLMath::Vector3 PhysicsManager::GetGravity()
	{
		if (!_physicsScene.expired())
			return _physicsScene.lock()->GetGravity();
		else
			return DUOLMath::Vector3::Zero;
	}

	bool PhysicsManager::Raycast(const DUOLMath::Vector3& start, const DUOLMath::Vector3& end,
	                             DUOLPhysics::RaycastHit& outRaycastHit)
	{
		DUOLMath::Vector3 dir = end - start;

		float maxDistance = dir.Length();

		dir.Normalize(dir);

		return _physicsScene.lock()->Raycast(start, dir, maxDistance, outRaycastHit);
	}

	bool PhysicsManager::Raycast(const DUOLMath::Vector3& start, const DUOLMath::Vector3& direction, float maxDistance,
		DUOLPhysics::RaycastHit& outRaycastHit)
	{
		return _physicsScene.lock()->Raycast(start, direction, maxDistance, outRaycastHit);
	}

	bool PhysicsManager::RaycastAll(const DUOLMath::Vector3& start, const DUOLMath::Vector3& direction,
		float maxDistance, std::vector<DUOLPhysics::RaycastHit>& outRaycastHit)
	{
		return _physicsScene.lock()->RaycastAll(start, direction, maxDistance, outRaycastHit);
	}

	bool PhysicsManager::Spherecast(const DUOLMath::Vector3& start, const DUOLMath::Vector3& direction, float radius,
	                                float maxDistance, DUOLPhysics::RaycastHit& outSpherecastHit)
	{
		return _physicsScene.lock()->Spherecast(start, radius, direction, maxDistance, outSpherecastHit);
	}

	bool PhysicsManager::SpherecastAll(const DUOLMath::Vector3& start, const DUOLMath::Vector3& direction, float radius,
		float maxDistance, std::vector<DUOLPhysics::RaycastHit>& outSpherecastHit)
	{
		return _physicsScene.lock()->SpherecastAll(start, radius, direction, maxDistance, outSpherecastHit);
	}

	bool PhysicsManager::Boxcast(const DUOLMath::Vector3& start, const DUOLMath::Vector3& direction,
		const DUOLMath::Vector3& halfExtents, const DUOLMath::Quaternion& rotation, float maxDistance,
		DUOLPhysics::RaycastHit& outBoxcastHit)
	{
		return _physicsScene.lock()->Boxcast(start, halfExtents, rotation, direction, maxDistance, outBoxcastHit);
	}

	bool PhysicsManager::BoxcastAll(const DUOLMath::Vector3& start, const DUOLMath::Vector3& direction,
		const DUOLMath::Vector3& halfExtents, const DUOLMath::Quaternion& rotation, float maxDistance,
		std::vector<DUOLPhysics::RaycastHit>& outBoxcastHit)
	{
		return _physicsScene.lock()->BoxcastAll(start, halfExtents, rotation, direction, maxDistance, outBoxcastHit);
	}

	bool PhysicsManager::CheckBox(const DUOLMath::Vector3& center, const DUOLMath::Vector3& halfExtents,
	                              const DUOLMath::Quaternion& rotation)
	{
		return _physicsScene.lock()->CheckBox(center, halfExtents, rotation);
	}

	bool PhysicsManager::CheckSphere(const DUOLMath::Vector3& center, float radius)
	{
		return _physicsScene.lock()->CheckSphere(center, radius);
	}

	bool PhysicsManager::OverlapBoxAll(const DUOLMath::Vector3& center, const DUOLMath::Vector3& halfExtents,
		const DUOLMath::Quaternion& rotation, std::vector<DUOLPhysics::RaycastHit>& outOverlapBox)
	{
		return _physicsScene.lock()->OverlapBoxAll(center, halfExtents, rotation, outOverlapBox);
	}

	bool PhysicsManager::OverlapSphereAll(const DUOLMath::Vector3& center, float radius,
		std::vector<DUOLPhysics::RaycastHit>& outOverlapSphere)
	{
		return _physicsScene.lock()->OverlapSphereAll(center, radius, outOverlapSphere);
	}
}