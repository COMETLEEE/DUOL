/**

	@file      Condition_IsTargetDie.h
	@brief     타겟이 아직 살아있는지 확인하는 함수.
	@details   ~
	@author    ShinSeongHyeon
	@date      19.05.2023
	@copyright © ShinSeongHyeon, 2023. All right reserved.

**/
#pragma once

#include <behaviortree_cpp/condition_node.h>

namespace DUOLClient
{
	class CharacterBase;
	class AI_EnemyBasic;

	class Condition_IsTargetDie : public BT::ConditionNode
	{
	public:
		Condition_IsTargetDie(const std::string& name, const BT::NodeConfig& config);

		virtual ~Condition_IsTargetDie() override = default;

	private:
		AI_EnemyBasic* _ai;

		CharacterBase* targetCharacterBase;

	public:
		virtual BT::NodeStatus tick() override;

		static BT::PortsList providedPorts();
	};

}

