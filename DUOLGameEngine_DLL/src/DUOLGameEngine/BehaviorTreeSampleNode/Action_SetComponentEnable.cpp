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

		auto gameobject = getInput<DUOLGameEngine::GameObject*>("GameObject").value();

		_component = reinterpret_cast<BehaviourBase*>(gameobject->GetComponent(_componentName));

		std::function<ComponentBase* (DUOLGameEngine::Transform*)> searchParent =
			[&](DUOLGameEngine::Transform* parent)->ComponentBase*
		{
			const auto component = parent->GetGameObject()->GetComponent(_componentName);

			if (component)
				return component;
			else
			{
				if (parent->GetParent())
					return searchParent(parent->GetParent());
			}
			return nullptr;
		};

		if (!_component) // 만약 없다면... 부모를 탐색해보자...!
		{
			auto parent = gameobject->GetTransform()->GetParent();

			_component = reinterpret_cast<BehaviourBase*>(searchParent(parent));
		}
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
