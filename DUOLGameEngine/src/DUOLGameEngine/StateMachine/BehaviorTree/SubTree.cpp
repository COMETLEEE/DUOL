#include "DUOLGameEngine/StateMachine/BehaviorTree/SubTree.h"

namespace DUOLGameEngine
{
	SubTree::SubTree(const std::string& treeName) :
		DecoratorNode(treeName, NodeType::SUBTREE)
	{

	}

	void SubTree::Execute()
	{

	}

	void SubTree::Stop()
	{
		if (_node != nullptr)
			_node->Stop();

		this->SetState(NodeState::IDLE);
	}
}