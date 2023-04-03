#pragma once

#include "behaviortree_cpp/action_node.h"


namespace DUOLClient
{
	class AI_EnemyBase;

	class Action_Die : public BT::SyncActionNode
	{
	public:
		Action_Die(const std::string& name, const BT::NodeConfig& config) :
			SyncActionNode(name, config), _ai(nullptr) {}

		virtual ~Action_Die() override = default;
	private:
		AI_EnemyBase* _ai;

	public:
		virtual BT::NodeStatus tick() override;

		static BT::PortsList providedPorts();
	};

}
