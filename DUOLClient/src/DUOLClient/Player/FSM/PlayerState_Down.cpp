#include "DUOLClient/Player/FSM/PlayerState_Down.h"

#include "DUOLGameEngine/ECS/Component/Animator.h"

namespace DUOLClient
{
	PlayerState_Down::PlayerState_Down(DUOLClient::Player* player) :
		PlayerStateBase(TEXT("PlayerState_Down"), player)
	{

	}

	PlayerState_Down::~PlayerState_Down()
	{

	}

	void PlayerState_Down::OnStateEnter(float deltaTime)
	{
		PlayerStateBase::OnStateEnter(deltaTime);

		_animator->SetBool(TEXT("IsDown"), true);
	}

	void PlayerState_Down::OnStateStay(float deltaTime)
	{
		PlayerStateBase::OnStateStay(deltaTime);
	}

	void PlayerState_Down::OnStateExit(float deltaTime)
	{
		PlayerStateBase::OnStateExit(deltaTime);

		_animator->SetBool(TEXT("IsDown"), false);
	}
}