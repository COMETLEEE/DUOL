#include "DUOLGameEngine/BehaviorTreeSampleNode/Action_SetComponentEnable.h"

#include "DUOLGameEngine/ECS/GameObject.h"

#include "DUOLGameEngine/Util/BehaviorTreeTypeConvert.h"

DUOLGameEngine::Action_SetComponentEnable::Action_SetComponentEnable(const std::string& name,
	const BT::NodeConfig& config) :
	SyncActionNode(name, config), _component(nullptr), _isEnable(false)
{
}


BT::NodeStatus DUOLGameEngine::Action_SetComponentEnable::tick()
{
	if (!_component)
	{
		_isEnable = getInput<bool>("IsEnable").value();
		_componentName = getInput<DUOLCommon::tstring>("ComponentName").value();
		_component = reinterpret_cast<BehaviourBase*>(getInput<DUOLGameEngine::GameObject*>("GameObject").value()->GetComponent(_componentName));
	}

	_component->SetIsEnabled(_isEnable);

	return BT::NodeStatus::SUCCESS;
}

BT::PortsList DUOLGameEngine::Action_SetComponentEnable::providedPorts()
{
	BT::PortsList result = {
	BT::InputPort<DUOLGameEngine::GameObject*>("GameObject"),
		BT::InputPort<DUOLCommon::tstring>("ComponentName"),
		BT::InputPort<bool>("IsEnable")
	};

	return result;
}
