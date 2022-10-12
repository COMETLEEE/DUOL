#include "DUOLGameEngine/StateMachine/BehaviorTree/Nodes/ConditionNode.h"

namespace DUOLGameEngine
{
	ConditionNode::ConditionNode(const std::string& name) :
		NodeBase(name, NodeType::CONDITION)
	{

	}

	NodeState ConditionNode::Tick()
	{
		(ConditionCheck() == true ? SetState(NodeState::SUCCESS) : SetState(NodeState::FAILURE));

		return GetState();
	}

	void ConditionNode::Stop()
	{

	}
}