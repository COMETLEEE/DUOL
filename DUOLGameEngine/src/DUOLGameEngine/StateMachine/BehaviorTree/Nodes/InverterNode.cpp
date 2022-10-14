#include "DUOLGameEngine/StateMachine/BehaviorTree/Nodes/InverterNode.h"

namespace DUOLGameEngine
{
	InverterNode::InverterNode(const std::string& name) :
		DecoratorNode(name, NodeType::DECORATOR)
	{

	}

	NodeState InverterNode::Tick()
	{
		if (_node == nullptr)
			return NodeState::FAILURE;

		return (_node->Execute() == NodeState::FAILURE) ? NodeState::SUCCESS : NodeState::FAILURE;
	}

	void InverterNode::Stop()
	{
		if (_node != nullptr)
			_node->SetState(NodeState::IDLE);

		this->SetState(NodeState::IDLE);
	}
}