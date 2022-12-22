#include "DUOLGame/ProtoType/PlayerStates/PlayerGroundCondition.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"

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