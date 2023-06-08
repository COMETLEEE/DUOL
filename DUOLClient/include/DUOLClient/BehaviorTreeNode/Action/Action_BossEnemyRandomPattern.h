/**

	@file      Action_BossEnemyRandomPattern.h
	@brief     BossEnemy의 랜덤 패턴을 위한 노드
	@details   ~
	@author    ShinSeongHyeon
	@date      08.06.2023
	@copyright © ShinSeongHyeon, 2023. All right reserved.

**/
#pragma once

#include <queue>

#include "behaviortree_cpp/action_node.h"

namespace DUOLGameEngine
{
	class Transform;
	class Animator;
}

namespace DUOLClient
{
	class AI_EnemyBasic;

	class Action_BossEnemyRandomPattern : public BT::SyncActionNode
	{
	public:
		Action_BossEnemyRandomPattern(const std::string& name, const BT::NodeConfig& config);

		virtual ~Action_BossEnemyRandomPattern() override = default;

	private:
		DUOLClient::AI_EnemyBasic* _ai;

		DUOLGameEngine::Animator* _animator;

		DUOLGameEngine::Transform* _targetTransform;

		DUOLGameEngine::Transform* _transform;

		int _currentPattern;

		std::queue<int> patternQueue;
	private:
		void SwordPattern();

		void FistPattern();

	public:
		virtual BT::NodeStatus tick() override;

		static BT::PortsList providedPorts();
	};
}
