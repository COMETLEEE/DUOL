#include "DUOLClient/Player/FSM/PlayerState_Run.h"

#include "DUOLGameEngine/ECS/Component/Animator.h"

namespace DUOLClient
{
	PlayerState_Run::PlayerState_Run() :
		PlayerStateBase(TEXT("PlayerState_Run"))
	{
	}

	PlayerState_Run::~PlayerState_Run()
	{
	}

	void PlayerState_Run::OnStateEnter(float deltaTime)
	{
		PlayerStateBase::OnStateEnter(deltaTime);

		_animator->SetBool(TEXT("IsMove"), true);

		_animator->SetBool(TEXT("IsRun"), true);
	}

	void PlayerState_Run::OnStateStay(float deltaTime)
	{
		PlayerStateBase::OnStateStay(deltaTime);
	}

	void PlayerState_Run::OnStateExit(float deltaTime)
	{
		PlayerStateBase::OnStateExit(deltaTime);
	}
}