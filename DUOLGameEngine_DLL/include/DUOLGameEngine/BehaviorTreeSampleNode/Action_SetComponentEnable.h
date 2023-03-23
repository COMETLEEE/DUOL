/**

	@file      Action_SetComponentEnable.h
	@brief     컴포넌트를 키고 끄는 노드
	@details   ~
	@author    ShinSeongHyeon
	@date      23.03.2023
	@copyright © Cool Guy, 2023. All right reserved.

**/
#pragma once

#include "behaviortree_cpp/action_node.h"
#include "DUOLCommon/StringHelper.h"

namespace DUOLGameEngine
{
	class BehaviourBase;

	class Action_SetComponentEnable : public BT::SyncActionNode
	{
	public:
		Action_SetComponentEnable(const std::string& name, const BT::NodeConfig& config);

		virtual ~Action_SetComponentEnable() override = default;

	private:
		BehaviourBase* _component;

		DUOLCommon::tstring _componentName;

		bool _isEnable;

	public:
		virtual BT::NodeStatus tick() override;

		static BT::PortsList providedPorts();
	};

}
