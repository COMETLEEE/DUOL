#include "DUOLGameEngine/BehaviorTreeSampleNode/Condition_Random.h"

#include "DUOLMath/DUOLMath.h"

BT::NodeStatus DUOLGameEngine::Condition_Random::tick()
{
	if (_randomRatio >= DUOLMath::MathHelper::RandF(0, 1.0f))
		return BT::NodeStatus::SUCCESS;
	else
		return BT::NodeStatus::FAILURE;
}

BT::PortsList DUOLGameEngine::Condition_Random::providedPorts()
{
	BT::PortsList result = {
		BT::InputPort<float>("RandomRatio")
	};

	return result;
}
