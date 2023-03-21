#include "DUOLClient/ECS/Component/AI_Enemy.h"
#include <rttr/registration>

#include "DUOLClient/BehaviorTreeNode/Condition/Condition_IsGroupCheck.h"
#include "DUOLClient/BehaviorTreeNode/Condition/Condition_IsLookTarget.h"
#include "DUOLClient/BehaviorTreeNode/Condition/Condition_IsInAttackRange.h"
#include "DUOLClient/ECS/Component/EnemyGroupController.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/BehaviortreeController.h"
#include "DUOLGameEngine/ECS/Component/NavMeshAgent.h"
#include "DUOLGameEngine/Manager/BehaviorTreeFactory.h"
#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"

RTTR_REGISTRATION
{
	rttr::registration::class_<DUOLClient::AI_Enemy>("AI_Enemy")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	);
}

DUOLClient::AI_Enemy::AI_Enemy(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
	MonoBehaviourBase(owner, name), _enemyGroupController(nullptr), _isLive(true), _navMeshAgent(nullptr)
{
}

DUOLClient::AI_Enemy::~AI_Enemy()
{
}

bool DUOLClient::AI_Enemy::GetIsGroupCheck()
{
	if (_enemyGroupController)
		return _enemyGroupController->GetIsGroupCheck();
	return false;
}

void DUOLClient::AI_Enemy::SetIsGroupCheck()
{
	if (_enemyGroupController)
		_enemyGroupController->SetIsGroupCheck();
}

void DUOLClient::AI_Enemy::SetGroupController(EnemyGroupController* enemyGroupController)
{
	_enemyGroupController = enemyGroupController;
}

void DUOLClient::AI_Enemy::UseToken()
{
	if (_isToken)
	{
		_isToken = false;
		_enemyGroupController->RetureTokken();
		// 토큰을 사용하고 무리에 다시 반납하여야 한다.
	}

}

DUOLGameEngine::NavMeshAgent* DUOLClient::AI_Enemy::GetNavMeshAgent()
{
	return _navMeshAgent;
}

DUOLGameEngine::Animator* DUOLClient::AI_Enemy::GetAnimator()
{
	return _animator;
}

void DUOLClient::AI_Enemy::OnAwake()
{
	_behaviortreeController = GetGameObject()->GetComponent<DUOLGameEngine::BehaviortreeController>();

	_navMeshAgent = GetGameObject()->GetComponent<DUOLGameEngine::NavMeshAgent>();

	_animator = GetGameObject()->GetComponent<DUOLGameEngine::Animator>();

	if (_behaviortreeController == nullptr)
		_behaviortreeController = GetGameObject()->AddComponent<DUOLGameEngine::BehaviortreeController>();

	auto treeFactory = DUOLGameEngine::BehaviorTreeFactory::GetInstance();

	auto tree = treeFactory->CreateTree("Enemy_MainTree");

	tree.rootBlackboard()->set<AI_Enemy*>("AI", this);

	tree.rootBlackboard()->set<DUOLGameEngine::Animator*>("Animator", _animator);

	/// ---------------------------- test Code ----------------------------------
	auto allGameObjects = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

	DUOLGameEngine::GameObject* target = nullptr;
	for (auto gameObject : allGameObjects)
	{
		if (gameObject->GetTag() == TEXT("Player"))
		{
			target = gameObject;
		}
	}
	if (target)
		tree.rootBlackboard()->set<DUOLGameEngine::Transform*>("TargetTransform", target->GetTransform());
	else
		tree.rootBlackboard()->set<DUOLGameEngine::Transform*>("TargetTransform", nullptr);

	/// ---------------------------- test Code ----------------------------------

	_behaviortreeController->Initialize(std::move(tree));
}

void DUOLClient::AI_Enemy::OnStart()
{
}
