#include "DUOLGameEngine/BehaviorTreeSampleNode/Action_SetGameObjectEnable.h"

#include "DUOLGameEngine/ECS/GameObject.h"

DUOLGameEngine::Action_SetGameObjectEnable::Action_SetGameObjectEnable(const std::string& name,
	const BT::NodeConfig& config) :
	SyncActionNode(name, config), _gameObject(nullptr),
	_isEnable(false)
{
}

BT::NodeStatus DUOLGameEngine::Action_SetGameObjectEnable::tick()
{
	if (!_gameObject)
	{
		_isEnable = getInput<bool>("IsEnable").value();

		_gameObject = getInput<DUOLGameEngine::GameObject*>("GameObject").value();
	}

	_gameObject->SetIsActive(_isEnable);

	return BT::NodeStatus::SUCCESS;
}

BT::PortsList DUOLGameEngine::Action_SetGameObjectEnable::providedPorts()
{
	BT::PortsList result = {
		BT::InputPort<DUOLGameEngine::GameObject*>("GameObject"),
		BT::InputPort<bool>("IsEnable")
	};

	return result;
}