#include "DUOLClient/Player/FSM/PlayerState_Dash.h"

#include "DUOLCommon/Log/LogHelper.h"
#include "DUOLGameEngine/ECS/Component/Transform.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"

namespace DUOLClient
{
	PlayerState_Dash::PlayerState_Dash(DUOLClient::Player* player) :
		PlayerStateBase(TEXT("PlayerState_Dash"), player)
	{
		_player->AddEventFunction(TEXT("StartDash"), std::bind(&DUOLClient::PlayerState_Dash::OnStartDash, this));

		_player->AddEventFunction(TEXT("EndDash"), std::bind(&DUOLClient::PlayerState_Dash::OnEndDash, this));
	}

	PlayerState_Dash::~PlayerState_Dash()
	{
		
	}

	void PlayerState_Dash::OnStartDash()
	{
		// 무적 시작.
		_isEndDash = false;
	}

	void PlayerState_Dash::OnEndDash()
	{
		// 무적 끝. 다른 스테이트로 전환됩니다.
		_isEndDash = true;
	}

	void PlayerState_Dash::OnStateEnter(float deltaTime)
	{
		PlayerStateBase::OnStateEnter(deltaTime);

		_animator->SetBool(TEXT("IsDash"), true);

		// Lock on mode
		if (_player->_isLockOnMode)
		{
			// TODO : 8방향 대쉬 애니메이션으로 전환
		}
		// Idle
		else if (_stateMachine->GetPrevState()->GetName() == TEXT("PlayerState_Idle"))
		{
			// 그냥 앞에 보고 대쉬한다
		}
		// Move, Attack
		else
		{
			// 대쉬 방향을 정하자
			LookDirectionUpdate();

			_transform->LookAt(_transform->GetWorldPosition() + _desiredLook * 10.f);
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

		if (_isEndDash && MoveCheck() && RunCheck())
		{
			_stateMachine->TransitionTo(TEXT("PlayerState_Run"), deltaTime);
		}
		else if (_isEndDash && MoveCheck())
		{
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

		// �÷��̾��� ��� ������Ʈ�� ������ �� �ִ� ���� '_isDash' �ʱ�ȭ.
		_player->_isDash = false;

		_animator->SetBool(TEXT("IsDash"), false);
	}
}