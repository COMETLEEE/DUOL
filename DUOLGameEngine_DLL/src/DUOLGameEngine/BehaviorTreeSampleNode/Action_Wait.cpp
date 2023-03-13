#include "DUOLGameEngine/BehaviorTreeSampleNode/Action_Wait.h"

#include "DUOLCommon/Log/LogHelper.h"
#include "DUOLGameEngine/Manager/TimeManager.h"

namespace DUOLGameEngine
{
	BT::NodeStatus Action_Wait::onStart()
	{
		auto port = getInput<float>("Time");

		if (!port)
			DUOL_CRITICAL("error reading port [target]:{0}", port.error());

		_time = port.value();

		return BT::NodeStatus::RUNNING;
	}

	BT::NodeStatus Action_Wait::onRunning()
	{
		_time -= TimeManager::GetInstance()->GetDeltaTime();

		if (0 < _time)
			return BT::NodeStatus::RUNNING;
		else
			return BT::NodeStatus::SUCCESS;
	}
}
