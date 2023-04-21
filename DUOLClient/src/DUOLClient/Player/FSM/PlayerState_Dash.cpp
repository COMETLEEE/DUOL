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
		_player->AddEventFunction(TEXT("StartDash"), std::bind(&DUOLClient::PlayerState_Dash::StartDash, this));

		_player->AddEventFunction(TEXT("EndDash"), std::bind(&DUOLClient::PlayerState_Dash::EndDash, this));
	}

	PlayerState_Dash::~PlayerState_Dash()
	{
		
	}

	void PlayerState_Dash::StartDash()
	{
		// 대쉬 소리 ?
	}

	void PlayerState_Dash::EndDash()
	{
		// 대쉬 끝. 다른 스테이트로 전환됩니다.
		if (MoveCheck() && RunCheck())
			_stateMachine->TransitionTo(TEXT("PlayerState_Run"), 0.f);
		else if (MoveCheck())
			_stateMachine->TransitionTo(TEXT("PlayerState_Move"), 0.f);
		else
			_stateMachine->TransitionTo(TEXT("PlayerState_Idle"), 0.f);
	}

	void PlayerState_Dash::OnStateEnter(float deltaTime)
	{
		PlayerStateBase::OnStateEnter(deltaTime);

		_animator->SetBool(TEXT("IsDash"), true);
		
		// Idle
		if (_stateMachine->GetPrevState()->GetName() == TEXT("PlayerState_Idle"))
		{
			// 그냥 앞에 보고 대쉬한다
		}
		// Move, Attack
		else if (_stateMachine->GetPrevState()->GetName() == TEXT("PlayerState_Attack"))
		{
			// 방향키 입력이 있다면, 대쉬 방향을 정한다.
			if (MoveCheck())
			{
				LookDirectionUpdate();

				_transform->LookAt(_transform->GetWorldPosition() + _desiredLook * 10.f);
			}

			// 아니면 그냥 앞 보고 대쉬
		}

		_player->_isDash = true;
	}

	void PlayerState_Dash::OnStateExit(float deltaTime)
	{
		PlayerStateBase::OnStateExit(deltaTime);

		// 대쉬 상태 끝 !
		_player->_isDash = false;

		_animator->SetBool(TEXT("IsDash"), false);
	}
}