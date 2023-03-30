/**

	@file      Action_RandomFloat.h
	@brief
	@details   ~
	@author    SinSeongHyeon
	@date      30.03.2023
	@copyright © SinSeongHyeon, 2023. All right reserved.

**/
#pragma once

#include "behaviortree_cpp/action_node.h"

namespace DUOLGameEngine
{
	class Action_RandomFloat : public BT::SyncActionNode
	{
	public:
		Action_RandomFloat(const std::string& name, const BT::NodeConfig& config);

		virtual ~Action_RandomFloat() override;

	private:
		float _minValue;
		float _maxValue;
		bool _isInit;
	public:
		virtual BT::NodeStatus tick() override;

		static BT::PortsList providedPorts();

	};
}

