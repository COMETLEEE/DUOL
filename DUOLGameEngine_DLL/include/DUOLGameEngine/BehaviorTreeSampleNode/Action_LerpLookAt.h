#pragma once
#include "behaviortree_cpp/action_node.h"
namespace DUOLGameEngine
{
	class GameObject;

	class Transform;

	class Action_LerpLookAt : public BT::SyncActionNode
	{
	public:
		Action_LerpLookAt(const std::string& name, const BT::NodeConfig& config) :
			SyncActionNode(name, config)
		{}

		virtual ~Action_LerpLookAt() override = default;

		BT::NodeStatus tick() override;

		GameObject* _gameObject;

		Transform* _targetTransform;

		static BT::PortsList providedPorts();
	};
}

