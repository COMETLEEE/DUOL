#include "DUOLGameEngine/BehaviorTreeSampleNode/Action_LerpLookAt.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/Manager/TimeManager.h"
#include "DUOLMath/DUOLMath.h"


DUOLGameEngine::Action_LerpLookAt::Action_LerpLookAt(const std::string& name, const BT::NodeConfig& config) :
	SyncActionNode(name, config)
{
	_gameObject = getInput<GameObject*>("GameObject").value();
	_targetTransform = getInput<Transform*>("TargetTransform").value();
}

BT::NodeStatus DUOLGameEngine::Action_LerpLookAt::tick()
{
	if (_targetTransform)
	{
		DUOLMath::Vector3 myPosition = _gameObject->GetTransform()->GetWorldPosition();
		DUOLMath::Vector3 targetPos = DUOLMath::Vector3(_targetTransform->GetWorldPosition().x, _gameObject->GetTransform()->GetWorldPosition().y, _targetTransform->GetWorldPosition().z);

		auto look = targetPos - myPosition;

		look.Normalize();

		look = DUOLMath::Vector3::Lerp(_gameObject->GetTransform()->GetLook(), look, 10.0f * TimeManager::GetInstance()->GetDeltaTime());

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
