#include "DUOLGame/TestScripts/PhysicsEventTest.h"

#include "DUOLCommon/Log/LogHelper.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/BoxCollider.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"

namespace DUOLGame
{
	PhysicsEventTest::PhysicsEventTest(const std::weak_ptr<DUOLGameEngine::GameObject>& owner) :
		DUOLGameEngine::MonoBehaviourBase(owner, TEXT("PHYSICS_EVENT"))
		, _isDynamic(false)
	{
	}

	PhysicsEventTest::~PhysicsEventTest()
	{
	}

	void PhysicsEventTest::OnStart()
	{
		// ´ÙÀÌ³ª¹Í °´Ã¼
		if (GetGameObject()->GetComponent<DUOLGameEngine::Rigidbody>() != nullptr)
		{
			GetGameObject()->GetComponent<DUOLGameEngine::Rigidbody>()->SetUseGravity(false);

			// ´ÙÀÌ³ª¹Í °´Ã¼¿¡ ´ëÇØ¼­´Â Trigger¸¦ ¾²Áö ¸¿½Ã´Ù !
			_isDynamic = true;
		}
		// ½ºÅÂÆ½ °´Ã¼
		else
		{
			GetGameObject()->GetComponent<DUOLGameEngine::BoxCollider>()->SetIsTrigger(true);

			_isDynamic = false;
		}
	}

	void PhysicsEventTest::OnUpdate(float deltaTime)
	{
		if (_isDynamic)
		{

		}
	}

	void PhysicsEventTest::OnCollisionEnter(const std::shared_ptr<DUOLPhysics::Collision>& collision)
	{
		DUOL_INFO("Physics Event Test : OnCollisionEnter !! {0}", GetUUID());
	}

	void PhysicsEventTest::OnCollisionStay(const std::shared_ptr<DUOLPhysics::Collision>& collision)
	{
		DUOL_TRACE("Physics Event Test : OnCollisionStay !! {0}", GetUUID());
	}

	void PhysicsEventTest::OnCollisionExit(const std::shared_ptr<DUOLPhysics::Collision>& collision)
	{
		DUOL_WARN("Physics Event Test : OnCollisionExit !! {0}", GetUUID());
	}

	void PhysicsEventTest::OnTriggerEnter(const std::shared_ptr<DUOLPhysics::Trigger>& trigger)
	{
		DUOL_INFO("Physics Event Test : OnTriggerEnter !! {0}", GetUUID());
	}

	void PhysicsEventTest::OnTriggerStay(const std::shared_ptr<DUOLPhysics::Trigger>& trigger)
	{
		DUOL_TRACE("Physics Event Test : OnTriggerStay !! {0}", GetUUID());
	}

	void PhysicsEventTest::OnTriggerExit(const std::shared_ptr<DUOLPhysics::Trigger>& trigger)
	{
		DUOL_WARN("Physics Event Test : OnTriggerExit !! {0}", GetUUID());
	}
}