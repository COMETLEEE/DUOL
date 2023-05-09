#include "DUOLClient/BehaviorTreeNode/Condition/Condition_IsSuperArmor.h"

#include "DUOLClient/ECS/Component/Enemy/AI_EnemyBasic.h"
#include "DUOLGameEngine/ECS/GameObject.h"

namespace DUOLClient
{
	BT::NodeStatus Condition_IsSuperArmor::tick()
	{
		if (!_ai)
			_ai = getInput<AI_EnemyBasic*>("AI").value();

		if (_ai->GetIsSuperArmor())
			return BT::NodeStatus::SUCCESS;

		return BT::NodeStatus::FAILURE;
	}

	BT::PortsList Condition_IsSuperArmor::providedPorts()
	{
		BT::PortsList result = {
BT::InputPort<AI_EnemyBasic*>("AI")
		};

		return result;
	}
}
