/**

	@file      Action_SetGameObjectEnable.h
	@brief     게임오브젝트 SetEnable 노드.
	@details   ~
	@author    ShinSeongHyeon
	@date      23.03.2023
	@copyright © Cool Guy, 2023. All right reserved.

**/
#pragma once

#include "behaviortree_cpp/action_node.h"

namespace DUOLGameEngine
{
	class GameObject;

	class Action_SetGameObjectEnable : public BT::SyncActionNode
	{
	public:
		Action_SetGameObjectEnable(const std::string& name, const BT::NodeConfig& config);

		virtual ~Action_SetGameObjectEnable() override = default;

	private:
		GameObject* _gameObject;

		bool _isEnable;

	public:
		virtual BT::NodeStatus tick() override;

		static BT::PortsList providedPorts();
	};
}
