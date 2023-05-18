#pragma once

#include "behaviortree_cpp/action_node.h"


namespace DUOLClient
{
	class AI_EnemyBasic;

	class Action_Die : public BT::SyncActionNode
	{
	public:
		Action_Die(const std::string& name, const BT::NodeConfig& config);

		virtual ~Action_Die() override = default;
	private:
		AI_EnemyBasic* _ai;

		bool _isOnce;
	public:
		virtual BT::NodeStatus tick() override;

		static BT::PortsList providedPorts();
	};

}
