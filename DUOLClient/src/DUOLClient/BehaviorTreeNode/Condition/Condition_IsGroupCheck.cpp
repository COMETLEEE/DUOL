#include "DUOLClient/BehaviorTreeNode/Condition/Condition_IsGroupCheck.h"

#include "DUOLClient/ECS/Component/Enemy/AI_EnemyBasic.h"
#include "DUOLGameEngine/ECS/GameObject.h"

DUOLClient::Condition_IsGroupCheck::Condition_IsGroupCheck(const std::string& name, const BT::NodeConfig& config) :
	ConditionNode(name, config), _ai(nullptr)
{
	_ai = getInput<AI_EnemyBasic*>("AI").value();
}

BT::NodeStatus DUOLClient::Condition_IsGroupCheck::tick()
{
	if (_ai->GetIsGroupCheck())
		return BT::NodeStatus::SUCCESS;
	else
		return BT::NodeStatus::FAILURE;
}

BT::PortsList DUOLClient::Condition_IsGroupCheck::providedPorts()
{
	BT::PortsList result = {
	BT::InputPort<AI_EnemyBasic*>("AI")
	};

	return result;
}
