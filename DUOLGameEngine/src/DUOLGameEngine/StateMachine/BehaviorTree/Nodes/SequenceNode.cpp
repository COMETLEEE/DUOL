#include "DUOLGameEngine/StateMachine/BehaviorTree/Nodes/SequenceNode.h"

namespace DUOLGameEngine
{
	SequenceNode::SequenceNode(const DUOLCommon::tstring& name) :
		ControlNode(name)
	{

	}

	NodeState SequenceNode::Tick()
	{
		for (; _currentIndex < _children.size(); _currentIndex++)
		{
			NodeState childState = _children[_currentIndex]->Execute();

			if (childState == NodeState::RUNNING)
				return NodeState::RUNNING;

			if (childState == NodeState::FAILURE)
			{
				Stop();

				return NodeState::FAILURE;
			}
		}

		Stop();

		return NodeState::SUCCESS;
	}
}