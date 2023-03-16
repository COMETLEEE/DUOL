#include "DUOLClient/BehaviorTreeNode/Action/Action_Hit.h"


BT::NodeStatus DUOLClient::Action_Hit::tick()
{
	if (!_ai)
		_ai = getInput<AI_Enemy*>("AI").value();


	return BT::NodeStatus::FAILURE;
}

BT::PortsList DUOLClient::Action_Hit::providedPorts()
{

	BT::PortsList result = {
		BT::InputPort<AI_Enemy*>("AI")
	};

	return result;
}
