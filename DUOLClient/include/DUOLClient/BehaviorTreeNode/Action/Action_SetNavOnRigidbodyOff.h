#pragma once
#include "behaviortree_cpp/action_node.h"
namespace DUOLClient
{
	class AI_EnemyBasic;

	class Action_SetNavOnRigidbodyOff : public BT::SyncActionNode
	{
	public:
		Action_SetNavOnRigidbodyOff(const std::string& name, const BT::NodeConfig& config);

		virtual ~Action_SetNavOnRigidbodyOff() override = default;
	private:
		AI_EnemyBasic* _ai;

	public:
		virtual BT::NodeStatus tick() override;

		static BT::PortsList providedPorts();
	};
}

