#include "DUOLClient/BehaviorTreeNode/Action/Action_Die.h"


BT::NodeStatus DUOLClient::Action_Die::tick()
{
	if (!_ai)
		_ai = getInput<AI_Enemy*>("AI").value();


	return BT::NodeStatus::FAILURE;
}

BT::PortsList DUOLClient::Action_Die::providedPorts()
{

	BT::PortsList result = {
		BT::InputPort<AI_Enemy*>("AI")
	};

	return result;
}
