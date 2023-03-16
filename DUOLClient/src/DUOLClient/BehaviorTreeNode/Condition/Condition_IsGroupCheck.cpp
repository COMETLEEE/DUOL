#include "DUOLClient/BehaviorTreeNode/Condition/Condition_IsGroupCheck.h"

#include "DUOLClient/ECS/Component/AI_Enemy.h"
#include "DUOLGameEngine/ECS/GameObject.h"

BT::NodeStatus DUOLClient::Condition_IsGroupCheck::tick()
{
	if (!_gameObject)
	{
		_gameObject = getInput<DUOLGameEngine::GameObject*>("GameObject").value();

		_ai = getInput<AI_Enemy*>("AI").value();
	}

	if (_ai->GetIsGroupCheck())
		return BT::NodeStatus::SUCCESS;
	else
		return BT::NodeStatus::FAILURE;
}

BT::PortsList DUOLClient::Condition_IsGroupCheck::providedPorts()
{
	BT::PortsList result = {
	BT::InputPort<DUOLGameEngine::GameObject*>("GameObject"),
	BT::InputPort<AI_Enemy*>("AI")
	};

	return result;

}
