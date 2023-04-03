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
	class AI_EnemyBase;

	class Action_UseTokenAttack : public BT::StatefulActionNode
	{
	public:
		Action_UseTokenAttack(const std::string& name, const BT::NodeConfig& config) :
			StatefulActionNode(name, config), _gameObject(nullptr), _targetTransform(nullptr),
			_navMeshAgent(nullptr) {}

		virtual ~Action_UseTokenAttack() override = default;
	private:
		DUOLGameEngine::GameObject* _gameObject;

		DUOLGameEngine::NavMeshAgent* _navMeshAgent;

		DUOLGameEngine::Transform* _targetTransform;

		DUOLGameEngine::Animator* _animator;

		AI_EnemyBase* _ai;

	public:
		virtual BT::NodeStatus onStart() override;

		virtual BT::NodeStatus onRunning() override;

		virtual void onHalted() override;

		static BT::PortsList providedPorts();

	};
}
