#include "DUOLClient/Player/FSM/PlayerState_Dash.h"

#include "DUOLGameEngine/ECS/Component/Animator.h"

namespace DUOLClient
{
	PlayerState_Dash::PlayerState_Dash() :
		PlayerStateBase(TEXT("PlayerState_Dash"))
	{

	}

	PlayerState_Dash::~PlayerState_Dash()
	{

	}

	void PlayerState_Dash::OnStateEnter(float deltaTime)
	{
		PlayerStateBase::OnStateEnter(deltaTime);

		_animator->SetBool(TEXT("IsDodge"), true);
	}

	void PlayerState_Dash::OnStateStay(float deltaTime)
	{
		PlayerStateBase::OnStateStay(deltaTime);
	}

	void PlayerState_Dash::OnStateExit(float deltaTime)
	{
		PlayerStateBase::OnStateExit(deltaTime);
	}
}