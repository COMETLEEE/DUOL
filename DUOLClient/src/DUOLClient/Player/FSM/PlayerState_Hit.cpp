#include "DUOLClient/Player/FSM/PlayerState_Hit.h"

namespace DUOLClient
{
	PlayerState_Hit::PlayerState_Hit() :
		PlayerStateBase(TEXT("PlayerState_Hit"))
		, _downPoint(0.f)
	{
	}

	PlayerState_Hit::~PlayerState_Hit()
	{
	}

	void PlayerState_Hit::OnStateEnter(float deltaTime)
	{
		PlayerStateBase::OnStateEnter(deltaTime);

		_downPoint += DOWN_POINT_PER_ATTACK;

		if (_downPoint >= MAX_DOWN_POINT)
		{
			_stateMachine->TransitionTo(TEXT("PlayerState_Down"), deltaTime);
		}
	}

	void PlayerState_Hit::OnStateStay(float deltaTime)
	{
		PlayerStateBase::OnStateStay(deltaTime);
		
	}

	void PlayerState_Hit::OnStateExit(float deltaTime)
	{
		PlayerStateBase::OnStateExit(deltaTime);
	}
}