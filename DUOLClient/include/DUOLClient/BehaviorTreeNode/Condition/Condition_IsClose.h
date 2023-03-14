/**

	@file      Condition_IsClose.h
	@brief     타겟과 거리가 가까운지 확인하는 컨디션
	@details   ~
	@author    SinSeongHyeon
	@date      14.03.2023
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
	class Condition_IsClose : public BT::ConditionNode
	{
	public:
		Condition_IsClose(const std::string& name, const BT::NodeConfig& config) :
			ConditionNode(name, config), _gameObject(nullptr), _targetTransform(nullptr), _distance(0)
		{}

		virtual ~Condition_IsClose() override = default;

	private:
		DUOLGameEngine::GameObject* _gameObject;

		DUOLGameEngine::Transform* _targetTransform;

		float _distance;
	public:
		virtual BT::NodeStatus tick() override;

		static BT::PortsList providedPorts();

	};
}
