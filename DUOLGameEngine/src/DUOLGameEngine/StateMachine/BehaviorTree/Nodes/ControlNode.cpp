#include "ControlNode.h"

namespace DUOLGameEngine
{
	ControlNode::ControlNode(const std::string& name) :
		TreeNode(name, NodeType::CONTROL)
		, _currentIndex(0)
	{

	}

	void ControlNode::Stop()
	{
		for (auto& iter : _children)
		{
			iter->Stop();
		}

		this->SetState(NodeState::IDLE);
	}
}