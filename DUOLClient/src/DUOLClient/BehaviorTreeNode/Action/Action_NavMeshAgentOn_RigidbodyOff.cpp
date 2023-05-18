#include "DUOLClient/BehaviorTreeNode/Action/Action_NavMeshAgentOn_RigidbodyOff.h"

#include "DUOLClient/ECS/Component/Enemy/AI_EnemyBasic.h"

namespace DUOLClient
{
	Action_NavMeshAgentOn_RigidbodyOff::Action_NavMeshAgentOn_RigidbodyOff(const std::string& name,
		const BT::NodeConfig& config) :
		SyncActionNode(name, config), _ai(nullptr)
	{
		_ai = getInput<AI_EnemyBasic*>("AI").value();
	}

	BT::NodeStatus Action_NavMeshAgentOn_RigidbodyOff::tick()
	{
		if (getInput<bool>("bool").value())
		{
			_ai->SetNavOnRigidbodyOff();
		}
		else
		{
			_ai->SetNavOffRigidbodyOn();
		}

		return BT::NodeStatus::SUCCESS;
	}

	BT::PortsList Action_NavMeshAgentOn_RigidbodyOff::providedPorts()
	{
		BT::PortsList result = {
		BT::InputPort<AI_EnemyBasic*>("AI"),
		BT::InputPort<bool>("bool")
		};

		return result;
	}
}
