#include "DUOLClient/Player/FSM/PlayerState_Move.h"

namespace DUOLClient
{
	PlayerState_Move::PlayerState_Move() :
		StateBase(TEXT("PlayerState_Move"))
	{
	}

	PlayerState_Move::~PlayerState_Move()
	{
	}

	void PlayerState_Move::OnStateEnter(float deltaTime)
	{
		// StateBase::OnStateEnter(deltaTime);
	}

	void PlayerState_Move::OnStateStay(float deltaTime)
	{
		// StateBase::OnStateStay(deltaTime);
	}

	void PlayerState_Move::OnStateExit(float deltaTime)
	{
		// StateBase::OnStateExit(deltaTime);
	}
}