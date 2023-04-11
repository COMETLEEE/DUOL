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

		if (LockOnCheck())
		{
			FindLockOnTarget();
		}

		if (DieCheck())
		{
			// ����
			_stateMachine->TransitionTo(TEXT("PlayerState_Die"), fixedTimeStep);
		}
		else if (DashCheck())
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
			// Lock on state ������ ����
			if (_player->_isLockOnMode)
			{
				DUOLMath::Vector3 lockOnYZero = _player->_lockOnTargetTransform->GetWorldPosition();

				lockOnYZero.y = 0;

				_transform->LookAt(lockOnYZero, DUOLMath::Vector3::Up);

				DUOLMath::Vector3 moveVelocity = _desiredLook * std::lerp(_player->_currentMoveSpeed, _player->_defaultMaxLockOnMoveSpeed, _moveSpeedSmoothnesss * fixedTimeStep);

				_rigidbody->SetLinearVelocity(moveVelocity);

				_player->_currentMoveSpeed = moveVelocity.Length();
			}
			else
			{
				_transform->LookAt(_transform->GetWorldPosition() + _desiredLook * 10.f, DUOLMath::Vector3::Up);

				DUOLMath::Vector3 moveVelocity = _desiredLook * std::lerp(_player->_currentMoveSpeed, _player->_defaultMaxMoveSpeed, _moveSpeedSmoothnesss * fixedTimeStep);

				_rigidbody->SetLinearVelocity(moveVelocity);

				_player->_currentMoveSpeed = moveVelocity.Length();
			}
		}
		// �ƹ� �Է��� ����.
		else
		{
			// ���� �ӵ��� ���� �ִٸ�
			if (_player->_currentMoveSpeed >= 0.5f)
			{
				// ���� ���⿡�� 
				DUOLMath::Vector3 moveVelocity = _transform->GetLook() * std::lerp(_player->_currentMoveSpeed, 0.f, _moveSpeedSmoothnesss * fixedTimeStep);

				_rigidbody->SetLinearVelocity(moveVelocity);

				_player->_currentMoveSpeed = moveVelocity.Length();
			}
			else
			{
				_rigidbody->SetLinearVelocity(DUOLMath::Vector3::Zero);

				_player->_currentMoveSpeed = 0.f;
				
				_stateMachine->TransitionTo(TEXT("PlayerState_Idle"), fixedTimeStep);
			}
		}
	}

	void PlayerState_Move::OnStateExit(float deltaTime)
	{
		StateBase::OnStateExit(deltaTime);

		// TODO : �ϴ� �ӵ� 0���� ��������� ..!
		if (_stateMachine->GetNextState()->GetName() != TEXT("PlayerState_Run"))
		{
			_rigidbody->SetLinearVelocity(DUOLMath::Vector3::Zero);
			_player->_currentMoveSpeed = 0.f;
		}
		
		// �ִϸ����� �Ķ���� ����
		_animator->SetBool(TEXT("IsMove"), false);
	}
}