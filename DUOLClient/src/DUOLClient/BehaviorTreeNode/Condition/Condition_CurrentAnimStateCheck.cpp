#include "DUOLClient/BehaviorTreeNode/Condition/Condition_CurrentAnimStateCheck.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"
#include "DUOLGameEngine/Util/BehaviorTreeTypeConvert.h"

BT::NodeStatus DUOLClient::Condition_CurrentAnimStateCheck::tick()
{
	if (!_gameObject)
	{
		_gameObject = getInput<DUOLGameEngine::GameObject*>("GameObject").value();

		_animator = _gameObject->GetComponent<DUOLGameEngine::Animator>();

		_animName = getInput<DUOLCommon::tstring>("StateName").value();
	}

	if (_animator->GetCurrentStateName() == _animName)
		return BT::NodeStatus::SUCCESS;
	else
		return BT::NodeStatus::FAILURE;
}

BT::PortsList DUOLClient::Condition_CurrentAnimStateCheck::providedPorts()
{
	BT::PortsList result = {
	BT::InputPort<DUOLGameEngine::GameObject*>("GameObject"),
	BT::InputPort<DUOLCommon::tstring>("StateName")
	};

	return result;
}
