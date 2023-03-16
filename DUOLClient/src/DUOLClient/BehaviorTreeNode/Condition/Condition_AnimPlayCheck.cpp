#include "DUOLClient/BehaviorTreeNode/Condition/Condition_AnimPlayCheck.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"

BT::NodeStatus DUOLClient::Condition_AnimPlayCheck::tick()
{
	if (!_gameObject)
	{
		_gameObject = getInput<DUOLGameEngine::GameObject*>("GameObject").value();

		_animator = _gameObject->GetComponent<DUOLGameEngine::Animator>();

		_animName = getInput<std::string>("AnimName").value();
	}

	return BT::NodeStatus::FAILURE;
}

BT::PortsList DUOLClient::Condition_AnimPlayCheck::providedPorts()
{
	BT::PortsList result = {
	BT::InputPort<DUOLGameEngine::GameObject*>("GameObject"),
	BT::InputPort<std::wstring>("AnimName")
	};

	return result;
}
