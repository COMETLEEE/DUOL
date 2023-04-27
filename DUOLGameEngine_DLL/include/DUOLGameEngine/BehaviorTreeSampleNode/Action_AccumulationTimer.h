#pragma once
#include "behaviortree_cpp/action_node.h"

namespace DUOLGameEngine
{
	class Action_AccumulationTimer : public BT::SyncActionNode
	{
	public:
		Action_AccumulationTimer(const std::string& name, const BT::NodeConfig& config) :
			SyncActionNode(name, config), _isOnce(false)
		{}

		virtual ~Action_AccumulationTimer() override = default;

		BT::NodeStatus tick() override;

		float _timer;

		bool _isOnce;

		static BT::PortsList providedPorts();
	};
}
