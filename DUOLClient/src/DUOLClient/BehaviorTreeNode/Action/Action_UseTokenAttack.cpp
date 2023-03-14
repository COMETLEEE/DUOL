#include "DUOLClient/BehaviorTreeNode/Action/Action_UseTokenAttack.h"

BT::NodeStatus DUOLClient::Action_UseTokenAttack::onStart()
{
	return BT::NodeStatus::RUNNING;
}

BT::NodeStatus DUOLClient::Action_UseTokenAttack::onRunning()
{
	return BT::NodeStatus::SUCCESS;
}

void DUOLClient::Action_UseTokenAttack::onHalted()
{
}

BT::PortsList DUOLClient::Action_UseTokenAttack::providedPorts()
{
	BT::PortsList result = {
	BT::InputPort<DUOLGameEngine::GameObject*>("GameObject"),
	BT::InputPort<DUOLGameEngine::Transform*>("TargetTransform")
	};
	return result;
}
