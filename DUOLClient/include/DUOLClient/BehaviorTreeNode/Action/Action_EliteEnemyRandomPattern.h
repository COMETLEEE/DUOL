/**

	@file      Action_EliteEnemyRandomPattern.h
	@brief     EliteEnemy의 랜덤 패턴을 위한 노드
	@details   ~
	@author    ShinSeongHyeon
	@date      15.05.2023
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

	class Action_EliteEnemyRandomPattern : public BT::StatefulActionNode
	{
	public:
		Action_EliteEnemyRandomPattern(const std::string& name, const BT::NodeConfig& config) :
			StatefulActionNode(name, config),
			_ai(nullptr),
			_animator(nullptr),
			_targetTransform(nullptr),
			_transform(nullptr),
			_rushCount(0),
			patternQueue(),
			_currentPattern(0),
			_timer(0),
			_isIdle(false)
		{}

		virtual ~Action_EliteEnemyRandomPattern() override = default;

	private:
		DUOLClient::AI_EnemyBasic* _ai;

		DUOLGameEngine::Animator* _animator;

		DUOLGameEngine::Transform* _targetTransform;

		DUOLGameEngine::Transform* _transform;

		int _currentPattern;

		int _rushCount;

		float _timer;

		bool _isIdle;
		std::queue<int> patternQueue;
	public:
		virtual BT::NodeStatus onStart() override;

		virtual BT::NodeStatus onRunning() override;

		virtual void onHalted() override;

		static BT::PortsList providedPorts();
	};
}

