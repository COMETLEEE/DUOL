#include "DUOLClient/Player/FSM/PlayerState_Run.h"

#include "DUOLGameEngine/ECS/Component/Animator.h"
#include "DUOLGameEngine/ECS/Component/Transform.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"

namespace DUOLClient
{
	PlayerState_Run::PlayerState_Run() :
		PlayerStateBase(TEXT("PlayerState_Run"))
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

	void PlayerState_Run::OnStateStay(float deltaTime)
	{
		PlayerStateBase::OnStateStay(deltaTime);

		LookDirectionUpdate();

		if (LockOnCheck())
		{
			FindLockOnTarget();
		}

		if (DieCheck())
		{
			_stateMachine->TransitionTo(TEXT("PlayerState_Die"), deltaTime);
		}
		else if (AttackCheck())
		{
			_stateMachine->TransitionTo(TEXT("PlayerState_Attack"), deltaTime);
		}
		else if (!RunCheck())
		{
			_stateMachine->TransitionTo(TEXT("PlayerState_Move"), deltaTime);
		}
		else if (MoveCheck())
		{
			// Lock on state ������ ����
			if (_player->_isLockOnMode)
			{
				_transform->LookAt(_player->_lockOnTargetTransform);

				DUOLMath::Vector3 moveVelocity = _desiredLook * std::lerp(_player->_currentMoveSpeed, _player->_defaultMaxLockOnRunSpeed, _runSpeedSmoothness * deltaTime);

				_rigidbody->SetLinearVelocity(moveVelocity);

				_player->_currentMoveSpeed = moveVelocity.Length();
			}
			else
			{
				_transform->LookAt(_transform->GetWorldPosition() + _desiredLook);

				DUOLMath::Vector3 moveVelocity = _desiredLook * std::lerp(_player->_currentMoveSpeed, _player->_defaultMaxRunSpeed, _runSpeedSmoothness * deltaTime);

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
				// ���� ���⿡�� �ӵ�
				DUOLMath::Vector3 moveVelocity = _transform->GetLook() * std::lerp(_player->_currentMoveSpeed, 0.f, _runSpeedSmoothness * deltaTime);

				_rigidbody->SetLinearVelocity(moveVelocity);

				_player->_currentMoveSpeed = moveVelocity.Length();
			}
			else
			{
				_rigidbody->SetLinearVelocity(DUOLMath::Vector3::Zero);

				_player->_currentMoveSpeed = 0.f;

				_stateMachine->TransitionTo(TEXT("PlayerState_Idle"), deltaTime);
			}
		}

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