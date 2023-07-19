#include "DUOLClient/Player/FSM/PlayerState_Move.h"

#include "DUOLClient/Manager/SystemManager.h"
#include "DUOLGameEngine/ECS/Component/Transform.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"

#include "DUOLClient/Player/FSM/PlayerState_Overdrive.h"
#include "DUOLCommon/Log/LogHelper.h"

namespace DUOLClient
{
	PlayerState_Move::PlayerState_Move(DUOLClient::Player* player) :
		PlayerStateBase(TEXT("PlayerState_Move"), player)
	{
		_player->AddEventFunction(TEXT("LeftFootStep"), std::bind(&DUOLClient::PlayerState_Move::LeftFootStep, this));
		_player->AddEventFunction(TEXT("RightFootStep"), std::bind(&DUOLClient::PlayerState_Move::RightFootStep, this));
	}

	PlayerState_Move::~PlayerState_Move()
	{
	}

	void PlayerState_Move::OnNormalStateStayFixed(float fixedTimeStep)
	{
		if (DashCheck())
		{
			_stateMachine->TransitionTo(TEXT("PlayerState_Dash"), fixedTimeStep);
		}
		else if (EnterOverdriveSwordCheck())
		{
			DUOLClient::SystemManager::GetInstance()->PlayerCameraAction("Player_Overdrive_Sword_Final", _player->GetTransform());

			_stateMachine->TransitionTo(TEXT("PlayerState_Overdrive"), fixedTimeStep);

			auto overdrive = reinterpret_cast<DUOLClient::PlayerState_Overdrive*>(_stateMachine->GetCurrentState());

			overdrive->EnterOverdriveSword();
		}
		else if (EnterOverdriveFistCheck())
		{
			DUOLClient::SystemManager::GetInstance()->PlayerCameraAction("Player_Overdrive_Fist_Final", _player->GetTransform());

			_stateMachine->TransitionTo(TEXT("PlayerState_Overdrive"), fixedTimeStep);

			auto overdrive = reinterpret_cast<DUOLClient::PlayerState_Overdrive*>(_stateMachine->GetCurrentState());

			overdrive->EnterOverdriveFist();
		}
		else if (SwordAttackCheck() || FistAttackCheck())
		{
			_stateMachine->TransitionTo(TEXT("PlayerState_Attack"), fixedTimeStep);
		}
		else if (RunCheck())
		{
			_stateMachine->TransitionTo(TEXT("PlayerState_Run"), fixedTimeStep);
		}
		else if (MoveCheck())
		{
			// Lock on state 움직임 통제
			if (_player->_isLockOnMode)
			{
				DUOLMath::Vector3 lockOnYZero = _player->_lockOnTargetTransform->GetWorldPosition();

				lockOnYZero.y = 0;

				_transform->LookAt(lockOnYZero, DUOLMath::Vector3::Up);

				DUOLMath::Vector3 moveVelocity = _desiredLook * std::lerp(_player->_currentMoveSpeed, _player->_defaultMaxLockOnMoveSpeed, _moveSpeedSmoothness * fixedTimeStep);

				_player->_currentMoveSpeed = moveVelocity.Length();

				if (SlopeCheck())
					moveVelocity = moveVelocity.Projection(_slopeHit._hitNormal);
				else
					moveVelocity.y = _gravity;

				_rigidbody->SetLinearVelocity(moveVelocity);
			}
			else
			{
				_transform->LookAt(_transform->GetWorldPosition() + _desiredLook * 10.f, DUOLMath::Vector3::Up);

				DUOLMath::Vector3 moveVelocity = _desiredLook * std::lerp(_player->_currentMoveSpeed, _player->_defaultMaxMoveSpeed, _moveSpeedSmoothness * fixedTimeStep);

				_player->_currentMoveSpeed = moveVelocity.Length();

				if (SlopeCheck())
					moveVelocity = moveVelocity.Projection(_slopeHit._hitNormal);
				else
					moveVelocity.y = _gravity;

				_rigidbody->SetLinearVelocity(moveVelocity);
			}
		}
		// 아무 입력이 없다.
		else
		{
			// 아직 속도가 남아 있다면
			if (_player->_currentMoveSpeed >= 0.5f)
			{
				// 현재 방향에서 
				DUOLMath::Vector3 moveVelocity = _transform->GetLook() * std::lerp(_player->_currentMoveSpeed, 0.f, _moveSpeedSmoothness * fixedTimeStep);

				_player->_currentMoveSpeed = moveVelocity.Length();

				if (SlopeCheck())
					moveVelocity = moveVelocity.Projection(_slopeHit._hitNormal);
				else
					moveVelocity.y = _gravity;

				_rigidbody->SetLinearVelocity(moveVelocity);
			}
			else
			{
				_rigidbody->SetLinearVelocity(DUOLMath::Vector3::Zero);

				_player->_currentMoveSpeed = 0.f;

				_stateMachine->TransitionTo(TEXT("PlayerState_Idle"), fixedTimeStep);
			}
		}
	}

