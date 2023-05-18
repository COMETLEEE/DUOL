#include "DUOLClient/BehaviorTreeNode/Condition/Condition_AnimPlayCheck.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"

DUOLClient::Condition_AnimPlayCheck::Condition_AnimPlayCheck(const std::string& name, const BT::NodeConfig& config) :
	ConditionNode(name, config)
{
	_gameObject = getInput<DUOLGameEngine::GameObject*>("GameObject").value();

	_animator = _gameObject->GetComponent<DUOLGameEngine::Animator>();

	_animName = getInput<std::string>("AnimName").value();
}

BT::NodeStatus DUOLClient::Condition_AnimPlayCheck::tick()
{
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
