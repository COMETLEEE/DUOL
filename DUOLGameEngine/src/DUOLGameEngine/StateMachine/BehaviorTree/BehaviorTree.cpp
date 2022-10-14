#include "DUOLGameEngine/StateMachine/BehaviorTree/BehaviorTree.h"

namespace DUOLGameEngine
{
	BehaviorTree::BehaviorTree(const std::string& name) :
		_root(name)
	{

	}

	void BehaviorTree::Execute()
	{
		_root.Execute();
	}
}
