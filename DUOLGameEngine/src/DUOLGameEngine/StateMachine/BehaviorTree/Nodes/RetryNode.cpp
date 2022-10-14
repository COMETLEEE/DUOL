#include "DUOLGameEngine/StateMachine/BehaviorTree/Nodes/RetryNode.h"

namespace DUOLGameEngine
{
	RetryNode::RetryNode(const std::string& name, int n) :
		DecoratorNode(name, NodeType::DECORATOR)
		, _retryTimes(n)
	{

	}

	void RetryNode::SetRetryTimes(int n)
	{
		if (n < 1)
			_retryTimes = 1;
		else
			_retryTimes = n;
	}

	NodeState RetryNode::Tick()
	{
		if (_node == nullptr)
		{
			SetState(NodeState::FAILURE);

			return NodeState::FAILURE;
		}
		
		for (int i = 0; i < _retryTimes; i++)
		{
			if (_node->Execute() == NodeState::SUCCESS)
			{
				SetState(NodeState::SUCCESS);

				return NodeState::SUCCESS;
			}
		}

		SetState(NodeState::FAILURE);

		return NodeState::FAILURE;
	}

	void RetryNode::Stop()
	{
		if (_node != nullptr)
			_node->SetState(NodeState::IDLE);

		this->SetState(NodeState::IDLE);
	}
}