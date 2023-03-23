#include "DUOLClient/Player/FSM/PlayerState_Dash.h"

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

		// 현재 눌린 방향키 확인한다. 일단, 방향키 쪽으로 AddForce ..!
		// TODO : Root Motion 으로 처리할 예정 ..!
		LookDirectionUpdate();

		_rigidbody->AddForce(_desiredLook * DASH_FORCE);

		// TODO : 현재 플레이어 바라보는 방향 기준, 입력의 방향 벡터를 계산해서 해당하는 8방향 애니메이션 중 하나를 재생한다.
		/*if (_desiredLook.z > 0)
			_animator->SetBool(TEXT("IsFront"), true);
		else if (_desiredLook.z < 0)
			_animator->SetBool(TEXT("IsBack"), true);

		if (_desiredLook.x > 0)
			_animator->SetBool(TEXT("IsRight"), true);
		else if (_desiredLook.x < 0)
			_animator->SetBool(TEXT("IsLeft"), true);*/

		_animator->SetBool(TEXT("IsFront"), true);

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

		if (_isEndDash)
		{
			_stateMachine->TransitionTo(TEXT("PlayerState_Idle"), deltaTime);
		}
		else if (_isEndDash && MoveCheck())
		{
			// 바로 이동 트랜지션도 추가할까요 ?
		}
	}

	void PlayerState_Dash::OnStateExit(float deltaTime)
	{
		PlayerStateBase::OnStateExit(deltaTime);

		_player->_isDash = false;

		_animator->SetBool(TEXT("IsDash"), false);
	}
}