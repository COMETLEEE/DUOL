#include "DUOLClient/BehaviorTreeNode/Action/Action_UseTokenAttack.h"

#include "DUOLClient/ECS/Component/Enemy/AI_EnemyBase.h"
#include "DUOLCommon/Log/LogHelper.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"
#include "DUOLGameEngine/ECS/Component/NavMeshAgent.h"

BT::NodeStatus DUOLClient::Action_UseTokenAttack::onStart()
{
	if (!_gameObject)
	{
		_gameObject = getInput<DUOLGameEngine::GameObject*>("GameObject").value();

		_targetTransform = getInput<DUOLGameEngine::Transform*>("TargetTransform").value();

		_ai = getInput<AI_EnemyBase*>("AI").value();

		_animator = _ai->GetAnimator();

		_navMeshAgent = _ai->GetNavMeshAgent();
	}

	return BT::NodeStatus::RUNNING;
}

BT::NodeStatus DUOLClient::Action_UseTokenAttack::onRunning()
{
	DUOL_TRACE(DUOL_CONSOLE, " 공격 ");

	_ai->UseToken();

	// 공격 애니메이션이 끝나면 Success 반환.
	if (_animator->GetCurrentStateName() != TEXT("Attack"))
		return BT::NodeStatus::SUCCESS;
	else
		return BT::NodeStatus::RUNNING;
}

void DUOLClient::Action_UseTokenAttack::onHalted()
{
}

BT::PortsList DUOLClient::Action_UseTokenAttack::providedPorts()
{
	BT::PortsList result = {
	BT::InputPort<DUOLGameEngine::GameObject*>("GameObject"),
	BT::InputPort<DUOLGameEngine::Transform*>("TargetTransform"),
	BT::InputPort<AI_EnemyBase*>("AI")
	};
	return result;
}
