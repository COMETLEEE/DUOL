/**

	@file      Action_BoidsMoveTo.h
	@brief     Boids Algorithm을 적용시킨 MoveTo
	@details   ~
	@author    Cool Guy
	@date      24.03.2023
	@copyright © Cool Guy, 2023. All right reserved.

**/

#pragma once

#include "behaviortree_cpp/action_node.h"

namespace DUOLGameEngine
{
	class Animator;
	class GameObject;
	class Transform;
	class NavMeshAgent;
}
namespace DUOLClient
{
	class AI_EnemyBasic;
	class EnemyGroupController;

	class Action_BoidsMoveTo : public BT::StatefulActionNode
	{
	public:
		Action_BoidsMoveTo(const std::string& name, const BT::NodeConfig& config);

		virtual ~Action_BoidsMoveTo() override = default;

	private:
		DUOLGameEngine::GameObject* _gameObject;

		DUOLGameEngine::NavMeshAgent* _navMeshAgent;

		DUOLGameEngine::Transform* _targetTransform;

		AI_EnemyBasic* _ai;

		EnemyGroupController* _enemyGroupController;

		DUOLGameEngine::Animator* _animator;

	public:
		virtual BT::NodeStatus onStart() override;

		virtual BT::NodeStatus onRunning() override;

		virtual void onHalted() override;

		static BT::PortsList providedPorts();
	};
}
