#include "DUOLClient/BehaviorTreeNode/Condition/Condition_CurrentAnimStateCheck.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"
#include "DUOLGameEngine/Util/BehaviorTreeTypeConvert.h"

DUOLClient::Condition_CurrentAnimStateCheck::Condition_CurrentAnimStateCheck(const std::string& name,
	const BT::NodeConfig& config) :
	ConditionNode(name, config)
{
	_animator = getInput<DUOLGameEngine::Animator*>("Animator").value();

	_animName = getInput<DUOLCommon::tstring>("StateName").value();
}

BT::NodeStatus DUOLClient::Condition_CurrentAnimStateCheck::tick()
{
	if (_animator->GetCurrentStateName() == _animName)
		return BT::NodeStatus::SUCCESS;
	else
		return BT::NodeStatus::FAILURE;
}

BT::PortsList DUOLClient::Condition_CurrentAnimStateCheck::providedPorts()
{
	BT::PortsList result = {
	BT::InputPort<DUOLCommon::tstring>("StateName"),
	BT::InputPort<DUOLGameEngine::Animator*>("Animator")
	};

	return result;
}
