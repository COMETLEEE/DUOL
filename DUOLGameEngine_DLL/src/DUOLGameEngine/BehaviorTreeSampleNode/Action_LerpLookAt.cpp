#include "DUOLGameEngine/BehaviorTreeSampleNode/Action_LerpLookAt.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLMath/DUOLMath.h"


BT::NodeStatus DUOLGameEngine::Action_LerpLookAt::tick()
{
	if (!_gameObject)
	{
		_gameObject = getInput<GameObject*>("GameObject").value();
		_targetTransform = getInput<Transform*>("TargetTransform").value();
	}

	if (_targetTransform)
	{
		DUOLMath::Vector3 myPosition = _gameObject->GetTransform()->GetWorldPosition();
		DUOLMath::Vector3 targetPos = DUOLMath::Vector3(_targetTransform->GetWorldPosition().x, _gameObject->GetTransform()->GetWorldPosition().y, _targetTransform->GetWorldPosition().z);

		auto look = targetPos - myPosition;

		look.Normalize();

		look = DUOLMath::Vector3::Lerp(_gameObject->GetTransform()->GetLook(), look, 0.1f);

		_gameObject->GetTransform()->LookAt(myPosition + look);
	}


	return BT::NodeStatus::SUCCESS;
}

BT::PortsList DUOLGameEngine::Action_LerpLookAt::providedPorts()
{
	BT::PortsList result = {
BT::InputPort<GameObject*>("GameObject"),
BT::InputPort<Transform*>("TargetTransform")
	};

	return result;
}
