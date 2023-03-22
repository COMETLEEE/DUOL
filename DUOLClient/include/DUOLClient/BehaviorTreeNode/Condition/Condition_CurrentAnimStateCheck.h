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
#include "DUOLCommon/StringHelper.h"

namespace DUOLGameEngine
{
	class Animator;
	class GameObject;
}

namespace DUOLClient
{
	class Condition_CurrentAnimStateCheck : public BT::ConditionNode
	{
	public:
		Condition_CurrentAnimStateCheck(const std::string& name, const BT::NodeConfig& config) :
			ConditionNode(name, config), _gameObject(nullptr)
		{}

		virtual ~Condition_CurrentAnimStateCheck() override = default;

	private:
		DUOLGameEngine::GameObject* _gameObject;

		DUOLGameEngine::Animator* _animator;

		DUOLCommon::tstring _animName;

	public:
		virtual BT::NodeStatus tick() override;

		static BT::PortsList providedPorts();
	};
}
