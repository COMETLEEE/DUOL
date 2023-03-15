#include "DUOLClient/BehaviorTreeNode/Condition/Condition_IsInAttackRange.h"

#include "DUOLCommon/Log/LogHelper.h"
#include "DUOLGameEngine/ECS/GameObject.h"

BT::NodeStatus DUOLClient::Condition_IsInAttackRange::tick()
{
	if (!_gameObject)
	{
		_gameObject = getInput<DUOLGameEngine::GameObject*>("GameObject").value();

		_targetTransform = getInput<DUOLGameEngine::Transform*>("TargetTransform").value();

		_range = getInput<float>("Range").value();

	}
	if(!_targetTransform)
		return BT::NodeStatus::FAILURE;
	auto tr = _gameObject->GetTransform();

	if (DUOLMath::Vector3::Distance(_targetTransform->GetWorldPosition(), tr->GetWorldPosition()) > _range) // 거리 밖이라면..
		return BT::NodeStatus::FAILURE;
	else
		return BT::NodeStatus::SUCCESS;
}

BT::PortsList DUOLClient::Condition_IsInAttackRange::providedPorts()
{
	BT::PortsList result = {
	BT::InputPort<DUOLGameEngine::GameObject*>("GameObject"),
	BT::InputPort<DUOLGameEngine::Transform*>("TargetTransform"),
	BT::InputPort<float>("Range")
	};

	return result;
}
