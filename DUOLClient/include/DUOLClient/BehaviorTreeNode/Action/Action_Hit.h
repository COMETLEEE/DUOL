#pragma once

#include "behaviortree_cpp/action_node.h"

namespace DUOLClient
{
	class AI_Enemy;

	class Action_Hit : public BT::SyncActionNode
	{
	public:
		Action_Hit(const std::string& name, const BT::NodeConfig& config) :
			SyncActionNode(name, config), _ai(nullptr) {}

		virtual ~Action_Hit() override = default;
	private:
		AI_Enemy* _ai;

	public:
		virtual BT::NodeStatus tick() override;

		static BT::PortsList providedPorts();
	};
}
