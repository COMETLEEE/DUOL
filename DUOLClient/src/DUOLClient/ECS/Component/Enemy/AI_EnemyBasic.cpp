#include "DUOLClient/ECS/Component/Enemy/AI_EnemyBasic.h"

#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"

#include "DUOLClient/ECS/Component/Enemy/EnemyGroupController.h"
#include "DUOLClient/ECS/Component/Enemy/Enemy.h"
#include "DUOLClient/ECS/Component/Enemy/EnemyData.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/BehaviortreeController.h"
#include "DUOLGameEngine/ECS/Component/NavMeshAgent.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"
#include "DUOLGameEngine/Manager/BehaviorTreeFactory.h"

#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"

using namespace rttr;

RTTR_REGISTRATION
{
	rttr::registration::class_<DUOLClient::AI_EnemyBasic>("AI_Enemy_Near")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	);
}



DUOLClient::AI_EnemyBasic::AI_EnemyBasic(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
	MonoBehaviourBase(owner, name), _enemyGroupController(nullptr), _enemy(nullptr)
{
}

DUOLClient::AI_EnemyBasic::~AI_EnemyBasic()
{
}

void DUOLClient::AI_EnemyBasic::Initialize()
{
	if (!_enemy)
		_enemy = GetGameObject()->GetComponent<Enemy>();

	auto treeFactory = DUOLGameEngine::BehaviorTreeFactory::GetInstance();

	auto tree = treeFactory->CreateTree(_enemy->GetEnemyData()->_behaviorTreeName);

	auto rootBlackBoard = tree.rootBlackboard();

	rootBlackBoard->set<AI_EnemyBasic*>("AI", this);

	rootBlackBoard->set<DUOLGameEngine::Animator*>("Animator", GetAnimator());

	rootBlackBoard->set<float>("AttackRange", _enemy->GetAttackRange());

	rootBlackBoard->set<float>("PatrolOffset", _enemy->GetPatrolOffset());

	rootBlackBoard->set<float>("LookRange", _enemy->GetLookRange());

	auto allGameObjects = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

	for (auto gameObject : allGameObjects)
	{
		if (gameObject->GetTag() == TEXT("Player"))
		{
			SetTarget(gameObject);
		}
	}
	if (GetTarget())
		rootBlackBoard->set<DUOLGameEngine::Transform*>("TargetTransform", GetTarget()->GetTransform());
	else
		rootBlackBoard->set<DUOLGameEngine::Transform*>("TargetTransform", nullptr);

	GetBehaviorTreeController()->Initialize(std::move(tree));
}

void DUOLClient::AI_EnemyBasic::SetAnimConditionReset()
{
	_enemy->_animator->AllParamReset();
}

bool DUOLClient::AI_EnemyBasic::GetIsHitCheck()
{
	return _enemy->_isHit;
}

void DUOLClient::AI_EnemyBasic::SetIsHit(bool isHit)
{
	_enemy->_isHit = isHit;
}

bool DUOLClient::AI_EnemyBasic::GetIsGroupCheck()
{
	if (_enemyGroupController)
		return _enemyGroupController->GetIsGroupCheck();
	return false;
}

void DUOLClient::AI_EnemyBasic::SetIsGroupCheck()
{
	if (_enemyGroupController)
		_enemyGroupController->SetIsGroupCheck();
}

void DUOLClient::AI_EnemyBasic::SetGroupController(EnemyGroupController* enemyGroupController)
{
	_enemyGroupController = enemyGroupController;
}

bool DUOLClient::AI_EnemyBasic::GetIsDie() const
{
	return _enemy->GetIsDie();
}

void DUOLClient::AI_EnemyBasic::UseToken()
{
	if (_enemy->_isToken)
	{
		_enemy->_isToken = false;
		_enemyGroupController->RetureTokken();
		// 토큰을 사용하고 무리에 다시 반납하여야 한다.
	}

}

void DUOLClient::AI_EnemyBasic::TakeToken()
{
	_enemy->_isToken = true;
}

DUOLGameEngine::GameObject* DUOLClient::AI_EnemyBasic::GetTarget() const
{
	return _enemy->_target;
}

void DUOLClient::AI_EnemyBasic::SetTarget(DUOLGameEngine::GameObject* target)
{
	_enemy->_target = target;

}

bool DUOLClient::AI_EnemyBasic::GetIsToken() const
{
	return _enemy->_isToken;
}

DUOLClient::EnemyGroupController* DUOLClient::AI_EnemyBasic::GetGroupController() const
{
	return _enemyGroupController;
}

DUOLGameEngine::NavMeshAgent* DUOLClient::AI_EnemyBasic::GetNavMeshAgent() const
{
	return _enemy->_navMeshAgent;
}

DUOLGameEngine::Animator* DUOLClient::AI_EnemyBasic::GetAnimator() const
{
	return _enemy->_animator;
}

DUOLGameEngine::BehaviortreeController* DUOLClient::AI_EnemyBasic::GetBehaviorTreeController() const
{
	return _behaviortreeController;
}

void DUOLClient::AI_EnemyBasic::OnAwake()
{
	GetGameObject()->SetTag(TEXT("Enemy"));

	if (_behaviortreeController == nullptr)
		_behaviortreeController = GetGameObject()->AddComponent<DUOLGameEngine::BehaviortreeController>();



}
