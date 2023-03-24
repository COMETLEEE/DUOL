#include "DUOLClient/ECS/Component/AI_Enemy.h"
#include <rttr/registration>

#include "DUOLClient/ECS/Component/EnemyGroupController.h"
#include "DUOLClient/ECS/Component/Enemy.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/BehaviortreeController.h"
#include "DUOLGameEngine/ECS/Component/NavMeshAgent.h"
#include "DUOLGameEngine/Manager/BehaviorTreeFactory.h"
#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"

#include "DUOLCommon/MetaDataType.h"

using namespace rttr;

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
	MonoBehaviourBase(owner, name), _enemyGroupController(nullptr),
	_navMeshAgent(nullptr)
{
}

DUOLClient::AI_Enemy::~AI_Enemy()
{
}



void DUOLClient::AI_Enemy::SetAnimConditionReset()
{
	_animator->SetFloat(TEXT("MoveSpeed"), 0);
	_animator->SetBool(TEXT("IsAttack"), false);
	_animator->SetBool(TEXT("IsWalkRight"), false);
	_animator->SetBool(TEXT("IsWalkLeft"), false);
	_animator->SetBool(TEXT("IsWalkBack"), false);
	_animator->SetBool(TEXT("IsJump_Backward"), false);
	_animator->SetBool(TEXT("IsHit_Front"), false);
	_animator->SetBool(TEXT("IsHit_Back"), false);
}

bool DUOLClient::AI_Enemy::GetIsHitCheck()
{
	return _enemy->_isHit;
}

void DUOLClient::AI_Enemy::SetIsHit(bool isHit)
{
	_enemy->_isHit = isHit;
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

bool DUOLClient::AI_Enemy::GetIsDie()
{
	return _enemy->GetIsDie();
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

DUOLClient::EnemyGroupController* DUOLClient::AI_Enemy::GetGroupController()
{
	return _enemyGroupController;
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

	_enemy = GetGameObject()->GetComponent<Enemy>();

	auto treeFactory = DUOLGameEngine::BehaviorTreeFactory::GetInstance();

	auto tree = treeFactory->CreateTree("Enemy_MainTree");

	tree.rootBlackboard()->set<AI_Enemy*>("AI", this);

	tree.rootBlackboard()->set<DUOLGameEngine::Animator*>("Animator", _animator);

	/// ---------------------------- test Code ----------------------------------
	auto allGameObjects = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();


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

void DUOLClient::AI_Enemy::OnUpdate(float deltaTime)
{

}

void DUOLClient::AI_Enemy::OnStart()
{
}
