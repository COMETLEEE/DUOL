#include "DUOLClient/Player/FSM/PlayerState_Die.h"

#include "DUOLGameEngine/ECS/Component/Animator.h"

namespace DUOLClient
{
	PlayerState_Die::PlayerState_Die(DUOLClient::Player* player) :
		PlayerStateBase(TEXT("PlayerState_Die"), player)
	{
#pragma region DIE_EVENT
		_player->AddEventFunction(TEXT("EndDieStart"), std::bind(&DUOLClient::PlayerState_Die::EndDieStart, this));
#pragma endregion
	}

	PlayerState_Die::~PlayerState_Die()
	{
	}

	void PlayerState_Die::EndDieStart()
	{
		_animator->SetBool(TEXT("IsDie"), false);
	}

	void PlayerState_Die::OnStateEnter(float deltaTime)
	{
		PlayerStateBase::OnStateEnter(deltaTime);

		_animator->SetBool(TEXT("IsDie"), true);
	}

	void PlayerState_Die::OnStateStay(float deltaTime)
	{
		PlayerStateBase::OnStateStay(deltaTime);

		// ÀÏ´Ü ½ºÅ×ÀÌÇÏ»ï ..
	}

	void PlayerState_Die::OnStateExit(float deltaTime)
	{
		PlayerStateBase::OnStateExit(deltaTime);

		// ¾ø»ï ..
	}
}