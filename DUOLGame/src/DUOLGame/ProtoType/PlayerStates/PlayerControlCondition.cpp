#include "DUOLGame/ProtoType/PlayerStates/PlayerControlCondition.h"

namespace DUOLGame
{
	PlayerControlCondition::PlayerControlCondition() :
		DecoratorNode(_T("PlayerControlCondition"), NodeType::CONDITION)
	{

	}

	PlayerControlCondition::~PlayerControlCondition()
	{

	}

	NodeState PlayerControlCondition::Tick()
	{
		if (GetDataFromBlackBoard<bool>(_T("IsPlayerControl")) == true)
			return NodeState::FAILURE;

		return NodeState::SUCCESS;
	}

	void PlayerControlCondition::Stop()
	{

	}
}