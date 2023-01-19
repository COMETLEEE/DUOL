#include "DUOLGameEngine/StateMachine/BehaviorTree/Nodes/AlwaysSuccessNode.h"

namespace DUOLGameEngine
{
	AlwaysSuccessNode::AlwaysSuccessNode(const DUOLCommon::tstring& name) :
		DecoratorNode(name, NodeType::DECORATOR)
	{

	}

	NodeState AlwaysSuccessNode::Tick()
	{
		if (_node != nullptr)
			_node->Execute();

		SetState(NodeState::SUCCESS);

		return NodeState::SUCCESS;
	}

	void AlwaysSuccessNode::Stop()
	{
		if (_node != nullptr)
			_node->SetState(NodeState::IDLE);

		this->SetState(NodeState::IDLE);
	}
}