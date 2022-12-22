#include "DUOLGame/ProtoType/PlayerStates/PlayerIdle.h"

#include "DUOLCommon/LogHelper.h"

namespace DUOLGame
{
	PlayerIdle::PlayerIdle() :
		ActionNode(_T("PlayerIdle"))
	{

	}

	PlayerIdle::~PlayerIdle()
	{

	}

	NodeState PlayerIdle::Tick()
	{
		auto deltaTime = GetDataFromBlackBoard<float>(_T("DeltaTime"));

		static float coolTime = 0.0f;

		if (coolTime <= 0.0f)
		{
			coolTime = 1.0f;

			DUOL_INFO("Player Idle");
		}
		else
		{
			coolTime -= deltaTime;
		}

		return NodeState::SUCCESS;
	}

	void PlayerIdle::Stop()
	{

	}
}