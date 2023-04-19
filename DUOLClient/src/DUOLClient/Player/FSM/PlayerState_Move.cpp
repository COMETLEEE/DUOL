#include "DUOLClient/Player/FSM/PlayerState_Move.h"

#include "DUOLGameEngine/ECS/Component/Transform.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"

namespace DUOLClient
{
	PlayerState_Move::PlayerState_Move(DUOLClient::Player* player) :
		PlayerStateBase(TEXT("PlayerState_Move"), player)
	{
	}

	PlayerState_Move::~PlayerState_Move()
	{
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

		if (LockOnCheck())
		{
			FindLockOnTarget();
		}

		if (DashCheck())
		{
			_stateMachine->TransitionTo(TEXT("PlayerState_Dash"), fixedTimeStep);
		}
		else if (SwordAttackCheck() || FistAttackCheck())
		{
			_transform->LookAt(_transform->GetWorldPosition() + _desiredLook * 10.f);

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

				DUOLMath::Vector3 moveVelocity = _desiredLook * std::lerp(_player->_currentMoveSpeed, _player->_defaultMaxLockOnMoveSpeed, _moveSpeedSmoothnesss * fixedTimeStep);

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

				DUOLMath::Vector3 moveVelocity = _desiredLook * std::lerp(_player->_currentMoveSpeed, _player->_defaultMaxMoveSpeed, _moveSpeedSmoothnesss * fixedTimeStep);

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
				DUOLMath::Vector3 moveVelocity = _transform->GetLook() * std::lerp(_player->_currentMoveSpeed, 0.f, _moveSpeedSmoothnesss * fixedTimeStep);

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

	void PlayerState_Move::OnStateStay(float deltaTime)
	{
		//PlayerStateBase::OnStateStay(deltaTime);

		//LookDirectionUpdate();

		//if (LockOnCheck())
		//{
		//	FindLockOnTarget();
		//}

		//if (DashCheck())
		//{
		//	_stateMachine->TransitionTo(TEXT("PlayerState_Dash"), deltaTime);
		//}
		//else if (SwordAttackCheck() || FistAttackCheck())
		//{
		//	_transform->LookAt(_transform->GetWorldPosition() + _desiredLook * 10.f);

		//	_stateMachine->TransitionTo(TEXT("PlayerState_Attack"), deltaTime);
		//}
		//else if (RunCheck())
		//{
		//	_stateMachine->TransitionTo(TEXT("PlayerState_Run"), deltaTime);
		//}
		//else if (MoveCheck())
		//{
		//	// Lock on state 움직임 통제
		//	if (_player->_isLockOnMode)
		//	{
		//		DUOLMath::Vector3 lockOnYZero = _player->_lockOnTargetTransform->GetWorldPosition();

		//		lockOnYZero.y = 0;

		//		_transform->LookAt(lockOnYZero, DUOLMath::Vector3::Up);

		//		DUOLMath::Vector3 moveVelocity = _desiredLook * std::lerp(_player->_currentMoveSpeed, _player->_defaultMaxLockOnMoveSpeed, _moveSpeedSmoothnesss * deltaTime);

		//		bool isOnSlope = SlopeCheck();

		//		if (isOnSlope)
		//		{
		//			moveVelocity = moveVelocity.Projection(_slopeHit._hitNormal) * moveVelocity.Length();
		//		}

		//		_player->_currentMoveSpeed = moveVelocity.Length();

		//		_player->_playerTransform->SetPosition(_player->_playerTransform->GetWorldPosition() + moveVelocity * deltaTime);
		//	}
		//	else
		//	{
		//		_transform->LookAt(_transform->GetWorldPosition() + _desiredLook * 10.f, DUOLMath::Vector3::Up);

		//		DUOLMath::Vector3 moveVelocity = _desiredLook * std::lerp(_player->_currentMoveSpeed, _player->_defaultMaxMoveSpeed, _moveSpeedSmoothnesss * deltaTime);

		//		bool isOnSlope = SlopeCheck();

		//		if (isOnSlope)
		//		{
		//			moveVelocity = moveVelocity.Projection(_slopeHit._hitNormal) * moveVelocity.Length();
		//		}

		//		_player->_currentMoveSpeed = moveVelocity.Length();

		//		_player->_playerTransform->SetPosition(_player->_playerTransform->GetWorldPosition() + moveVelocity * deltaTime);
		//	}
		//}
		//// 아무 입력이 없다.
		//else
		//{
		//	// 아직 속도가 남아 있다면
		//	if (_player->_currentMoveSpeed >= 0.5f)
		//	{
		//		// 현재 방향에서 
		//		DUOLMath::Vector3 moveVelocity = _transform->GetLook() * std::lerp(_player->_currentMoveSpeed, 0.f, _moveSpeedSmoothnesss * deltaTime);

		//		_player->_currentMoveSpeed = moveVelocity.Length();

		//		_player->_playerTransform->SetPosition(_player->_playerTransform->GetWorldPosition() + moveVelocity * deltaTime);
		//	}
		//	else
		//	{
		//		_player->_currentMoveSpeed = 0.f;

		//		_stateMachine->TransitionTo(TEXT("PlayerState_Idle"), deltaTime);
		//	}
		//}
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