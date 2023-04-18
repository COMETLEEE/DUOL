#include "DUOLClient/BehaviorTreeNode/Condition/Condition_IsAirborne.h"

#include "DUOLClient/ECS/Component/Enemy/AI_EnemyBasic.h"

namespace DUOLClient
{
	BT::NodeStatus Condition_IsAirborne::tick()
	{
		if (!_ai)
			_ai = getInput<AI_EnemyBasic*>("AI").value();


		if (_ai->GetIsAirborne())
			return BT::NodeStatus::SUCCESS;
		else
			return BT::NodeStatus::FAILURE;

	}

	BT::PortsList Condition_IsAirborne::providedPorts()
	{
		BT::PortsList result = {
BT::InputPort<AI_EnemyBasic*>("AI"),
		};

		return result;
	}
}
