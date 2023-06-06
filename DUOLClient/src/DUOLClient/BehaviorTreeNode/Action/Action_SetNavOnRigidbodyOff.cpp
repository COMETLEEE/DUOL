#include "DUOLClient/BehaviorTreeNode/Action/Action_SetNavOnRigidbodyOff.h"

#include "DUOLClient/ECS/Component/Enemy/AI_EnemyBasic.h"

namespace DUOLClient
{
	DUOLClient::Action_SetNavOnRigidbodyOff::Action_SetNavOnRigidbodyOff(const std::string& name,
		const BT::NodeConfig& config) :
		SyncActionNode(name, config)
	{
		_ai = getInput<AI_EnemyBasic*>("AI").value();
	}

	BT::NodeStatus DUOLClient::Action_SetNavOnRigidbodyOff::tick()
	{
		_ai->SetNavOnRigidbodyOff();

		return BT::NodeStatus::SUCCESS;
	}

	BT::PortsList DUOLClient::Action_SetNavOnRigidbodyOff::providedPorts()
	{
		BT::PortsList result = {
			BT::InputPort<AI_EnemyBasic*>("AI")
		};

		return result;
	}
}
