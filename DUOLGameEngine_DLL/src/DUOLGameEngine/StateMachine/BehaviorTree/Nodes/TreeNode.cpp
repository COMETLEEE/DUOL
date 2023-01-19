#include "DUOLGameEngine/StateMachine/BehaviorTree/Nodes/TreeNode.h"

namespace DUOLGameEngine
{
    TreeNode::TreeNode(const DUOLCommon::tstring& name, NodeType type) :
		NodeBase(name, type)
    {

    }

    bool TreeNode::SubChild(unsigned int UID)
    {
		for (int i = 0; i < _children.size(); i++)
		{
			if (_children[i]->GetUID() != UID)
				continue;

			NodeState targetNodeState = _children[i]->GetState();

			if (targetNodeState != NodeState::IDLE)
				_children[i]->Stop();

			_children.erase(_children.begin() + i);

			return true;
		}

		return false;
    }
}