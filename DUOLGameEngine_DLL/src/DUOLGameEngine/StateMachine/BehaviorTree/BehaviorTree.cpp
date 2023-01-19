#include "DUOLGameEngine/StateMachine/BehaviorTree/BehaviorTree.h"

namespace DUOLGameEngine
{
	BehaviorTree::BehaviorTree(const DUOLCommon::tstring& name) :
		_root(std::make_shared<SubTree>(name))
	{
		
	}

	void BehaviorTree::RegistBlackBoard(const std::shared_ptr<BlackBoard>& blackBoard)
	{
		_root->SetBlackBoard(blackBoard);
	}

	void BehaviorTree::Execute()
	{
		_root->Execute();
	}
}
