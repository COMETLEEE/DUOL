#include "DUOLClient/Player/FSM/PlayerState_Dash.h"

#include "DUOLCommon/Log/LogHelper.h"
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
		// ���� ���� �� �뽬�� �������μ� ���� ������Ʈ���� ��ȯ üũ��
		_isEndDash = false;
	}

	void PlayerState_Dash::OnEndDash()
	{
		// ���� ���� �� �뽬�� �������μ� ���� ������Ʈ���� ��ȯ üũ��
		_isEndDash = true;
	}

	void PlayerState_Dash::OnStateEnter(float deltaTime)
	{
		PlayerStateBase::OnStateEnter(deltaTime);

		_animator->SetBool(TEXT("IsDash"), true);

		// Lock on mode
		if (_player->_isLockOnMode)
		{
			// TODO : ���� ���⿡ ���ؼ� 8���� �뽬���� ���̸� �����մϴ�.
			// 
		}
		// Idle
		else if (_stateMachine->GetPrevState()->GetName() == TEXT("PlayerState_Idle"))
		{
			// ���� �ٶ󺸰� �ִ� ���� �״�� ����.
		}
		// Move, Attack
		else
		{
			// ������ �ִ� ����Ű�� ����.
			LookDirectionUpdate();

			DUOL_INFO(DUOL_CONSOLE, "Dash : Look direction initialized.");

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

		if (_isEndDash && MoveCheck())
		{
			// �ٷ� �̵� Ʈ�����ǵ� �߰��ұ�� ?
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