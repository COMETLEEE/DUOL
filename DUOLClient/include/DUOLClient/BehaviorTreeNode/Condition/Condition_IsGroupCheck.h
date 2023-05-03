/**

	@file      Condition_IsGroupCheck.h
	@brief     무리 내에 타겟을 확인 한 객체가 있는지 확인.
	@details   ~
	@author    SinSeongHyeon
	@date      13.03.2023
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

	class Condition_IsGroupCheck : public BT::ConditionNode
	{
	public:
		Condition_IsGroupCheck(const std::string& name, const BT::NodeConfig& config) :
			ConditionNode(name, config),_ai(nullptr)
		{}

		virtual ~Condition_IsGroupCheck() override = default;

	private:
		AI_EnemyBasic* _ai;

	public:
		virtual BT::NodeStatus tick() override;

		static BT::PortsList providedPorts();

	};
}
