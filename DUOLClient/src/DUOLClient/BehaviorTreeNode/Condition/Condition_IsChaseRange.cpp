#include "DUOLClient/BehaviorTreeNode/Condition/Condition_IsChaseRange.h"

#include "DUOLClient/ECS/Component/Enemy/AI_EnemyBasic.h"

DUOLClient::Condition_IsChaseRange::Condition_IsChaseRange(const std::string& name, const BT::NodeConfig& config) :
	ConditionNode(name, config)
{
	_ai = getInput<DUOLClient::AI_EnemyBasic*>("AI").value();

}

BT::NodeStatus DUOLClient::Condition_IsChaseRange::tick()
{
	// �ϴ� ���׷� ���� ���Ƶд�...! Todo :
	//if (_ai->GetIsChase())

	return BT::NodeStatus::SUCCESS;

	//return BT::NodeStatus::FAILURE;
}

BT::PortsList DUOLClient::Condition_IsChaseRange::providedPorts()
{
	BT::PortsList result = {
	BT::InputPort<DUOLClient::AI_EnemyBasic*>("AI"),
	};

	return result;
}
