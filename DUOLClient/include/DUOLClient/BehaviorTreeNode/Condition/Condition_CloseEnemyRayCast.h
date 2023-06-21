/**

	@file      Condition_CloseEnemyRayCast.h
	@brief     몬스터와 플레이어 사이에 다른 객체가 있는지 확인하는 컨디션
	@details   ~ 아무것도 없다면 SUCCESS를 반환
	@author    ShinSeongHyeon
	@date      21.06.2023
	@copyright © Cool Guy, 2023. All right reserved.

**/
#pragma once
#include "Condition_AnimPlayCheck.h"

namespace DUOLClient
{
	class AI_EnemyBasic;

	class Condition_CloseEnemyRayCast : public BT::ConditionNode
	{
	public:
		Condition_CloseEnemyRayCast(const std::string& name, const BT::NodeConfig& config);

		virtual ~Condition_CloseEnemyRayCast() override = default;

	private:
		DUOLClient::AI_EnemyBasic* _ai;

	public:
		virtual BT::NodeStatus tick() override;

		static BT::PortsList providedPorts();
	};
}

