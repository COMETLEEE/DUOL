/**

	@file      Action_Wait.h
	@brief     데코레이트 노드 샘플입니다.
	@details   ~ Success를 반환할 때까지 Running 상태를 유지합니다.
	@author    SinSeongHyeon
	@date      8.03.2023
	@copyright © SinSeongHyeon, 2023. All right reserved.

**/
#pragma once

#include "behaviortree_cpp/decorator_node.h"

class Decorator_KeepRunningUntilSuccess : public BT::DecoratorNode
{
public:
	Decorator_KeepRunningUntilSuccess(const std::string& name) : DecoratorNode(name, {})
	{
		setRegistrationID("Decorator_KeepRunningUntilSuccess");
	}

private:
	virtual BT::NodeStatus tick() override;
};
inline BT::NodeStatus Decorator_KeepRunningUntilSuccess::tick()
{
	setStatus(BT::NodeStatus::RUNNING);

	const BT::NodeStatus child_state = child_node_->executeTick();

	switch (child_state)
	{
	case BT::NodeStatus::SUCCESS:
	{
		return BT::NodeStatus::SUCCESS;
	}
	case BT::NodeStatus::FAILURE:
	case BT::NodeStatus::RUNNING:
	{
		return BT::NodeStatus::RUNNING;
	}

	default: {
		// TODO throw?
	}
	}
	return status();
}