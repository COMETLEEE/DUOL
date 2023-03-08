/**

	@file      Action_Wait.h
	@brief     액션 노드 샘플입니다.
	@details   ~ 네비게이션 메쉬를 사용하지 않고 특정 방향으로 바로 이동 후 Success를 반환합니다.
	@author    SinSeongHyeon
	@date      8.03.2023
	@copyright © SinSeongHyeon, 2023. All right reserved.

**/
#pragma once

#include "behaviortree_cpp/action_node.h"
#include "DUOLMath/DUOLMath.h"

namespace DUOLGameEngine
{
	class GameObject;

	class Action_MoveDirectlyToward : public BT::StatefulActionNode
	{
	public:
		Action_MoveDirectlyToward(const std::string& name, const BT::NodeConfig& config);

	private:
		GameObject* _gameObject;

		DUOLMath::Vector3 _targetPos;

		float _speed;
	public:

		virtual BT::NodeStatus onStart() override { return BT::NodeStatus::RUNNING; }

		virtual BT::NodeStatus onRunning() override;

		virtual void onHalted() override {}

		static BT::PortsList providedPorts();
	};
}

