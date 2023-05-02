#include "DUOLClient/BehaviorTreeNode/Condition/Condition_IsChaseRange.h"

#include "DUOLClient/ECS/Component/Enemy/AI_EnemyBasic.h"

BT::NodeStatus DUOLClient::Condition_IsChaseRange::tick()
{
	if (!_ai)
	{
		_ai = getInput<DUOLClient::AI_EnemyBasic*>("AI").value();
	}

	if (_ai->GetIsChase())
		return BT::NodeStatus::SUCCESS;

	return BT::NodeStatus::FAILURE;
}

BT::PortsList DUOLClient::Condition_IsChaseRange::providedPorts()
{
	BT::PortsList result = {
	BT::InputPort<DUOLClient::AI_EnemyBasic*>("AI"),
	};

	return result;
}
