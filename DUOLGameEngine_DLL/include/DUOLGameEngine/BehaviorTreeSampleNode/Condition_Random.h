/**

	@file      Condition_Random.h
	@brief     랜덤으로 참 거짓을 반환하는 컨디션 노드
	@details   ~
	@author    SinSeongHyeon
	@date      14.03.2023
	@copyright © SinSeongHyeon, 2023. All right reserved.

**/
#pragma once

#include "behaviortree_cpp/condition_node.h"

namespace DUOLGameEngine
{
	class Condition_Random : public BT::ConditionNode
	{
	public:
		Condition_Random(const std::string& name, const BT::NodeConfig& config) :
			ConditionNode(name, config)
		{
			_randomRatio = getInput<float>("RandomRatio").value();
		}

		virtual ~Condition_Random() override = default;
	private:
		float _randomRatio;
	public:
		virtual BT::NodeStatus tick() override;

		static BT::PortsList providedPorts();
	};
}

