#pragma once

#include <behaviortree_cpp/condition_node.h>

namespace DUOLClient
{
	class AI_EnemyBasic;

	class Condition_IsChaseRange : public BT::ConditionNode
	{
	public:
		Condition_IsChaseRange(const std::string& name, const BT::NodeConfig& config);

		virtual ~Condition_IsChaseRange() override = default;

	private:
		DUOLClient::AI_EnemyBasic* _ai;

	public:
		virtual BT::NodeStatus tick() override;

		static BT::PortsList providedPorts();
	};
}


