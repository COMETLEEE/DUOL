/**

	@file      Action_AroundPatrol.h
	@brief     목표 주위를 맴도는 액션 노드
	@details   ~
	@author    SinSeongHyeon
	@date      14.03.2023
	@copyright © SinSeongHyeon, 2023. All right reserved.

**/
#pragma once


#include "behaviortree_cpp/action_node.h"
#include "DUOLMath/DUOLMath.h"

namespace DUOLGameEngine
{
	class Animator;
	class GameObject;
	class Transform;
	class NavMeshAgent;
}

namespace DUOLClient
{
	class EnemyGroupController;
	class AI_EnemyBasic;

	class Action_AroundPatrol : public BT::StatefulActionNode
	{
	public:
		Action_AroundPatrol(const std::string& name, const BT::NodeConfig& config);

		virtual ~Action_AroundPatrol() override = default;
	private:
		DUOLGameEngine::NavMeshAgent* _navMeshAgent;

		DUOLGameEngine::Transform* _targetTransform;

		DUOLGameEngine::Transform* _transform;

		DUOLGameEngine::Animator* _animator;

		DUOLClient::AI_EnemyBasic* _ai;

		DUOLClient::EnemyGroupController* _enemyGroupController;

		float _randomOffset;

		float _distance;

		float _tiemr;

		DUOLMath::Vector3 _dest;

	public:
		virtual BT::NodeStatus onStart() override;

		virtual BT::NodeStatus onRunning() override;

		virtual void onHalted() override;

		static BT::PortsList providedPorts();

	};

}
