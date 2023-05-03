#include "DUOLClient/BehaviorTreeNode/Condition/Condition_IsGroupCheck.h"

#include "DUOLClient/ECS/Component/Enemy/AI_EnemyBasic.h"
#include "DUOLGameEngine/ECS/GameObject.h"

BT::NodeStatus DUOLClient::Condition_IsGroupCheck::tick()
{
	if (!_ai)
	{
		_ai = getInput<AI_EnemyBasic*>("AI").value();
	}

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
