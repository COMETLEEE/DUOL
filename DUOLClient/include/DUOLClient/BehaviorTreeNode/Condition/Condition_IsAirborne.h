/**

	@file      Condition_IsAirborne.h
	@brief
	@details   ~
	@author    SinSeongHyeon
	@date      18.04.2023
	@copyright © SinSeongHyeon, 2023. All right reserved.

**/
#pragma once

#include <behaviortree_cpp/condition_node.h>

namespace DUOLClient
{
	class AI_EnemyBasic;

	class Condition_IsAirborne : public BT::ConditionNode
	{
	public:
		Condition_IsAirborne(const std::string& name, const BT::NodeConfig& config);

		virtual ~Condition_IsAirborne() override = default;

	private:
		DUOLClient::AI_EnemyBasic* _ai;

	public:
		virtual BT::NodeStatus tick() override;

		static BT::PortsList providedPorts();
	};
}
