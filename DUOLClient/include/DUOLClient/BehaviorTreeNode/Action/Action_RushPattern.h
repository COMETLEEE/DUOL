/**

	@file      Action_RushPattern.h
	@brief     EliteEnemy 돌진 패턴 노드
	@details   ~
	@author    ShinSeongHyeon
	@date      17.05.2023
	@copyright © ShinSeongHyeon, 2023. All right reserved.

**/
#pragma once

#include "behaviortree_cpp/action_node.h"

namespace DUOLGameEngine
{
	class Transform;
	class Animator;
}

namespace DUOLClient
{
	class AI_EnemyBasic;

	class Action_RushPattern : public BT::StatefulActionNode
	{
	public:
		Action_RushPattern(const std::string& name, const BT::NodeConfig& config) :
			StatefulActionNode(name, config),
			_ai(nullptr),
			_animator(nullptr),
			_targetTransform(nullptr),
			_transform(nullptr),
			_timer(0),
			_rushCount(0),
			_isIdle(false)
		{}

		virtual ~Action_RushPattern() override = default;

	private:
		int _rushCount;

		float _timer;

		bool _isIdle;

		DUOLClient::AI_EnemyBasic* _ai;

		DUOLGameEngine::Animator* _animator;

		DUOLGameEngine::Transform* _targetTransform;

		DUOLGameEngine::Transform* _transform;

	public:
		virtual BT::NodeStatus onStart() override;

		virtual BT::NodeStatus onRunning() override;

		virtual void onHalted() override;

		static BT::PortsList providedPorts();
	};
}