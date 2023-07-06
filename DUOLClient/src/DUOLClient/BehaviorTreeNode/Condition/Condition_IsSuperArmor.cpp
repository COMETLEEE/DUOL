#include "DUOLClient/BehaviorTreeNode/Condition/Condition_IsSuperArmor.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"
#include "DUOLClient/ECS/Component/Enemy/AI_EnemyBasic.h"
#include "DUOLGameEngine/ECS/GameObject.h"

namespace DUOLClient
{
	Condition_IsSuperArmor::Condition_IsSuperArmor(const std::string& name, const BT::NodeConfig& config) :
		ConditionNode(name, config)
	{
		_ai = getInput<AI_EnemyBasic*>("AI").value();
	}

	BT::NodeStatus Condition_IsSuperArmor::tick()
	{
		if (_ai->GetIsSuperArmor())
		{
			_ai->GetAnimator()->SetBool(TEXT("IsHit_Front"), false);
			_ai->GetAnimator()->SetBool(TEXT("IsHit_Back"), false);

			return BT::NodeStatus::SUCCESS;
		}

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
