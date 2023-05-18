#include "DUOLClient/BehaviorTreeNode/Condition/Condition_HasToken.h"

#include "DUOLClient/ECS/Component/Enemy/AI_EnemyBasic.h"
#include "DUOLGameEngine/ECS/GameObject.h"

DUOLClient::Condition_HasToken::Condition_HasToken(const std::string& name, const BT::NodeConfig& config) :
	ConditionNode(name, config), _ai(nullptr)
{
	//	_ai = getInput<AI_EnemyBasic*>("AI").value();
}

BT::NodeStatus DUOLClient::Condition_HasToken::tick()
{
	//if (_ai->GetIsToken())
	//	return BT::NodeStatus::SUCCESS;
	//else

	return BT::NodeStatus::FAILURE;
}

BT::PortsList DUOLClient::Condition_HasToken::providedPorts()
{
	BT::PortsList result = {
		BT::InputPort<AI_EnemyBasic*>("AI")
	};
	return result;
}
