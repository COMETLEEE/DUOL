#include "DUOLGameEngine/Manager/PhysicsManager.h"

#include "DUOLPhysics/System/PhysicsSystem.h"

namespace DUOLGameEngine
{
	PhysicsManager::PhysicsManager() :
		_fixedTimeStep(0.02f)
	{
		
	}

	PhysicsManager::~PhysicsManager()
	{
	}

	void PhysicsManager::Initialize()
	{
		// 0. Physics engine on.
		DUOLPhysics::PhysicsSystemDesc physicsDesc {false };

		_physicsSystem = std::make_shared<DUOLPhysics::PhysicsSystem>();

		_physicsSystem->Init(physicsDesc);

		// 1. Create physics scenes.
		DUOLPhysics::PhysicsSceneDesc physicsSceneDesc{ DUOLMath::Vector3(0.f, -9.8f, 0.f)};

		_physicsSystem->CreateScene(TEXT("DUOL_PHYSICS"), physicsSceneDesc);

		// 일단 테스트 코드 내일 작성합시다.
	}

	void PhysicsManager::UnInitialize()
	{
		_fixedUpdateEventHandlers.RemoveAllListeners();
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
				FixedUpdate();

				TriggerUpdate();

				CollisionUpdate();
			}

			accumTime = accumTime - static_cast<float>(physicsUpdateCount) * _fixedTimeStep;
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