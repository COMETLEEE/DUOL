#pragma once

#include <behaviortree_cpp/condition_node.h>

namespace DUOLGameEngine
{
	class GameObject;
	class Transform;
}

namespace DUOLClient
{
	class AI_EnemyBasic;

	class Condition_IsInPatrolRange : public BT::ConditionNode
	{
	public:
		Condition_IsInPatrolRange(const std::string& name, const BT::NodeConfig& config);

		virtual ~Condition_IsInPatrolRange() override = default;

	private:
		AI_EnemyBasic* _ai;

		DUOLGameEngine::Transform* _transform;

		DUOLGameEngine::Transform* _targetTransform;

		float _range;

	public:
		virtual BT::NodeStatus tick() override;

		static BT::PortsList providedPorts();
	};
}