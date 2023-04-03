#include "DUOLClient/ECS/Component/Enemy/AI_EnemyBase.h"

#include "DUOLClient/ECS/Component/Enemy/EnemyGroupController.h"
#include "DUOLClient/ECS/Component/Enemy/Enemy.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/BehaviortreeController.h"
#include "DUOLGameEngine/ECS/Component/NavMeshAgent.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"

DUOLClient::AI_EnemyBase::AI_EnemyBase(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
	MonoBehaviourBase(owner, name), _enemyGroupController(nullptr)
{
}

DUOLClient::AI_EnemyBase::~AI_EnemyBase()
{
}

void DUOLClient::AI_EnemyBase::SetAnimConditionReset()
{
	_enemy->_animator->AllParamReset();
}

bool DUOLClient::AI_EnemyBase::GetIsHitCheck()
{
	return _enemy->_isHit;
}

void DUOLClient::AI_EnemyBase::SetIsHit(bool isHit)
{
	_enemy->_isHit = isHit;
}

bool DUOLClient::AI_EnemyBase::GetIsGroupCheck()
{
	if (_enemyGroupController)
		return _enemyGroupController->GetIsGroupCheck();
	return false;
}

void DUOLClient::AI_EnemyBase::SetIsGroupCheck()
{
	if (_enemyGroupController)
		_enemyGroupController->SetIsGroupCheck();
}

void DUOLClient::AI_EnemyBase::SetGroupController(EnemyGroupController* enemyGroupController)
{
	_enemyGroupController = enemyGroupController;
}

bool DUOLClient::AI_EnemyBase::GetIsDie() const
{
	return _enemy->GetIsDie();
}

void DUOLClient::AI_EnemyBase::UseToken()
{
	if (_enemy->_isToken)
	{
		_enemy->_isToken = false;
		_enemyGroupController->RetureTokken();
		// 토큰을 사용하고 무리에 다시 반납하여야 한다.
	}

}

void DUOLClient::AI_EnemyBase::TakeToken()
{
	_enemy->_isToken = true;
}

DUOLGameEngine::GameObject* DUOLClient::AI_EnemyBase::GetTarget() const
{
	return _enemy->_target;
}

void DUOLClient::AI_EnemyBase::SetTarget(DUOLGameEngine::GameObject* target)
{
	_enemy->_target = target;

}

bool DUOLClient::AI_EnemyBase::GetIsToken() const
{
	return _enemy->_isToken;
}

DUOLClient::EnemyGroupController* DUOLClient::AI_EnemyBase::GetGroupController() const
{
	return _enemyGroupController;
}

DUOLGameEngine::NavMeshAgent* DUOLClient::AI_EnemyBase::GetNavMeshAgent() const
{
	return _enemy->_navMeshAgent;
}

DUOLGameEngine::Animator* DUOLClient::AI_EnemyBase::GetAnimator() const
{
	return _enemy->_animator;
}

DUOLGameEngine::BehaviortreeController* DUOLClient::AI_EnemyBase::GetBehaviorTreeController() const
{
	return _behaviortreeController;
}

void DUOLClient::AI_EnemyBase::OnAwake()
{
	GetGameObject()->SetTag(TEXT("Enemy"));

	if (_behaviortreeController == nullptr)
		_behaviortreeController = GetGameObject()->AddComponent<DUOLGameEngine::BehaviortreeController>();

	_enemy = GetGameObject()->AddComponent<Enemy>();
}
