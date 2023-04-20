/**

	@file      Action_Hit.h
	@brief     Action_Hit 피격 처리를 하는 노드
	@details   ~ Hit 노드는 몬스터 당 하나만 가지고 있을 것이다. 그렇기 때문에 내부에서 여러가지 처리를 해도 괜찮을 듯.
	@author    SinSeongHyeon
	@date      14.03.2023
	@copyright © SinSeongHyeon, 2023. All right reserved.

**/
#pragma once

#include "behaviortree_cpp/action_node.h"

namespace DUOLClient
{
	class AI_EnemyBasic;



	class Action_Hit : public BT::SyncActionNode
	{
	public:
		Action_Hit(const std::string& name, const BT::NodeConfig& config);

		virtual ~Action_Hit() override = default;
	private:
		AI_EnemyBasic* _ai;

		float _delayTime;

		float _timer;

		bool _hitOnce;
	public:
		virtual BT::NodeStatus tick() override;

		static BT::PortsList providedPorts();
	};
}
