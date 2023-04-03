/**

	@file      Condition_IsLookTarget.h
	@brief     타겟이 시야에 들어왔는지 확인하는 컨디션.
	@details   ~
	@author    SinSeongHyeon
	@date      13.03.2023
	@copyright © SinSeongHyeon, 2023. All right reserved.

**/
#pragma once

#include <behaviortree_cpp/condition_node.h>

namespace DUOLGameEngine
{
	class GameObject;
	class Transform;
}

namespace DUOLClient
{
	class AI_EnemyBase;

	class Condition_IsLookTarget : public BT::ConditionNode
	{
	public:
		Condition_IsLookTarget(const std::string& name, const BT::NodeConfig& config) :
			ConditionNode(name, config), _gameObject(nullptr), _targetTransform(nullptr),
			_range(0)
		{}

		virtual ~Condition_IsLookTarget() override = default;

	private:
		DUOLGameEngine::GameObject* _gameObject;
		DUOLGameEngine::Transform* _targetTransform;
		AI_EnemyBase* _ai;

		float _range;

	public:
		virtual BT::NodeStatus tick() override;

		static BT::PortsList providedPorts();
	};
}
