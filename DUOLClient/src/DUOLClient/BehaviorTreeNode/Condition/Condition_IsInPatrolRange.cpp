#include "DUOLClient/BehaviorTreeNode/Condition/Condition_IsInPatrolRange.h"
#include "DUOLClient/ECS/Component/Enemy/AI_EnemyBasic.h"
#include "DUOLCommon/Log/LogHelper.h"
#include "DUOLGameEngine/ECS/GameObject.h"

DUOLClient::Condition_IsInPatrolRange::Condition_IsInPatrolRange(const std::string& name, const BT::NodeConfig& config) :
	ConditionNode(name, config), _ai(nullptr), _targetTransform(nullptr),
	_range(0), _transform(nullptr)
{
	_ai = getInput<AI_EnemyBasic*>("AI").value();

	_transform = _ai->GetParentTransform();

	_targetTransform = _ai->GetTargetTransform();

	_range = _ai->GetPatrolRange();
}

BT::NodeStatus DUOLClient::Condition_IsInPatrolRange::tick()
{
	if (DUOLMath::Vector3::Distance(_targetTransform->GetWorldPosition(), _transform->GetWorldPosition()) > _range) // �Ÿ� ���̶��..
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
