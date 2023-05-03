#include "DUOLClient/BehaviorTreeNode/Condition/Condition_IsInPatrolRange.h"
#include "DUOLClient/ECS/Component/Enemy/AI_EnemyBasic.h"
#include "DUOLCommon/Log/LogHelper.h"
#include "DUOLGameEngine/ECS/GameObject.h"

BT::NodeStatus DUOLClient::Condition_IsInPatrolRange::tick()
{
	if (!_ai)
	{
		_ai = getInput<AI_EnemyBasic*>("AI").value();

		_transform = _ai->GetParentTransform();

		_targetTransform = _ai->GetTargetTransform();

		_range = _ai->GetPatrolRange();
	}

	if (DUOLMath::Vector3::Distance(_targetTransform->GetWorldPosition(), _transform->GetWorldPosition()) > _range) // 거리 밖이라면..
		return BT::NodeStatus::FAILURE;
	else
		return BT::NodeStatus::SUCCESS;
}

BT::PortsList DUOLClient::Condition_IsInPatrolRange::providedPorts()
{
	BT::PortsList result = {
	BT::InputPort<AI_EnemyBasic*>("AI")
	};

	return result;
}
