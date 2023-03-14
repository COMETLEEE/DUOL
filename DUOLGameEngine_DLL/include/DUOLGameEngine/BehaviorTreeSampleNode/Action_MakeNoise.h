/**

	@file      Action_Wait.h
	@brief     액션 노드 샘플입니다.
	@details   ~ 노이즈 재생 후 Success를 반환합니다.
	@author    SinSeongHyeon
	@date      8.03.2023
	@copyright © SinSeongHyeon, 2023. All right reserved.

**/
#pragma once

#include "behaviortree_cpp/action_node.h"

namespace DUOLGameEngine
{
	class Action_MakeNoise : public BT::SyncActionNode
	{
	public:
		Action_MakeNoise(const std::string& name, const BT::NodeConfig& config) :
			SyncActionNode(name, config)
		{}

		virtual ~Action_MakeNoise() override = default;

		BT::NodeStatus tick() override;

		static BT::PortsList providedPorts()
		{
			return {};
		}
	};
}
