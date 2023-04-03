#include "DUOLClient/BehaviorTreeNode/Action/Action_Die.h"

#include "DUOLClient/ECS/Component/Enemy/AI_EnemyBase.h"


BT::NodeStatus DUOLClient::Action_Die::tick()
{
	if (!_ai)
		_ai = getInput<AI_EnemyBase*>("AI").value();

	if (_ai->GetIsDie())
		return BT::NodeStatus::SUCCESS;
	else
		return BT::NodeStatus::FAILURE;

}

BT::PortsList DUOLClient::Action_Die::providedPorts()
{
	BT::PortsList result = {
		BT::InputPort<AI_EnemyBase*>("AI")
	};

	return result;
}
