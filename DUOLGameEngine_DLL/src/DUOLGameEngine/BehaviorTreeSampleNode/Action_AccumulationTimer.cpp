#include "DUOLGameEngine/BehaviorTreeSampleNode/Action_AccumulationTimer.h"

#include "DUOLGameEngine/Manager/TimeManager.h"


BT::NodeStatus DUOLGameEngine::Action_AccumulationTimer::tick()
{
	if (!_isOnce)
	{
		_timer = getInput<float>("Time").value();
		_isOnce = true;
	}

	_timer -= TimeManager::GetInstance()->GetDeltaTime();

	if (_timer < 0)
	{
		_timer = getInput<float>("Time").value();
		return BT::NodeStatus::SUCCESS;
	}

	return BT::NodeStatus::FAILURE;
}

BT::PortsList DUOLGameEngine::Action_AccumulationTimer::providedPorts()
{
	BT::PortsList result = {
	BT::InputPort<float>("Time")
	};

	return result;
}
