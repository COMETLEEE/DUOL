/**

	@file      Condition_HasToken.h
	@brief     토큰을 가지고 있는지 확인하는 컨디션
	@details   ~
	@author    SinSeongHyeon
	@date      14.03.2023
	@copyright © SinSeongHyeon, 2023. All right reserved.

**/
#pragma once

#include <behaviortree_cpp/condition_node.h>

namespace DUOLGameEngine
{
	class GameObject;
}

namespace DUOLClient
{
	class AI_EnemyBasic;

	class Condition_HasToken : public BT::ConditionNode
	{
	public:
		Condition_HasToken(const std::string& name, const BT::NodeConfig& config);

		virtual ~Condition_HasToken() override = default;

	private:
		DUOLClient::AI_EnemyBasic* _ai;

	public:
		virtual BT::NodeStatus tick() override;

		static BT::PortsList providedPorts();
	};
}
