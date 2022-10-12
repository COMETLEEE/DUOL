#include "DUOLGameEngine/StateMachine/BehaviorTree/Nodes/SelectorNode.h"

namespace DUOLGameEngine
{
	SelectorNode::SelectorNode(const std::string& name) :
		ControlNode(name)
	{

	}

	NodeState SelectorNode::Tick()
	{
		NodeState newState = NodeState::FAILURE;

		for ( ; _currentIndex < _children.size(); _currentIndex++)
		{
			newState = _children[_currentIndex]->Tick();

			if (newState == NodeState::RUNNING)
				return newState;

			if (newState == NodeState::SUCCESS)
			{
				_currentIndex = 0;

				return newState;
			}
		}

		_currentIndex = 0;

		return newState;
	}
}