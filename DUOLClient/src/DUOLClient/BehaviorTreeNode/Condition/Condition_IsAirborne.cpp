#include "DUOLClient/BehaviorTreeNode/Condition/Condition_IsAirborne.h"

#include "DUOLClient/ECS/Component/Enemy/AI_EnemyBasic.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"

namespace DUOLClient
{
	BT::NodeStatus Condition_IsAirborne::tick()
	{
		if (!_ai)
			_ai = getInput<AI_EnemyBasic*>("AI").value();


		if (_ai->GetIsAirborne())
			return BT::NodeStatus::SUCCESS;

		if (_ai->GetAnimator()->GetSpeed() == 0)
		{
			_ai->GetAnimator()->SetSpeed(1.0f);
			_ai->GetAnimator()->SetBool(TEXT("IsAirBorne"), false);
		}
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
