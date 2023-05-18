/**

	@file      Action_NavMeshAgentOn_RigidbodyOff.h
	@brief     네비 메쉬를 키고 리지드바디를 끄는 액션. 반대의 기능도 동작하게 만들자..!
	@details   ~
	@author    SinSeongHyeon
	@date      18.04.2023
	@copyright © SinSeongHyeon, 2023. All right reserved.

**/
#pragma once

#include "behaviortree_cpp/action_node.h"

namespace DUOLClient
{
	class AI_EnemyBasic;

	class Action_NavMeshAgentOn_RigidbodyOff : public BT::SyncActionNode
	{
	public:
		Action_NavMeshAgentOn_RigidbodyOff(const std::string& name, const BT::NodeConfig& config);

		virtual ~Action_NavMeshAgentOn_RigidbodyOff() override = default;

	private:
		AI_EnemyBasic* _ai;

	public:
		virtual BT::NodeStatus tick() override;

		static BT::PortsList providedPorts();
	};

}
