#include "DUOLClient/Player/FSM/PlayerState_Die.h"

namespace DUOLClient
{
	PlayerState_Die::PlayerState_Die(DUOLClient::Player* player) :
		PlayerStateBase(TEXT("PlayerState_Die"), player)
	{
	}

	PlayerState_Die::~PlayerState_Die()
	{
	}

	void PlayerState_Die::OnStateEnter(float deltaTime)
	{
		PlayerStateBase::OnStateEnter(deltaTime);
	}

	void PlayerState_Die::OnStateStay(float deltaTime)
	{
		PlayerStateBase::OnStateStay(deltaTime);
	}

	void PlayerState_Die::OnStateExit(float deltaTime)
	{
		PlayerStateBase::OnStateExit(deltaTime);
	}
}