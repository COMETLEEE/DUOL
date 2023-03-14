#include "DUOLClient/BehaviorTreeNode/Action/Action_AroundPatrol.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/NavMeshAgent.h"

BT::NodeStatus DUOLClient::Action_AroundPatrol::onStart()
{
	if (!_gameObject)
	{
		_gameObject = getInput<DUOLGameEngine::GameObject*>("GameObject").value();

		_targetTransform = getInput<DUOLGameEngine::Transform*>("TargetTransform").value();

		_navMeshAgent = _gameObject->GetComponent<DUOLGameEngine::NavMeshAgent>();
	}


	return BT::NodeStatus::RUNNING;
}

BT::NodeStatus DUOLClient::Action_AroundPatrol::onRunning()
{

	return BT::NodeStatus::SUCCESS;
}

void DUOLClient::Action_AroundPatrol::onHalted()
{
}

BT::PortsList DUOLClient::Action_AroundPatrol::providedPorts()
{
	BT::PortsList result = {
		BT::InputPort<DUOLGameEngine::GameObject*>("GameObject"),
		BT::InputPort<DUOLGameEngine::Transform*>("TargetTransform")
	};

	return result;

}
