#include "DUOLGameEngine/StateMachine/BehaviorTree/Nodes/SelectorNode.h"

namespace DUOLGameEngine
{
	SelectorNode::SelectorNode(const std::string& name) :
		ControlNode(name)
	{

	}

	NodeState SelectorNode::Tick()
	{
		for (; _currentIndex < _children.size(); _currentIndex++)
		{
			NodeState childState = _children[_currentIndex]->Tick();

			if (childState == NodeState::RUNNING)
			{
				SetState(NodeState::RUNNING);

				return NodeState::RUNNING;
			}

			if (childState == NodeState::SUCCESS)
			{
				_currentIndex = 0;

				SetState(NodeState::SUCCESS);

				return NodeState::SUCCESS;
			}
		}

		_currentIndex = 0;

		SetState(NodeState::FAILURE);

		return NodeState::FAILURE;
	}
}