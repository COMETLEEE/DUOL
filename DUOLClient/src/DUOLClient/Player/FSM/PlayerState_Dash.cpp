#include "DUOLClient/Player/FSM/PlayerState_Dash.h"

#include "DUOLGameEngine/ECS/Component/Transform.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"

namespace DUOLClient
{
	PlayerState_Dash::PlayerState_Dash() :
		PlayerStateBase(TEXT("PlayerState_Dash"))
	{
		_eventListenerIDHolder.AddEventFunction(TEXT("Player_StartDash"), std::bind(&DUOLClient::PlayerState_Dash::OnStartDash, this));

		_eventListenerIDHolder.AddEventFunction(TEXT("Player_EndDash"), std::bind(&DUOLClient::PlayerState_Dash::OnEndDash, this));
	}

	PlayerState_Dash::~PlayerState_Dash()
	{
		
	}

	void PlayerState_Dash::OnStartDash()
	{
		// 무적스 ?
		_isEndDash = false;
	}

	void PlayerState_Dash::OnEndDash()
	{
		// 무적스
		_isEndDash = true;
	}

	void PlayerState_Dash::OnStateEnter(float deltaTime)
	{
		PlayerStateBase::OnStateEnter(deltaTime);

		_animator->SetBool(TEXT("IsDash"), true);

		// Lock on mode
		if (_player->_isLockOnMode)
		{
			// TODO : 눌린 방향에 대해서 8방향 대쉬로의 전이를 진행합니다.
			// 
		}
		// Idle
		else if (_stateMachine->GetPrevState()->GetName() == TEXT("PlayerState_Idle"))
		{
			// 현재 바라보고 있는 방향 그대로 가자.
		}
		// Move, Attack
		else
		{
			// 누르고 있는 방향키를 받자.
			LookDirectionUpdate();

			_transform->LookAt(_transform->GetWorldPosition() + _desiredLook);
		}

		_player->_isDash = true;

		_isEndDash = false;
	}

	void PlayerState_Dash::OnStateStay(float deltaTime)
	{
		PlayerStateBase::OnStateStay(deltaTime);

		if (DieCheck())
		{
			_stateMachine->TransitionTo(TEXT("PlayerState_Die"), deltaTime);
		}

		if (_isEndDash && MoveCheck())
		{
			// 바로 이동 트랜지션도 추가할까요 ?
			_stateMachine->TransitionTo(TEXT("PlayerState_Move"), deltaTime);
		}
		else if (_isEndDash)
		{
			_stateMachine->TransitionTo(TEXT("PlayerState_Idle"), deltaTime);
		}
	}

	void PlayerState_Dash::OnStateExit(float deltaTime)
	{
		PlayerStateBase::OnStateExit(deltaTime);

		// 플레이어의 모든 스테이트가 공유할 수 있는 변수 '_isDash' 초기화.
		_player->_isDash = false;

		_animator->SetBool(TEXT("IsDash"), false);
	}
}