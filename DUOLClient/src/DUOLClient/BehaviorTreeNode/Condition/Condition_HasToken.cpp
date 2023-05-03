#include "DUOLClient/BehaviorTreeNode/Condition/Condition_HasToken.h"

#include "DUOLClient/ECS/Component/Enemy/AI_EnemyBasic.h"
#include "DUOLGameEngine/ECS/GameObject.h"

BT::NodeStatus DUOLClient::Condition_HasToken::tick()
{
	if (!_ai)
	{
		_ai = getInput<AI_EnemyBasic*>("AI").value();
	}

	if (_ai->GetIsToken())
		return BT::NodeStatus::SUCCESS;
	else
		return BT::NodeStatus::FAILURE;
}

BT::PortsList DUOLClient::Condition_HasToken::providedPorts()
{
	BT::PortsList result = {
		BT::InputPort<AI_EnemyBasic*>("AI")
	};
	return result;
}
