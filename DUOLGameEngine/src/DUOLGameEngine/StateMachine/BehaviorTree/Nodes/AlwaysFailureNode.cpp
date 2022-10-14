#include "DUOLGameEngine/StateMachine/BehaviorTree/Nodes/AlwaysFailureNode.h"

namespace DUOLGameEngine
{
	AlwaysFailureNode::AlwaysFailureNode(const std::string& name) :
		DecoratorNode(name, NodeType::DECORATOR)
	{

	}

	NodeState AlwaysFailureNode::Tick()
	{
		if (_node != nullptr)
			_node->Execute();

		SetState(NodeState::FAILURE);

		return NodeState::FAILURE;
	}

	void AlwaysFailureNode::Stop()
	{
		if (_node != nullptr)
			_node->SetState(NodeState::IDLE);

		this->SetState(NodeState::IDLE);
	}
}