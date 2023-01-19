#include "DUOLGameEngine/StateMachine/BehaviorTree/Nodes/RepeatNode.h"

namespace DUOLGameEngine
{
	RepeatNode::RepeatNode(const DUOLCommon::tstring& name, int n) :
		DecoratorNode(name, NodeType::DECORATOR)
		, _repeatTimes(n)
	{

	}

	void RepeatNode::SetRetryTimes(int n)
	{
		if (n < 1)
			_repeatTimes = 1;
		else
			_repeatTimes = n;
	}

	NodeState RepeatNode::Tick()
	{
		if (_node == nullptr)
		{
			SetState(NodeState::FAILURE);

			return NodeState::FAILURE;
		}

		for (int i = 0; i < _repeatTimes; i++)
		{
			if (_node->Execute() != NodeState::SUCCESS)
			{
				SetState(NodeState::FAILURE);

				return NodeState::FAILURE;
			}
		}

		SetState(NodeState::SUCCESS);

		return NodeState::SUCCESS;
	}

	void RepeatNode::Stop()
	{
		if (_node != nullptr)
			_node->SetState(NodeState::IDLE);

		this->SetState(NodeState::IDLE);
	}
}