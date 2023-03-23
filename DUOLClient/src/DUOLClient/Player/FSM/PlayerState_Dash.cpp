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
		// ������ ?
		_isEndDash = false;
	}

	void PlayerState_Dash::OnEndDash()
	{
		// ������
		_isEndDash = true;
	}

	void PlayerState_Dash::OnStateEnter(float deltaTime)
	{
		PlayerStateBase::OnStateEnter(deltaTime);

		_animator->SetBool(TEXT("IsDash"), true);

		// ���� ���� ����Ű Ȯ���Ѵ�. �ϴ�, ����Ű ������ AddForce ..!
		// TODO : Root Motion ���� ó���� ���� ..!
		LookDirectionUpdate();

		_rigidbody->AddForce(_desiredLook * DASH_FORCE);

		// TODO : ���� �÷��̾� �ٶ󺸴� ���� ����, �Է��� ���� ���͸� ����ؼ� �ش��ϴ� 8���� �ִϸ��̼� �� �ϳ��� ����Ѵ�.
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
			// �ٷ� �̵� Ʈ�����ǵ� �߰��ұ�� ?
		}
	}

	void PlayerState_Dash::OnStateExit(float deltaTime)
	{
		PlayerStateBase::OnStateExit(deltaTime);

		_player->_isDash = false;

		_animator->SetBool(TEXT("IsDash"), false);
	}
}