#include "DUOLClient/BehaviorTreeNode/Action/Action_Die.h"

#include "DUOLClient/ECS/Component/Enemy/AI_EnemyBasic.h"
#include "DUOLClient/ECS/Component/Enemy/Enemy.h"
#include "DUOLClient/ECS/Component/Enemy/EnemyGroupController.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"


DUOLClient::Action_Die::Action_Die(const std::string& name, const BT::NodeConfig& config)
	:SyncActionNode(name, config)
{
	_ai = getInput<AI_EnemyBasic*>("AI").value();
}

BT::NodeStatus DUOLClient::Action_Die::tick()
{
	if (_ai->GetIsDie())
	{
		_ai->SetIsDie();
		return BT::NodeStatus::SUCCESS;
	}
	else
	{
		_isOnce = false;
		return BT::NodeStatus::FAILURE;
	}
}

BT::PortsList DUOLClient::Action_Die::providedPorts()
{
	BT::PortsList result = {
		BT::InputPort<AI_EnemyBasic*>("AI")
	};

	return result;
}