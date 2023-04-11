#include "DUOLClient/Player/FSM/PlayerState_Hit.h"

#include "DUOLGameEngine/ECS/Component/Animator.h"

namespace DUOLClient
{
	PlayerState_Hit::PlayerState_Hit(DUOLClient::Player* player) :
		PlayerStateBase(TEXT("PlayerState_Hit"), player)
		, _downPoint(0.f)
		, _currentAttackType(AttackType::NoAttack)
		, _isAccumulatedHit(false)
	{

	}

	PlayerState_Hit::~PlayerState_Hit()
	{
	}

	void PlayerState_Hit::AccumulateHit()
	{
		_isAccumulatedHit = true;
	}

	void PlayerState_Hit::OnStateEnter(float deltaTime)
	{
		PlayerStateBase::OnStateEnter(deltaTime);

		_downPoint += DOWN_POINT_PER_ATTACK;

		// 애니메이션 변동
		if (_currentAttackType == AttackType::HeavyAttack)
		{
			StartHeavyHitAnimation();
		}
		else
		{
			// 3개 중 랜덤 재생
			
		}
	}

	void PlayerState_Hit::OnStateStay(float deltaTime)
	{
		PlayerStateBase::OnStateStay(deltaTime);

		if (_downPoint >= MAX_DOWN_POINT)
		{
			_stateMachine->TransitionTo(TEXT("PlayerState_Down"), deltaTime);
		}
		else if (_isAccumulatedHit)
		{
			_downPoint += DOWN_POINT_PER_ATTACK;

			// 중복 공격을 통해서도 다운될 수 있으니 한 번 더 체크 ..!
			if (_downPoint >= MAX_DOWN_POINT)
				_stateMachine->TransitionTo(TEXT("PlayerState_Down"), deltaTime);

			_isAccumulatedHit = false;
		}
	}

	void PlayerState_Hit::OnStateExit(float deltaTime)
	{
		PlayerStateBase::OnStateExit(deltaTime);

		// 피격 이후에는 Idle state로 넘어간다.
		_stateMachine->TransitionTo(TEXT("PlayerState_Idle"), deltaTime);

		// TODO : 보통 연속 공격 당하는 것 아니면 다운 포인트를 초기화하지 않나요 ?
		_downPoint = 0.f;
	}

	void PlayerState_Hit::StartHit1Animation()
	{
		_animator->SetBool(TEXT("IsHit1"), true);
		_animator->SetBool(TEXT("IsHit2"), false);
		_animator->SetBool(TEXT("IsHit3"), false);
		_animator->SetBool(TEXT("IsHeavyHit"), false);
	}

	void PlayerState_Hit::StartHit2Animation()
	{
		_animator->SetBool(TEXT("IsHit1"), false);
		_animator->SetBool(TEXT("IsHit2"), true);
		_animator->SetBool(TEXT("IsHit3"), false);
		_animator->SetBool(TEXT("IsHeavyHit"), false);
	}

	void PlayerState_Hit::StartHit3Animation()
	{
		_animator->SetBool(TEXT("IsHit1"), false);
		_animator->SetBool(TEXT("IsHit2"), false);
		_animator->SetBool(TEXT("IsHit3"), true);
		_animator->SetBool(TEXT("IsHeavyHit"), false);
	}

	void PlayerState_Hit::StartHeavyHitAnimation()
	{
		_animator->SetBool(TEXT("IsHit1"), false);
		_animator->SetBool(TEXT("IsHit2"), false);
		_animator->SetBool(TEXT("IsHit3"), false);
		_animator->SetBool(TEXT("IsHeavyHit"), true);
	}

	void PlayerState_Hit::SetCurrentAttackType(AttackType attackType)
	{
		_currentAttackType = attackType;
	}
}