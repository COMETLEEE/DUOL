#include "DUOLGameEngine/StateMachine/BehaviorTree/BehaviorTree.h"

namespace DUOLGameEngine
{
	BehaviorTree::BehaviorTree(const std::string& name) :
		_root(name)
	{

	}

	void BehaviorTree::RegistBlackBoard(const std::shared_ptr<BlackBoard>& blackBoard)
	{
		_blackBoard = blackBoard;

		_root.SetBlackBoard(blackBoard);
	}

	void BehaviorTree::Execute()
	{
		_root.Execute();
	}
}
