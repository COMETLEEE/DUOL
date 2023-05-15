/**

	@file      Action_EliteMonsterNormalOrHeavyAttack.h
	@brief     EliteMonster의 기본 공격과 강 공격의 랜덤성과 쿨타임을 주기위한 클래스.
	@details   ~ 재사용은 고려하지 않음.
	@author    ShinSeongHyeon
	@date      15.05.2023
	@copyright © ShinSeongHyeon, 2023. All right reserved.

**/
#pragma once


#include "behaviortree_cpp/action_node.h"

namespace DUOLGameEngine
{
	class TimeManager;
	class Animator;
	class NavMeshAgent;
}

namespace DUOLClient
{
	class AI_EnemyBasic;

	class Action_EliteMonsterNormalOrHeavyAttack : public BT::SyncActionNode
	{
	public:
		Action_EliteMonsterNormalOrHeavyAttack(const std::string& name, const BT::NodeConfig& config) :
			SyncActionNode(name, config), _ai(nullptr), _lastHeavyAttackTime(0)
		{}

		virtual ~Action_EliteMonsterNormalOrHeavyAttack() override = default;
	private:
		AI_EnemyBasic* _ai;

		DUOLGameEngine::NavMeshAgent* _navMesh;

		DUOLGameEngine::Animator* _animator;

		DUOLGameEngine::TimeManager* _timeManager;

		float _lastHeavyAttackTime;
	public:
		virtual BT::NodeStatus tick() override;

		static BT::PortsList providedPorts();
	};
}
