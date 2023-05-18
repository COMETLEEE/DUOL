#pragma once

#include "behaviortree_cpp/action_node.h"
namespace DUOLGameEngine
{
	class GameObject;

	class Transform;

	class Action_LookAt : public BT::SyncActionNode
	{
	public:
		Action_LookAt(const std::string& name, const BT::NodeConfig& config);

		virtual ~Action_LookAt() override = default;

		BT::NodeStatus tick() override;

		GameObject* _gameObject;

		Transform* _targetTransform;

		static BT::PortsList providedPorts();
	};

}

