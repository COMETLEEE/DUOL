#include "DUOLClient/BehaviorTreeNode/Condition/Condition_IsClose.h"

#include "DUOLGameEngine/ECS/GameObject.h"

BT::NodeStatus DUOLClient::Condition_IsClose::tick()
{
	if (!_gameObject)
	{
		_gameObject = getInput<DUOLGameEngine::GameObject*>("GameObject").value();

		_targetTransform = getInput<DUOLGameEngine::Transform*>("TargetTransform").value();

		_distance = getInput<float>("Distance").value();
	}

	const float distance = DUOLMath::Vector3::Distance(_gameObject->GetTransform()->GetWorldPosition(), _targetTransform->GetWorldPosition());

	if (distance < _distance)
		return BT::NodeStatus::SUCCESS;
	else
		return BT::NodeStatus::FAILURE;
}

BT::PortsList DUOLClient::Condition_IsClose::providedPorts()
{
	BT::PortsList result = {
		BT::InputPort<DUOLGameEngine::GameObject*>("GameObject"),
		BT::InputPort<DUOLGameEngine::Transform*>("TargetTransform"),
		BT::InputPort<float>("Distance")
	};

	return result;
}
