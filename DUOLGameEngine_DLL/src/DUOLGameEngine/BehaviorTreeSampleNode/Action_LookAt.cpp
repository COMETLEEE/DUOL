#include "DUOLGameEngine/BehaviorTreeSampleNode/Action_LookAt.h"

#include "DUOLGameEngine/ECS/GameObject.h"

BT::NodeStatus DUOLGameEngine::Action_LookAt::tick()
{
	if (!_gameObject)
	{
		_gameObject = getInput<GameObject*>("GameObject").value();
		_targetTransform = getInput<Transform*>("TargetTransform").value();
	}

	if (_targetTransform)
		_gameObject->GetTransform()->LookAt(
			DUOLMath::Vector3(_targetTransform->GetWorldPosition().x,
				_gameObject->GetTransform()->GetWorldPosition().y,
				_targetTransform->GetWorldPosition().z
			));

	return BT::NodeStatus::SUCCESS;
}

BT::PortsList DUOLGameEngine::Action_LookAt::providedPorts()
{
	BT::PortsList result = {
	BT::InputPort<GameObject*>("GameObject"),
	BT::InputPort<Transform*>("TargetTransform")
	};

	return result;
}
