#include "DUOLGameEngine/StateMachine/BehaviorTree/Nodes/ControlNode.h"

namespace DUOLGameEngine
{
	ControlNode::ControlNode(const DUOLCommon::tstring& name) :
		TreeNode(name, NodeType::CONTROL)
		, _currentIndex(0)
	{

	}

	void ControlNode::Stop()
	{
		for (auto& iter : _children)
		{
			iter->SetState(NodeState::IDLE);
		}

		_currentIndex = 0;

		this->SetState(NodeState::IDLE);
	}
}