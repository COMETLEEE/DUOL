#include "DUOLClient/BehaviorTreeNode/Condition/Condition_IsTargetDie.h"

#include "DUOLClient/ECS/Component/Enemy/AI_EnemyBasic.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLClient/ECS/Component/CharacterBase.h"
DUOLClient::Condition_IsTargetDie::Condition_IsTargetDie(const std::string& name, const BT::NodeConfig& config) :
	ConditionNode(name, config)
{
	_ai = getInput<AI_EnemyBasic*>("AI").value();

	targetCharacterBase = _ai->GetTarget()->GetComponent<CharacterBase>();
}

BT::NodeStatus DUOLClient::Condition_IsTargetDie::tick()
{
	if (targetCharacterBase->GetIsDie())
		return BT::NodeStatus::SUCCESS;

	return BT::NodeStatus::FAILURE;
}

BT::PortsList DUOLClient::Condition_IsTargetDie::providedPorts()
{
	BT::PortsList result = {
BT::InputPort<AI_EnemyBasic*>("AI"),
	};

	return result;
}
