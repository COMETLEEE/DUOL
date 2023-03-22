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

	class Action_AroundPatrol : public BT::StatefulActionNode
	{
	public:
		Action_AroundPatrol(const std::string& name, const BT::NodeConfig& config) :
			StatefulActionNode(name, config), _gameObject(nullptr), _targetTransform(nullptr),
			_navMeshAgent(nullptr), _randomOffset(0), _distance(0) {}

		virtual ~Action_AroundPatrol() override = default;
	private:
		DUOLGameEngine::GameObject* _gameObject;

		DUOLGameEngine::NavMeshAgent* _navMeshAgent;

		DUOLGameEngine::Transform* _targetTransform;

		DUOLGameEngine::Animator* _animator;

		float _randomOffset;

		float _distance;

		DUOLMath::Vector3 _dest;

	public:
		virtual BT::NodeStatus onStart() override;

		virtual BT::NodeStatus onRunning() override;

		virtual void onHalted() override;

		static BT::PortsList providedPorts();

	};

}
