#include "DUOLClient/BehaviorTreeNode/Action/Action_BossEnemySoundUpdate.h"

DUOLClient::Action_BossEnemySoundUpdate::Action_BossEnemySoundUpdate(const std::string& name, const BT::NodeConfig& config) :
	SyncActionNode(name, config)
{
	_ai = getInput<DUOLClient::AI_EnemyBasic*>("AI").value();
}

BT::NodeStatus DUOLClient::Action_BossEnemySoundUpdate::tick()
{

	return BT::NodeStatus::SUCCESS;
}

BT::PortsList DUOLClient::Action_BossEnemySoundUpdate::providedPorts()
{
	BT::PortsList result = {
	BT::InputPort<DUOLClient::AI_EnemyBasic*>("AI")
	};

	return result;
}
