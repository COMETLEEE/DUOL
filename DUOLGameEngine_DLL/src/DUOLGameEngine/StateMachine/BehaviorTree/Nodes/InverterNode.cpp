#include "DUOLGameEngine/StateMachine/BehaviorTree/Nodes/InverterNode.h"

namespace DUOLGameEngine
{
	InverterNode::InverterNode(const DUOLCommon::tstring& name) :
		DecoratorNode(name, NodeType::DECORATOR)
	{

	}

	NodeState InverterNode::Tick()
	{
		if (_node == nullptr)
		{
			SetState(NodeState::FAILURE);

			return NodeState::FAILURE;
		}

		(_node->Execute() == NodeState::FAILURE) ? SetState(NodeState::SUCCESS) : SetState(NodeState::FAILURE);

		return GetState();
	}

	void InverterNode::Stop()
	{
		if (_node != nullptr)
			_node->SetState(NodeState::IDLE);

		this->SetState(NodeState::IDLE);
	}
}