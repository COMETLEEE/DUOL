#include "DUOLGame/ProtoType/PlayerStates/PlayerGroundCondition.h"

namespace DUOLGame
{
	PlayerGroundCondition::PlayerGroundCondition() :
		DecoratorNode(_T("PlayerGroundCondition"), NodeType::DECORATOR)
	{

	}

	PlayerGroundCondition::~PlayerGroundCondition()
	{

	}

	NodeState PlayerGroundCondition::Tick()
	{
		if (GetDataFromBlackBoard<bool>(_T("GroundCondition")) == true)
			return NodeState::SUCCESS;

		return NodeState::FAILURE;
	}

	void PlayerGroundCondition::Stop()
	{

	}
}