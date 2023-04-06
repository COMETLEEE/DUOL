#include "DUOLClient/Player/FSM/PlayerState_Transcendence.h"

namespace DUOLClient
{
	PlayerState_Transcendence::PlayerState_Transcendence(DUOLClient::Player* player) :
		PlayerStateBase(TEXT("PlayerState_Transcendence"), player)
	{
	}

	PlayerState_Transcendence::~PlayerState_Transcendence()
	{
	}

	void PlayerState_Transcendence::OnStateEnter(float deltaTime)
	{
		PlayerStateBase::OnStateEnter(deltaTime);
	}

	void PlayerState_Transcendence::OnStateStay(float deltaTime)
	{
		PlayerStateBase::OnStateStay(deltaTime);
	}

	void PlayerState_Transcendence::OnStateExit(float deltaTime)
	{
		PlayerStateBase::OnStateExit(deltaTime);
	}
}