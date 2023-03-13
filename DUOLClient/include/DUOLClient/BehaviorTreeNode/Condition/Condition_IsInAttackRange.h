/**

	@file      Condition_IsInActtackRange.h
	@brief     사정거리에 진입하였는지 확인하는 컨디션
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
	class Condition_IsInAttackRange : public BT::ConditionNode
	{
	public:
		Condition_IsInAttackRange(const std::string& name, const BT::NodeConfig& config) :
			ConditionNode(name, config), _gameObject(nullptr), _targetTransform(nullptr),
			_range(0)
		{}

		~Condition_IsInAttackRange() override = default;

	private:
		DUOLGameEngine::GameObject* _gameObject;
		DUOLGameEngine::Transform* _targetTransform;
		float _range;

	public:
		virtual BT::NodeStatus tick() override;

		static BT::PortsList providedPorts();
	};
}
