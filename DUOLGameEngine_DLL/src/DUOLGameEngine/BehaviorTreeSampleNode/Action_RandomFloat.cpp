#include "DUOLGameEngine/BehaviorTreeSampleNode/Action_RandomFloat.h"

#include "DUOLMath/DUOLMath.h"

DUOLGameEngine::Action_RandomFloat::Action_RandomFloat(const std::string& name, const BT::NodeConfig& config) :
	SyncActionNode(name, config),
	_minValue(0),
	_maxValue(0)
{
	_minValue = getInput<float>("MinValue").value();
	_maxValue = getInput<float>("MaxValue").value();
}

DUOLGameEngine::Action_RandomFloat::~Action_RandomFloat()
{
}

BT::NodeStatus DUOLGameEngine::Action_RandomFloat::tick()
{
	float result = DUOLMath::MathHelper::RandF(_minValue, _maxValue);

	setOutput("OutputValue", result);

	return BT::NodeStatus::SUCCESS;
}

BT::PortsList DUOLGameEngine::Action_RandomFloat::providedPorts()
{
	BT::PortsList result = {
		BT::InputPort<float>("MaxValue"),
		BT::InputPort<float>("MinValue"),
		BT::OutputPort<float>("OutputValue")
	};

	return result;
}
