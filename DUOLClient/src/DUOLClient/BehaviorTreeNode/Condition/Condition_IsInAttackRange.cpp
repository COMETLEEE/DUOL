#include "DUOLClient/BehaviorTreeNode/Condition/Condition_IsInAttackRange.h"

#include "DUOLClient/ECS/Component/Enemy/AI_EnemyBasic.h"
#include "DUOLCommon/Log/LogHelper.h"
#include "DUOLGameEngine/ECS/GameObject.h"

DUOLClient::Condition_IsInAttackRange::Condition_IsInAttackRange(const std::string& name, const BT::NodeConfig& config) :
	ConditionNode(name, config)
{
	_ai = getInput<AI_EnemyBasic*>("AI").value();

	_transform = _ai->GetParentTransform();

	_targetTransform = _ai->GetTargetTransform();

	_range = _ai->GetAttackRange();
}

BT::NodeStatus DUOLClient::Condition_IsInAttackRange::tick()
{
	if (DUOLMath::Vector3::Distance(_targetTransform->GetWorldPosition(), _transform->GetWorldPosition()) > _range) // 거리 밖이라면..
		return BT::NodeStatus::FAILURE;
	else
		return BT::NodeStatus::SUCCESS;
}

BT::PortsList DUOLClient::Condition_IsInAttackRange::providedPorts()
{
	BT::PortsList result = {
	BT::InputPort<AI_EnemyBasic*>("AI")
	};

	return result;
}
