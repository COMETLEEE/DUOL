#include "DUOLClient/BehaviorTreeNode/Condition/Condition_IsClose.h"

#include "DUOLGameEngine/ECS/GameObject.h"

DUOLClient::Condition_IsClose::Condition_IsClose(const std::string& name, const BT::NodeConfig& config) :
	ConditionNode(name, config), _gameObject(nullptr), _targetTransform(nullptr), _distance(0)
{
	_gameObject = getInput<DUOLGameEngine::GameObject*>("GameObject").value();

	_targetTransform = getInput<DUOLGameEngine::Transform*>("TargetTransform").value();

	_distance = getInput<float>("Distance").value() - 1.0f;
}

BT::NodeStatus DUOLClient::Condition_IsClose::tick()
{
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
