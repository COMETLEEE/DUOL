#include "DUOLClient/Player/FSM/PlayerState_Attack.h"

#include "DUOLGameEngine/ECS/Component/Animator.h"

namespace DUOLClient
{
	PlayerState_Attack::PlayerState_Attack() :
		PlayerStateBase(TEXT("PlayerState_Attack"))
	{
	}

	PlayerState_Attack::~PlayerState_Attack()
	{
	}

	void PlayerState_Attack::OnStateEnter(float deltaTime)
	{
		PlayerStateBase::OnStateEnter(deltaTime);

		_animator->SetBool(TEXT("IsAttack"), true);
	}

	void PlayerState_Attack::OnStateStay(float deltaTime)
	{
		PlayerStateBase::OnStateStay(deltaTime);
		static float time = 0;

		time += deltaTime;

		if (time > 4)
		{
			_stateMachine->TransitionTo(TEXT("PlayerState_Idle"), deltaTime);

			time = 0;
		}
	}

	void PlayerState_Attack::OnStateExit(float deltaTime)
	{
		PlayerStateBase::OnStateExit(deltaTime);

		_animator->SetBool(TEXT("IsAttack"), false);
	}
}
