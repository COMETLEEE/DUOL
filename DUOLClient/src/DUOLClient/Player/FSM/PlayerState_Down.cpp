#include "DUOLClient/Player/FSM/PlayerState_Down.h"

#include "DUOLGameEngine/ECS/Component/Animator.h"

namespace DUOLClient
{
	PlayerState_Down::PlayerState_Down(DUOLClient::Player* player) :
		PlayerStateBase(TEXT("PlayerState_Down"), player)
	{
#pragma region DOWN_EVENT
		_player->AddEventFunction(TEXT("EndDown"), std::bind(&DUOLClient::PlayerState_Down::EndDown, this));

		_player->AddEventFunction(TEXT("EndDownup"), std::bind(&DUOLClient::PlayerState_Down::EndDownup, this));
#pragma endregion
	}

	PlayerState_Down::~PlayerState_Down()
	{

	}

	void PlayerState_Down::EndDown()
	{
		_animator->SetBool(TEXT("IsDown"), false);

		_animator->SetBool(TEXT("IsDownup"), true);
	}

	void PlayerState_Down::EndDownup()
	{
		_animator->SetBool(TEXT("IsDownup"), false);

		_stateMachine->TransitionTo(TEXT("PlayerState_Idle"), 0.f);
	}

	void PlayerState_Down::OnStateEnter(float deltaTime)
	{
		PlayerStateBase::OnStateEnter(deltaTime);

		// 다운 햇으니 다운 포인트 초기화
		_player->_currentDownPoint = 0.f;

		_animator->SetBool(TEXT("IsDown"), true);
	}

	void PlayerState_Down::OnStateStay(float deltaTime)
	{
		PlayerStateBase::OnStateStay(deltaTime);
	}

	void PlayerState_Down::OnStateExit(float deltaTime)
	{
		PlayerStateBase::OnStateExit(deltaTime);
	}
}