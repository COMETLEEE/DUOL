#include "DUOLClient/Player/FSM/PlayerState_Interaction.h"

namespace DUOLClient
{
	PlayerState_Interaction::PlayerState_Interaction(DUOLClient::Player* player) :
		PlayerStateBase(TEXT("PlayerState_Interaction"), player)
	{
	}

	PlayerState_Interaction::~PlayerState_Interaction()
	{
	}

	void PlayerState_Interaction::OnStateEnter(float deltaTime)
	{
		PlayerStateBase::OnStateEnter(deltaTime);
	}

	void PlayerState_Interaction::OnStateStay(float deltaTime)
	{
		PlayerStateBase::OnStateStay(deltaTime);
	}

	void PlayerState_Interaction::OnStateExit(float deltaTime)
	{
		PlayerStateBase::OnStateExit(deltaTime);
	}
}