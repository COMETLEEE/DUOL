#include "DUOLGameEngine/StateMachine/BehaviorTree/Nodes/SequenceNode.h"

namespace DUOLGameEngine
{
	SequenceNode::SequenceNode(const std::string& name) :
		ControlNode(name)
	{

	}

	NodeState SequenceNode::Tick()
	{
		for (; _currentIndex < _children.size(); _currentIndex++)
		{
			NodeState childState = _children[_currentIndex]->Tick();

			if (childState == NodeState::RUNNING)
			{
				SetState(NodeState::RUNNING);

				return NodeState::RUNNING;
			}

			if (childState == NodeState::FAILURE)
			{
				_currentIndex = 0;

				SetState(NodeState::FAILURE);

				return NodeState::FAILURE;
			}
		}

		_currentIndex = 0;

		SetState(NodeState::SUCCESS);

		return NodeState::SUCCESS;
	}
}