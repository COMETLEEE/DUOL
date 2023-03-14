/**

	@file      Action_Wait.h
	@brief     액션 노드 샘플입니다.
	@details   ~ 네비게이션 매쉬를 사용하여 이동 후 Success를 반환합니다.
	@author    SinSeongHyeon
	@date      8.03.2023
	@copyright © SinSeongHyeon, 2023. All right reserved.

**/
#pragma once

#include "behaviortree_cpp/action_node.h"

namespace DUOLGameEngine
{
	class GameObject;
	class Transform;
	class NavMeshAgent;

	class Action_MoveTo : public BT::StatefulActionNode
	{
	public:
		Action_MoveTo(const std::string& name, const BT::NodeConfig& config);

		virtual ~Action_MoveTo() override = default;

	private:
		GameObject* _gameObject;

		NavMeshAgent* _navMeshAgent;

		Transform* _targetTransform;

	public:
		virtual BT::NodeStatus onStart() override;

		virtual BT::NodeStatus onRunning() override;

		virtual void onHalted() override;

		static BT::PortsList providedPorts();
	};
}
