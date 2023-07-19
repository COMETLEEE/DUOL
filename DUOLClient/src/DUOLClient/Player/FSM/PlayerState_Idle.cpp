#include "DUOLClient/Player/FSM/PlayerState_Idle.h"

#include "DUOLClient/Player/FSM/PlayerState_Overdrive.h"
#include "DUOLCommon/Log/LogHelper.h"
#include "DUOLGameEngine/ECS/Component/Transform.h"
#include "DUOLGameEngine/Manager/InputManager.h"
#include "DUOLClient/Manager/SystemManager.h"
#include "DUOLMath/DUOLMath.h"

namespace DUOLClient
{
	PlayerState_Idle::PlayerState_Idle(DUOLClient::Player* player) :
		PlayerStateBase(TEXT("PlayerState_Idle"), player)
		, _isReservedEndOverdrive(false)
	{
	}

	PlayerState_Idle::~PlayerState_Idle()
	{
	}

	void PlayerState_Idle::OnNormalStateStay(float deltaTime)
	{
		if (DashCheck())
		{
			_stateMachine->TransitionTo(TEXT("PlayerState_Dash"), deltaTime);
		}
		else if (EnterOverdriveSwordCheck())
		{
			// TODO : Overdrive Start
			DUOLClient::SystemManager::GetInstance()->PlayerCameraAction("Player_Overdrive_Sword_Final", _player->GetTransform());

			_stateMachine->TransitionTo(TEXT("PlayerState_Overdrive"), deltaTime);

			auto overdrive = reinterpret_cast<DUOLClient::PlayerState_Overdrive*>(_stateMachine->GetCurrentState());

			overdrive->EnterOverdriveSword();
		}
		else if (EnterOverdriveFistCheck())
		{
			// TODO : Overdrive Start
			DUOLClient::SystemManager::GetInstance()->PlayerCameraAction("Player_Overdrive_Fist_Final", _player->GetTransform());

			_stateMachine->TransitionTo(TEXT("PlayerState_Overdrive"), deltaTime);

			auto overdrive = reinterpret_cast<DUOLClient::PlayerState_Overdrive*>(_stateMachine->GetCurrentState());

			overdrive->EnterOverdriveFist();
		}
		else if (SwordAttackCheck() || FistAttackCheck())
		{
			_stateMachine->TransitionTo(TEXT("PlayerState_Attack"), deltaTime);
		}
		else if (MoveCheck())
		{
			RunCheck()
				? _stateMachine->TransitionTo(TEXT("PlayerState_Run"), deltaTime)
				: _stateMachine->TransitionTo(TEXT("PlayerState_Move"), deltaTime);
		}
	}

	void PlayerState_Idle::OnOverdriveStateStay(float deltaTime)
	{

		if (_isReservedEndOverdrive)
		{
			// 오버 드라이브 전환 상태로 들어갑니다.
			_stateMachine->TransitionTo(TEXT("PlayerState_Overdrive"), deltaTime);

			auto overdrive = reinterpret_cast<DUOLClient::PlayerState_Overdrive*>(_stateMachine->GetCurrentState());

			overdrive->ExitOverdrive();

			_isReservedEndOverdrive = false;

			DUOL_TRACE(DUOL_CONSOLE, "IDLE | ReserveEndOverDrive");
		}
		else if (UltimateCheck())
		{
			// TODO : Ultimate Start
			_stateMachine->TransitionTo(TEXT("PlayerState_Ultimate"), deltaTime);

			DUOL_TRACE(DUOL_CONSOLE, "Idle | UltStart ()");
		}
		else if (DashCheck())
		{
			_stateMachine->TransitionTo(TEXT("PlayerState_Dash"), deltaTime);
		}
		else if ((_player->_isOverdriveSwordMode && SwordAttackCheck()) || (_player->_isOverdriveFistMode && FistAttackCheck()))
		{
			_stateMachine->TransitionTo(TEXT("PlayerState_Attack"), deltaTime);
		}
		else if (MoveCheck())
		{
			RunCheck()
				? _stateMachine->TransitionTo(TEXT("PlayerState_Run"), deltaTime)
				: _stateMachine->TransitionTo(TEXT("PlayerState_Move"), deltaTime);
		}
	}

	void PlayerState_Idle::ReserveEndOverdrive()
	{
		_isReservedEndOverdrive = true;
	}

	void PlayerState_Idle::OnStateEnter(float deltaTime)
	{
		StateBase::OnStateEnter(deltaTime);
	}

	void PlayerState_Idle::OnStateStay(float deltaTime)
	{
		StateBase::OnStateStay(deltaTime);

		if (_player->_canInteraction)
		{
			// Update desired look direction.
			LookDirectionUpdate();

			// Look On
			if (LockOnCheck())
				FindLockOnTarget();

			InOverdriveCheck()
				? OnOverdriveStateStay(deltaTime)
				: OnNormalStateStay(deltaTime);
		}
	}

	void PlayerState_Idle::OnStateExit(float deltaTime)
	{
		StateBase::OnStateExit(deltaTime);
	}
}