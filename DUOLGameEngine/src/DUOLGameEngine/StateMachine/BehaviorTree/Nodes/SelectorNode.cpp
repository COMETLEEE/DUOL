#include "DUOLGameEngine/StateMachine/BehaviorTree/Nodes/SelectorNode.h"

namespace DUOLGameEngine
{
	SelectorNode::SelectorNode(const DUOLCommon::tstring& name) :
		ControlNode(name)
	{

	}

	NodeState SelectorNode::Tick()
	{
		for (; _currentIndex < _children.size(); _currentIndex++)
		{
			NodeState childState = _children[_currentIndex]->Execute();

			if (childState == NodeState::RUNNING)
				return NodeState::RUNNING;

			if (childState == NodeState::SUCCESS)
			{
				Stop();

				return NodeState::SUCCESS;
			}
		}

		Stop();

		return NodeState::FAILURE;
	}
}