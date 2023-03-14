/**

	@file      Action_Wait.h
	@brief     액션 노드 샘플입니다.
	@details   ~ 일정 시간 대기 후 Success를 반환합니다.
	@author    SinSeongHyeon
	@date      8.03.2023
	@copyright © SinSeongHyeon, 2023. All right reserved.

**/
#pragma once

#include "behaviortree_cpp/action_node.h"

namespace DUOLGameEngine
{
	class Action_Wait : public BT::StatefulActionNode
	{
	public:
		Action_Wait(const std::string& name, const BT::NodeConfig& config) :
			StatefulActionNode(name, config)
		{}

		virtual ~Action_Wait() override = default;

		virtual BT::NodeStatus onStart() override;

		virtual BT::NodeStatus onRunning() override;

		virtual void onHalted() override {}

		float _time;

		static BT::PortsList providedPorts()
		{
			return { BT::InputPort<float>("Time") };
		}

	};
}
