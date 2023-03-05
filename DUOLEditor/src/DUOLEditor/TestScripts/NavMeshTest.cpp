#include "DUOLEditor/TestScripts/NavMeshTest.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"
#include "DUOLGameEngine/ECS/Component/CapsuleCollider.h"

#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"

#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"
#include "DUOLGameEngine/ECS/Component/NavMeshAgent.h"

using namespace rttr;

RTTR_REGISTRATION
{
	rttr::registration::class_<DUOLEditor::NavMeshTest>("NavMestTest")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	);
}

namespace DUOLEditor
{
	NavMeshTest::NavMeshTest(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		MonoBehaviourBase(owner, name)
	{

	}

	NavMeshTest::~NavMeshTest()
	{

	}

	void NavMeshTest::OnAwake()
	{
		MonoBehaviourBase::OnAwake();

		_transform = reinterpret_cast<DUOLGameEngine::Transform*>(GetGameObject()->GetComponent(TEXT("Transform")));

		_animator = reinterpret_cast<DUOLGameEngine::Animator*>(GetGameObject()->GetComponent(TEXT("Animator")));

		_rigidbody = reinterpret_cast<DUOLGameEngine::Rigidbody*>(GetGameObject()->GetComponent(TEXT("Rigidbody")));

		_capsuleCollider = reinterpret_cast<DUOLGameEngine::CapsuleCollider*>(GetGameObject()->GetComponent(TEXT("CapsuleCollider")));

		_navMeshAgent = reinterpret_cast<DUOLGameEngine::NavMeshAgent*>(GetGameObject()->GetComponent(TEXT("NavMeshAgent")));

		auto allGameObjects	= DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

		for (auto gameObject : allGameObjects)
		{
			if (gameObject->GetName() == TEXT("PlayerCharacter"))
			{
				_playerGameObject = gameObject;
			}
		}
	}

	void NavMeshTest::OnStart()
	{
		MonoBehaviourBase::OnStart();

		_rigidbody->SetIsFreezeRotation(true);

		_rigidbody->SetCenterOfMass(_capsuleCollider->GetCenter());
	}

	void NavMeshTest::OnUpdate(float deltaTime)
	{
		MonoBehaviourBase::OnUpdate(deltaTime);

		const DUOLMath::Vector3& playerPos = _playerGameObject->GetTransform()->GetWorldPosition();

		_navMeshAgent->SetDestination(playerPos);
	}
}