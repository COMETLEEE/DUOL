/**

	@file      Condition_IsLookTarget.h
	@brief     타겟이 시야에 들어왔는지 확인하는 컨디션.
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
	class Transform;
}

namespace DUOLClient
{
	class AI_EnemyBasic;

	class Condition_IsLookTarget : public BT::ConditionNode
	{
	public:
		Condition_IsLookTarget(const std::string& name, const BT::NodeConfig& config) :
			ConditionNode(name, config)
		{}

		virtual ~Condition_IsLookTarget() override = default;

	private:
		AI_EnemyBasic* _ai;

	public:
		virtual BT::NodeStatus tick() override;

		static BT::PortsList providedPorts();
	};
}
