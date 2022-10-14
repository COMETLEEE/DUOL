#include "DUOLGameEngine/StateMachine/BehaviorTree/SubTree.h"

namespace DUOLGameEngine
{
	SubTree::SubTree(const std::string& treeName) :
		DecoratorNode(treeName, NodeType::SUBTREE)
	{

	}

	NodeState SubTree::Tick()
	{
		if (_node == nullptr)
			return NodeState::FAILURE;

		return _node->Execute();
	}

	void SubTree::Stop()
	{
		if (_node != nullptr)
			_node->SetState(NodeState::IDLE);

		this->SetState(NodeState::IDLE);
	}
}