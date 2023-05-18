/**

	@file      Condition_AnimPlayCheck.h
	@brief     애니메이션이 재생 중인지 확인하는 컨디션
	@details   ~ 재생중이라면 SUCCESS를 반환한다.
	@author    Cool Guy
	@date      16.03.2023
	@copyright © Cool Guy, 2023. All right reserved.

**/
#pragma once

#include <behaviortree_cpp/condition_node.h>

namespace DUOLGameEngine
{
	class Animator;
	class GameObject;
}

namespace DUOLClient
{
	class Condition_AnimPlayCheck : public BT::ConditionNode
	{
	public:
		Condition_AnimPlayCheck(const std::string& name, const BT::NodeConfig& config);

		virtual ~Condition_AnimPlayCheck() override = default;

	private:
		DUOLGameEngine::GameObject* _gameObject;

		DUOLGameEngine::Animator* _animator;

		std::string _animName;

	public:
		virtual BT::NodeStatus tick() override;

		static BT::PortsList providedPorts();
	};
}