	void PlayerState_Move::OnOverdriveStateStayFixed(float fixedTimeStep)
	{
		if (DashCheck())
		{
			_stateMachine->TransitionTo(TEXT("PlayerState_Dash"), fixedTimeStep);
		}
		else if ((InOverdriveSwordCheck() && SwordAttackCheck()) || (InOverdriveFistCheck() && FistAttackCheck()))
		{
			_stateMachine->TransitionTo(TEXT("PlayerState_Attack"), fixedTimeStep);
		}
		else if (RunCheck())
		{
			_stateMachine->TransitionTo(TEXT("PlayerState_Run"), fixedTimeStep);
		}
		else if (UltimateCheck())
		{
			if (_player->_isOverdriveSwordMode)
				DUOLClient::SystemManager::GetInstance()->PlayerCameraAction("Player_Sword_Final", _player->GetTransform());

			else if (_player->_isOverdriveFistMode)
				DUOLClient::SystemManager::GetInstance()->PlayerCameraAction("Player_Ultimate_Fist_Final", _player->GetTransform());

			_stateMachine->TransitionTo(TEXT("PlayerState_Ultimate"), fixedTimeStep);

			DUOL_TRACE(DUOL_CONSOLE, "Move | UltStart ()");
		}
		else if (MoveCheck())
		{
			// Lock on state 움직임 통제
			if (_player->_isLockOnMode)
			{
				DUOLMath::Vector3 lockOnYZero = _player->_lockOnTargetTransform->GetWorldPosition();

				lockOnYZero.y = 0;

				_transform->LookAt(lockOnYZero, DUOLMath::Vector3::Up);

				DUOLMath::Vector3 moveVelocity = _desiredLook * std::lerp(_player->_currentMoveSpeed, _player->_defaultMaxLockOnMoveSpeed, _moveSpeedSmoothness * fixedTimeStep);

				_player->_currentMoveSpeed = moveVelocity.Length();

				if (SlopeCheck())
					moveVelocity = moveVelocity.Projection(_slopeHit._hitNormal);
				else
					moveVelocity.y = _gravity;

				_rigidbody->SetLinearVelocity(moveVelocity);
			}
			else
			{
				_transform->LookAt(_transform->GetWorldPosition() + _desiredLook * 10.f, DUOLMath::Vector3::Up);

				DUOLMath::Vector3 moveVelocity = _desiredLook * std::lerp(_player->_currentMoveSpeed, _player->_defaultMaxMoveSpeed, _moveSpeedSmoothness * fixedTimeStep);

				_player->_currentMoveSpeed = moveVelocity.Length();

				if (SlopeCheck())
					moveVelocity = moveVelocity.Projection(_slopeHit._hitNormal);
				else
					moveVelocity.y = _gravity;

				_rigidbody->SetLinearVelocity(moveVelocity);
			}
		}
		// 아무 입력이 없다.
		else
		{
			// 아직 속도가 남아 있다면
			if (_player->_currentMoveSpeed >= 0.5f)
			{
				// 현재 방향에서 
				DUOLMath::Vector3 moveVelocity = _transform->GetLook() * std::lerp(_player->_currentMoveSpeed, 0.f, _moveSpeedSmoothness * fixedTimeStep);

				_player->_currentMoveSpeed = moveVelocity.Length();

				if (SlopeCheck())
					moveVelocity = moveVelocity.Projection(_slopeHit._hitNormal);
				else
					moveVelocity.y = _gravity;

				_rigidbody->SetLinearVelocity(moveVelocity);
			}
			else
			{
				_rigidbody->SetLinearVelocity(DUOLMath::Vector3::Zero);

				_player->_currentMoveSpeed = 0.f;

				_stateMachine->TransitionTo(TEXT("PlayerState_Idle"), fixedTimeStep);
			}
		}
	}

	void PlayerState_Move::LeftFootStep()
	{
		_player->PlaySoundClip(PlayerSoundTable::FootStep01, false);
	}

	void PlayerState_Move::RightFootStep()
	{
		_player->PlaySoundClip(PlayerSoundTable::FootStep02, false);
	}

	void PlayerState_Move::OnStateEnter(float deltaTime)
	{
		StateBase::OnStateEnter(deltaTime);

		_animator->SetBool(TEXT("IsMove"), true);
	}

	void PlayerState_Move::OnStateStayFixed(float fixedTimeStep)
	{
		PlayerStateBase::OnStateStayFixed(fixedTimeStep);

		LookDirectionUpdate();

		CalculateGravity(fixedTimeStep);

		InOverdriveCheck()
			? OnOverdriveStateStayFixed(fixedTimeStep)
			: OnNormalStateStayFixed(fixedTimeStep);
	}

	void PlayerState_Move::OnStateStay(float deltaTime)
	{
		PlayerStateBase::OnStateStay(deltaTime);

		if (LockOnCheck())
			FindLockOnTarget();
	}

	void PlayerState_Move::OnStateExit(float deltaTime)
	{
		StateBase::OnStateExit(deltaTime);

		// TODO : 일단 속도 0으로 만들어주자 ..!
		if (_stateMachine->GetNextState()->GetName() != TEXT("PlayerState_Run"))
		{
			_rigidbody->SetLinearVelocity(DUOLMath::Vector3::Zero);

			_player->_currentMoveSpeed = 0.f;
		}
		
		// 애니메이터 파라미터 정리
		_animator->SetBool(TEXT("IsMove"), false);
	}
}
