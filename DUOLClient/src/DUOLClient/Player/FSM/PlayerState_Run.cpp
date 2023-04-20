#include "DUOLClient/Player/FSM/PlayerState_Run.h"

#include "DUOLGameEngine/ECS/Component/Animator.h"
#include "DUOLGameEngine/ECS/Component/Transform.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"

namespace DUOLClient
{
	PlayerState_Run::PlayerState_Run(DUOLClient::Player* player) :
		PlayerStateBase(TEXT("PlayerState_Run"), player)
	{
	}

	PlayerState_Run::~PlayerState_Run()
	{
	}

	void PlayerState_Run::OnStateEnter(float deltaTime)
	{
		PlayerStateBase::OnStateEnter(deltaTime);

		// �ִϸ��̼� �Ķ���� �¾�
		_animator->SetBool(TEXT("IsMove"), true);
		_animator->SetBool(TEXT("IsRun"), true);
	}

	void PlayerState_Run::OnStateStayFixed(float fixedTimeStep)
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
		else if ((_player->_isOverdriveSwordMode && SwordAttackCheck()) || (_player->_isOverdriveFistMode && FistAttackCheck())
			|| (!_player->_isOverdriveFistMode && !_player->_isOverdriveSwordMode && (SwordAttackCheck() || FistAttackCheck())))
		{
			_stateMachine->TransitionTo(TEXT("PlayerState_Attack"), fixedTimeStep);
		}
		else if (!RunCheck() && MoveCheck())
		{
			_stateMachine->TransitionTo(TEXT("PlayerState_Move"), fixedTimeStep);
		}
		else if (MoveCheck())
		{
			// Lock on state ������ ����
			if (_player->_isLockOnMode)
			{
				DUOLMath::Vector3 lockOnYZero = _player->_lockOnTargetTransform->GetWorldPosition();

				lockOnYZero.y = 0;

				_transform->LookAt(lockOnYZero);

				DUOLMath::Vector3 moveVelocity = _desiredLook * std::lerp(_player->_currentMoveSpeed, _player->_defaultMaxLockOnRunSpeed, _runSpeedSmoothness * fixedTimeStep);

				_player->_currentMoveSpeed = moveVelocity.Length();

				if (SlopeCheck())
					moveVelocity = moveVelocity.Projection(_slopeHit._hitNormal);
				else
					moveVelocity.y = _gravity;

				_rigidbody->SetLinearVelocity(moveVelocity);

			}
			else
			{
				_transform->LookAt(_transform->GetWorldPosition() + _desiredLook * 10.f);

				DUOLMath::Vector3 moveVelocity = _desiredLook * std::lerp(_player->_currentMoveSpeed, _player->_defaultMaxRunSpeed, _runSpeedSmoothness * fixedTimeStep);

				_player->_currentMoveSpeed = moveVelocity.Length();

				if (SlopeCheck())
					moveVelocity = moveVelocity.Projection(_slopeHit._hitNormal);
				else
					moveVelocity.y = _gravity;

				_rigidbody->SetLinearVelocity(moveVelocity);
			}
		}
		// �ƹ� �Է��� ����.
		else
		{
			// ���� �ӵ��� ���� �ִٸ�
			if (_player->_currentMoveSpeed >= 0.5f)
			{
				DUOLMath::Vector3 moveVelocity = _transform->GetLook() * std::lerp(_player->_currentMoveSpeed, 0.f, _runSpeedSmoothness * fixedTimeStep);

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

	void PlayerState_Run::OnStateStay(float deltaTime)
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
		//	_stateMachine->TransitionTo(TEXT("PlayerState_Attack"), deltaTime);
		//}
		//else if (!RunCheck() && MoveCheck())
		//{
		//	_stateMachine->TransitionTo(TEXT("PlayerState_Move"), deltaTime);
		//}
		//else if (MoveCheck())
		//{
		//	// Lock on state ������ ����
		//	if (_player->_isLockOnMode)
		//	{
		//		DUOLMath::Vector3 lockOnYZero = _player->_lockOnTargetTransform->GetWorldPosition();

		//		lockOnYZero.y = 0;

		//		_transform->LookAt(lockOnYZero);

		//		DUOLMath::Vector3 moveVelocity = _desiredLook * std::lerp(_player->_currentMoveSpeed, _player->_defaultMaxLockOnRunSpeed, _runSpeedSmoothness * deltaTime);

		//		_player->_currentMoveSpeed = moveVelocity.Length();

		//		_player->_playerTransform->SetPosition(_player->_playerTransform->GetWorldPosition() + moveVelocity * deltaTime);
		//	}
		//	else
		//	{
		//		_transform->LookAt(_transform->GetWorldPosition() + _desiredLook * 10.f);

		//		DUOLMath::Vector3 moveVelocity = _desiredLook * std::lerp(_player->_currentMoveSpeed, _player->_defaultMaxRunSpeed, _runSpeedSmoothness * deltaTime);

		//		_player->_currentMoveSpeed = moveVelocity.Length();

		//		_player->_playerTransform->SetPosition(_player->_playerTransform->GetWorldPosition() + moveVelocity * deltaTime);
		//	}
		//}
		//// �ƹ� �Է��� ����.
		//else
		//{
		//	// ���� �ӵ��� ���� �ִٸ�
		//	if (_player->_currentMoveSpeed >= 0.5f)
		//	{
		//		// ���� ���⿡�� �ӵ�
		//		DUOLMath::Vector3 moveVelocity = _transform->GetLook() * std::lerp(_player->_currentMoveSpeed, 0.f, _runSpeedSmoothness * deltaTime);

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

	void PlayerState_Run::OnStateExit(float deltaTime)
	{
		PlayerStateBase::OnStateExit(deltaTime);

		// TODO : �ϴ� �ӵ� 0���� ��������� ..! ���� ������Ʈ�� �̵� ������ �ƴ϶�� !
		if (_stateMachine->GetNextState()->GetName() != TEXT("PlayerState_Move"))
		{
			_rigidbody->SetLinearVelocity(DUOLMath::Vector3::Zero);

			_player->_currentMoveSpeed = 0.f;
		}

		_animator->SetBool(TEXT("IsRun"), false);
		_animator->SetBool(TEXT("IsMove"), false);
	}
}