#include "DUOLGameEngine/StateMachine/BehaviorTree/Nodes/ConditionNode.h"

namespace DUOLGameEngine
{
	ConditionNode::ConditionNode(const std::string& name) :
		NodeBase(name, NodeType::CONDITION)
	{

	}

	NodeState ConditionNode::Tick()
	{
		return (ConditionCheck() == true ? NodeState::SUCCESS : NodeState::FAILURE);
	}

	void ConditionNode::Stop()
	{

	}
}