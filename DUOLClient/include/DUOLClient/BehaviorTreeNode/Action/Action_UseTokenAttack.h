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

	class Action_UseTokenAttack : public BT::StatefulActionNode
	{
	public:
		Action_UseTokenAttack(const std::string& name, const BT::NodeConfig& config);

		virtual ~Action_UseTokenAttack() override = default;
	private:
		DUOLGameEngine::Transform* _targetTransform;

		DUOLGameEngine::Animator* _animator;

		AI_EnemyBasic* _ai;

	public:
		virtual BT::NodeStatus onStart() override;

		virtual BT::NodeStatus onRunning() override;

		virtual void onHalted() override;

		static BT::PortsList providedPorts();

	};
}
