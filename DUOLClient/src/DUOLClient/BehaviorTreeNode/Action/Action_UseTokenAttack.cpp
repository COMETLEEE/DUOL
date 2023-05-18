#include "DUOLClient/BehaviorTreeNode/Action/Action_UseTokenAttack.h"

#include "DUOLClient/ECS/Component/Enemy/AI_EnemyBasic.h"
#include "DUOLCommon/Log/LogHelper.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"
#include "DUOLGameEngine/ECS/Component/NavMeshAgent.h"

DUOLClient::Action_UseTokenAttack::Action_UseTokenAttack(const std::string& name, const BT::NodeConfig& config) :
	StatefulActionNode(name, config), _targetTransform(nullptr)
{
	_ai = getInput<AI_EnemyBasic*>("AI").value();

	_targetTransform = _ai->GetTargetTransform();

	_animator = _ai->GetAnimator();
}

BT::NodeStatus DUOLClient::Action_UseTokenAttack::onStart()
{
	return BT::NodeStatus::RUNNING;
}

BT::NodeStatus DUOLClient::Action_UseTokenAttack::onRunning()
{
	_ai->UseToken();

	// 공격 애니메이션이 끝나면 Success 반환.
	if (_animator->GetCurrentStateName() == TEXT("Idle"))
		return BT::NodeStatus::SUCCESS;
	else if (_animator->GetCurrentStateName() == TEXT("Attack"))
		return BT::NodeStatus::RUNNING;
	else
		return BT::NodeStatus::FAILURE;
}

void DUOLClient::Action_UseTokenAttack::onHalted()
{
}

BT::PortsList DUOLClient::Action_UseTokenAttack::providedPorts()
{
	BT::PortsList result = {
	BT::InputPort<AI_EnemyBasic*>("AI")
	};
	return result;
}
