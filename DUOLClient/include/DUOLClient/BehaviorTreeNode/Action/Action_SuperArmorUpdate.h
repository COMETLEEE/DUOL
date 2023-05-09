/**

	@file      Condition_IsLookTarget.h
	@brief     슈퍼아머 게이지를 올리고 활성화 시키는 액션
	@details   ~
	@author    SinSeongHyeon
	@date      08.05.2023
	@copyright © SinSeongHyeon, 2023. All right reserved.

**/

#pragma once

#include "behaviortree_cpp/action_node.h"

namespace DUOLClient
{
	class AI_EnemyBasic;

	class Action_SuperArmorUpdate : public BT::SyncActionNode
	{
	public:
		Action_SuperArmorUpdate(const std::string& name, const BT::NodeConfig& config) :
			BT::SyncActionNode(name, config), _ai(nullptr)
		{}

		virtual ~Action_SuperArmorUpdate() override = default;
	private:
		AI_EnemyBasic* _ai;

	public:
		virtual BT::NodeStatus tick() override;

		static BT::PortsList providedPorts();
	};
}
