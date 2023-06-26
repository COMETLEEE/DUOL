#include "DUOLClient/BehaviorTreeNode/Action/Action_EliteEnemySoundUpdate.h"

DUOLClient::Action_EliteEnemySoundUpdate::Action_EliteEnemySoundUpdate(const std::string& name,
	const BT::NodeConfig& config) :
	SyncActionNode(name, config)
{
	_ai = getInput<DUOLClient::AI_EnemyBasic*>("AI").value();
}

BT::NodeStatus DUOLClient::Action_EliteEnemySoundUpdate::tick()
{

	return BT::NodeStatus::SUCCESS;
}

BT::PortsList DUOLClient::Action_EliteEnemySoundUpdate::providedPorts()
{
	BT::PortsList result = {
	BT::InputPort<DUOLClient::AI_EnemyBasic*>("AI")
	};

	return result;
}
