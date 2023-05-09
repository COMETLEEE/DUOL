#include "DUOLClient/BehaviorTreeNode/Action/Action_Die.h"

#include "DUOLClient/ECS/Component/Enemy/AI_EnemyBasic.h"
#include "DUOLClient/ECS/Component/Enemy/Enemy.h"
#include "DUOLClient/ECS/Component/Enemy/EnemyGroupController.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"


BT::NodeStatus DUOLClient::Action_Die::tick()
{
	if (!_ai)
		_ai = getInput<AI_EnemyBasic*>("AI").value();

	if (_ai->GetIsDie())
	{
		_ai->UseToken();
		_ai->GetGroupController()->EraseEnemy(_ai->GetGameObject()->GetUUID());

		_ai->GetAnimator()->SetBool(TEXT("IsDie"), true);

		_ai->ChangeMaterial(EnemyMaterial::DIE);

		return BT::NodeStatus::SUCCESS;
	}
	else
		return BT::NodeStatus::FAILURE;

}

BT::PortsList DUOLClient::Action_Die::providedPorts()
{
	BT::PortsList result = {
		BT::InputPort<AI_EnemyBasic*>("AI")
	};

	return result;
}