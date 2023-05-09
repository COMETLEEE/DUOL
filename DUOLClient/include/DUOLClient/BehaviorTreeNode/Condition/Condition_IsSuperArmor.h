/**

	@file      Condition_IsLookTarget.h
	@brief     슈퍼아머가 활성화 하였는지 확인하는 컨디션.
	@details   ~
	@author    SinSeongHyeon
	@date      08.05.2023
	@copyright © SinSeongHyeon, 2023. All right reserved.

**/
#pragma once

#include <behaviortree_cpp/condition_node.h>

namespace DUOLClient
{
	class AI_EnemyBasic;

	class Condition_IsSuperArmor : public BT::ConditionNode
	{
	public:
		Condition_IsSuperArmor(const std::string& name, const BT::NodeConfig& config) :
			ConditionNode(name, config)
		{}

		virtual ~Condition_IsSuperArmor() override = default;

	private:
		AI_EnemyBasic* _ai;

	public:
		virtual BT::NodeStatus tick() override;

		static BT::PortsList providedPorts();
	};
}

