#include "DUOLClient/Player/FSM/PlayerState_Idle.h"

#include "DUOLClient/Player/FSM/PlayerState_Overdrive.h"
#include "DUOLGameEngine/ECS/Component/Transform.h"
#include "DUOLGameEngine/Manager/InputManager.h"
#include "DUOLMath/DUOLMath.h"

namespace DUOLClient
{
	PlayerState_Idle::PlayerState_Idle(DUOLClient::Player* player) :
		PlayerStateBase(TEXT("PlayerState_Idle"), player)
	{
	}

	PlayerState_Idle::~PlayerState_Idle()
	{
	}

	void PlayerState_Idle::OnStateEnter(float deltaTime)
	{
		StateBase::OnStateEnter(deltaTime);
	}

	void PlayerState_Idle::OnStateStay(float deltaTime)
	{
		StateBase::OnStateStay(deltaTime);

		// Update desired look direction.
		LookDirectionUpdate();

		// Look On
		if (LockOnCheck())
		{
			FindLockOnTarget();
		}

		if (DashCheck())
		{
			_stateMachine->TransitionTo(TEXT("PlayerState_Dash"), deltaTime);
		}
		else if (OverdriveSwordCheck())
		{
			_stateMachine->TransitionTo(TEXT("PlayerState_Overdrive"), deltaTime);

			auto overdrive = reinterpret_cast<DUOLClient::PlayerState_Overdrive*>(_stateMachine->GetCurrentState());

			overdrive->EnterOverdriveSword();
		}
		else if (OverdriveFistCheck())
		{
			_stateMachine->TransitionTo(TEXT("PlayerState_Overdrive"), deltaTime);

			auto overdrive = reinterpret_cast<DUOLClient::PlayerState_Overdrive*>(_stateMachine->GetCurrentState());

			overdrive->EnterOverdriveFist();
		}
		else if ((_player->_isOverdriveSwordMode && SwordAttackCheck()) || (_player->_isOverdriveFistMode && FistAttackCheck())
			|| (!_player->_isOverdriveFistMode && !_player->_isOverdriveSwordMode && (SwordAttackCheck() || FistAttackCheck())))
		{
			_stateMachine->TransitionTo(TEXT("PlayerState_Attack"), deltaTime);
		}
		else if (MoveCheck())
		{
			if (RunCheck())
				_stateMachine->TransitionTo(TEXT("PlayerState_Run"), deltaTime);
			else
				_stateMachine->TransitionTo(TEXT("PlayerState_Move"), deltaTime);
		}
	}

	void PlayerState_Idle::OnStateExit(float deltaTime)
	{
		StateBase::OnStateExit(deltaTime);

		// Idle State는 끝날 때 할 일 별로 없습니다.
	}
}