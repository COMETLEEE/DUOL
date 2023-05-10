#include "DUOLClient/Player/FSM/PlayerState_Run.h"

#include "DUOLGameEngine/ECS/Component/Animator.h"
#include "DUOLGameEngine/ECS/Component/Transform.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"
#include "DUOLGameEngine/ECS/Component/ParticleRenderer.h"

#include "DUOLClient/Manager/ParticleManager.h"
#include "DUOLClient/Player/FSM/PlayerState_Overdrive.h"
#include "DUOLGameEngine/ECS/GameObject.h"

namespace DUOLClient
{
	PlayerState_Run::PlayerState_Run(DUOLClient::Player* player) :
		PlayerStateBase(TEXT("PlayerState_Run"), player)
	{
	}

	PlayerState_Run::~PlayerState_Run()
	{
	}

	void PlayerState_Run::OnNormalStateStayFixed(float fixedTimeStep)
	{
		if (DashCheck())
		{
			_stateMachine->TransitionTo(TEXT("PlayerState_Dash"), fixedTimeStep);
		}
		else if (EnterOverdriveSwordCheck())
		{
			_stateMachine->TransitionTo(TEXT("PlayerState_Overdrive"), fixedTimeStep);

			auto overdrive = reinterpret_cast<DUOLClient::PlayerState_Overdrive*>(_stateMachine->GetCurrentState());

			overdrive->EnterOverdriveSword();
		}
		else if (EnterOverdriveFistCheck())
		{
			_stateMachine->TransitionTo(TEXT("PlayerState_Overdrive"), fixedTimeStep);

			auto overdrive = reinterpret_cast<DUOLClient::PlayerState_Overdrive*>(_stateMachine->GetCurrentState());

			overdrive->EnterOverdriveFist();
		}
		else if (SwordAttackCheck() || FistAttackCheck())
		{
			_stateMachine->TransitionTo(TEXT("PlayerState_Attack"), fixedTimeStep);
		}
		else if (!RunCheck() && MoveCheck())
		{
			_stateMachine->TransitionTo(TEXT("PlayerState_Move"), fixedTimeStep);
		}
		else if (MoveCheck())
		{
			// Lock on state 움직임 통제
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
		// 아무 입력이 없다.
		else
		{
			// 아직 속도가 남아 있다면
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

	void PlayerState_Run::OnOverdriveStateStayFixed(float fixedTimeStep)
	{
		if (DashCheck())
		{
			_stateMachine->TransitionTo(TEXT("PlayerState_Dash"), fixedTimeStep);
		}
		else if ((InOverdriveSwordCheck() && SwordAttackCheck()) || (InOverdriveFistCheck() && FistAttackCheck()))
		{
			_stateMachine->TransitionTo(TEXT("PlayerState_Attack"), fixedTimeStep);
		}
		else if (!RunCheck() && MoveCheck())
		{
			_stateMachine->TransitionTo(TEXT("PlayerState_Move"), fixedTimeStep);
		}
		else if (MoveCheck())
		{
			// Lock on state 움직임 통제
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
		// 아무 입력이 없다.
		else
		{
			// 아직 속도가 남아 있다면
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

	void PlayerState_Run::OnStateEnter(float deltaTime)
	{
		PlayerStateBase::OnStateEnter(deltaTime);

		// 애니메이션 파라미터 셋업
		_animator->SetBool(TEXT("IsMove"), true);
		_animator->SetBool(TEXT("IsRun"), true);

		// 질주 이펙트
		_particleRenderer = DUOLClient::ParticleManager::GetInstance()->Pop(ParticleEnum::RunShift);

		_particleGameObject = _particleRenderer->GetGameObject();

		_particleGameObject->GetTransform()->SetParent(_player->_cameraTransform, false);
	}

	void PlayerState_Run::OnStateStayFixed(float fixedTimeStep)
	{
		PlayerStateBase::OnStateStayFixed(fixedTimeStep);

		LookDirectionUpdate();

		CalculateGravity(fixedTimeStep);

		InOverdriveCheck()
			? OnOverdriveStateStayFixed(fixedTimeStep)
			: OnNormalStateStayFixed(fixedTimeStep);
	}

	void PlayerState_Run::OnStateStay(float deltaTime)
	{
		if (LockOnCheck())
			FindLockOnTarget();

		// Run shift effect logic.
		if (_particleGameObject != nullptr)
		{
			_particleGameObject->GetTransform()->SetLocalScale(DUOLMath::Vector3(0.1f, 0.1f, 0.1f));
			_particleGameObject->GetTransform()->SetLocalRotation(DUOLMath::Quaternion::CreateFromEulerAngle(DUOLMath::Vector3(-90.f, 0.f, 0.f)));
			_particleGameObject->GetTransform()->SetLocalPosition(DUOLMath::Vector3(0.f, 0.f, 2.f));
		}
	}

	void PlayerState_Run::OnStateExit(float deltaTime)
	{
		PlayerStateBase::OnStateExit(deltaTime);

		// TODO : 일단 속도 0으로 만들어주자 ..! 다음 스테이트가 이동 관련이 아니라면 !
		if (_stateMachine->GetNextState()->GetName() != TEXT("PlayerState_Move"))
		{
			_rigidbody->SetLinearVelocity(DUOLMath::Vector3::Zero);

			_player->_currentMoveSpeed = 0.f;
		}

		_animator->SetBool(TEXT("IsRun"), false);
		_animator->SetBool(TEXT("IsMove"), false);

		if (_particleGameObject != nullptr)
		{
			// TODO : 서서히 사라지는거 어떻게 ?
			_particleRenderer->Stop();
		}
	}
}