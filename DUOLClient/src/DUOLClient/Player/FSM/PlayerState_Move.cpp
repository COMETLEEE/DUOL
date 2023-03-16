#include "DUOLClient/Player/FSM/PlayerState_Move.h"

#include "DUOLGameEngine/ECS/Component/Transform.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"

namespace DUOLClient
{
	PlayerState_Move::PlayerState_Move() :
		PlayerStateBase(TEXT("PlayerState_Move"))
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

	void PlayerState_Move::OnStateStay(float deltaTime)
	{
		StateBase::OnStateStay(deltaTime);

		LookDirectionUpdate();

		if (AttackCheck())
		{
			_transform->LookAt(_transform->GetWorldPosition() + _desiredLook);

			_stateMachine->TransitionTo(TEXT("PlayerState_Attack"), deltaTime);
		}
		else if (MoveCheck())
		{
			_transform->LookAt(_transform->GetWorldPosition() + _desiredLook);

			// Velocity Up ..!
			if (DashCheck())
			{
				_animator->SetBool(TEXT("IsDash"), true);

				DUOLMath::Vector3 moveVelocity = _desiredLook * std::lerp(_player->_currentPlayerProperty._moveSpeed, _player->_currentPlayerProperty._maxDashSpeed, _moveSpeedSmoothnesss * deltaTime);

				 _rigidbody->SetLinearVelocity(moveVelocity);

				 _player->_currentPlayerProperty._moveSpeed = _rigidbody->GetLinearVelocity().Length();

				// _transform->Translate(moveVelocity * deltaTime, DUOLGameEngine::Space::World);

				_player->_currentPlayerProperty._moveSpeed = moveVelocity.Length();

			}
			else
			{
				_animator->SetBool(TEXT("IsDash"), false);

				DUOLMath::Vector3 moveVelocity = _desiredLook * std::lerp(_player->_currentPlayerProperty._moveSpeed, _player->_currentPlayerProperty._maxMoveSpeed, _moveSpeedSmoothnesss * deltaTime);

				_rigidbody->SetLinearVelocity(moveVelocity);

				_player->_currentPlayerProperty._moveSpeed = _rigidbody->GetLinearVelocity().Length();

				// _transform->Translate(moveVelocity * deltaTime, DUOLGameEngine::Space::World);

				_player->_currentPlayerProperty._moveSpeed = moveVelocity.Length();
			}
		}
		// 아무 입력이 없다.
		else
		{
			// 아직 속도가 남아 있다면
			if (_player->_currentPlayerProperty._moveSpeed >= 0.5f)
			{
				// 현재 방향에서 
				DUOLMath::Vector3 moveVelocity = _transform->GetLook() * std::lerp(_player->_currentPlayerProperty._moveSpeed, 0.f, _moveSpeedSmoothnesss * deltaTime);

				_rigidbody->SetLinearVelocity(moveVelocity);

				_player->_currentPlayerProperty._moveSpeed = _rigidbody->GetLinearVelocity().Length();

				// _transform->Translate(moveVelocity * deltaTime, DUOLGameEngine::Space::World);

				_player->_currentPlayerProperty._moveSpeed = moveVelocity.Length();
			}
			else
			{
				_rigidbody->SetLinearVelocity(DUOLMath::Vector3::Zero);

				_stateMachine->TransitionTo(TEXT("PlayerState_Idle"), deltaTime);
			}
		}
	}

	void PlayerState_Move::OnStateExit(float deltaTime)
	{
		StateBase::OnStateExit(deltaTime);

		// 애니메이터 파라미터 정리
		_animator->SetBool(TEXT("IsMove"), false);

		_animator->SetBool(TEXT("IsDash"), false);
	}
}