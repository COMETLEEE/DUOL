#include "DUOLClient/Player/FSM/PlayerState_Hit.h"

#include "DUOLGameEngine/ECS/Component/Animator.h"
#include "DUOLGameEngine/Util/Coroutine/WaitForSeconds.h"

namespace DUOLClient
{
	PlayerState_Hit::PlayerState_Hit(DUOLClient::Player* player) :
		PlayerStateBase(TEXT("PlayerState_Hit"), player)
		, _currentAttackType(AttackType::NoAttack)
		, _isAccumulatedHit(false)
		, _currentAnimIndex(0)
		, _resetDownPoint(nullptr)
	{
#pragma region END_HIT_EVENT
		_player->AddEventFunction(TEXT("EndHit"), std::bind(&DUOLClient::PlayerState_Hit::EndHit, this));
#pragma endregion

#pragma region BUILD_HIT_PARAMETERS
		_hitAnimParameters.push_back(TEXT("IsHit1"));
		_hitAnimParameters.push_back(TEXT("IsHit2"));
#pragma endregion
	}

	PlayerState_Hit::~PlayerState_Hit()
	{
	}

	void PlayerState_Hit::AccumulateHit()
	{
		_isAccumulatedHit = true;

		_currentAttackType == AttackType::HeavyAttack
			? StartHeavyHit()
			: StartHit();
	}

	void PlayerState_Hit::OnStateEnter(float deltaTime)
	{
		PlayerStateBase::OnStateEnter(deltaTime);

		// 애니메이션 변동
		_currentAttackType == AttackType::HeavyAttack
			? StartHeavyHit()
			: StartHit();

		if (_resetDownPoint != nullptr)
			_player->StopCoroutine(_resetDownPoint);

		_resetDownPoint = nullptr;
	}

	void PlayerState_Hit::OnStateStay(float deltaTime)
	{
		PlayerStateBase::OnStateStay(deltaTime);
	}

	void PlayerState_Hit::OnStateExit(float deltaTime)
	{
		PlayerStateBase::OnStateExit(deltaTime);

		_isAccumulatedHit = false;

		_currentAnimIndex = 0;

		std::function<DUOLGameEngine::CoroutineHandler(void)> resetDownPoint = std::bind(&DUOLClient::PlayerState_Hit::ResetDownPoint, this);

		// 혹시 모르니 여기서도 초기화한다.
		_animator->SetBool(TEXT("IsHit1"), false);
		_animator->SetBool(TEXT("IsHit2"), false);
		_animator->SetBool(TEXT("IsHit3"), false);
		_animator->SetBool(TEXT("IsHeavyHit"), false);

		_resetDownPoint = _player->StartCoroutine(resetDownPoint);
	}

	void PlayerState_Hit::StartHit()
	{
		int randIndex = DUOLMath::MathHelper::Rand(0, _hitAnimParameters.size() - 1);

		if (_isAccumulatedHit && randIndex == _currentAnimIndex)
			randIndex = (_currentAnimIndex + 1) % _hitAnimParameters.size();

		_animator->SetBool(_hitAnimParameters[randIndex], true);

		_animator->SetBool(TEXT("IsHeavyHit"), false);

		for (int i = randIndex + 1 ; ; i++)
		{
			i = i % _hitAnimParameters.size();

			if (i == randIndex)
				break;

			_animator->SetBool(_hitAnimParameters[i], false);
		}
	}

	void PlayerState_Hit::StartHeavyHit()
	{
		_animator->SetBool(TEXT("IsHit1"), false);
		_animator->SetBool(TEXT("IsHit2"), false);
		_animator->SetBool(TEXT("IsHit3"), false);
		_animator->SetBool(TEXT("IsHeavyHit"), true);
	}

	void PlayerState_Hit::EndHit()
	{
		_animator->SetBool(TEXT("IsHit1"), false);
		_animator->SetBool(TEXT("IsHit2"), false);
		_animator->SetBool(TEXT("IsHit3"), false);
		_animator->SetBool(TEXT("IsHeavyHit"), false);

		_stateMachine->TransitionTo(TEXT("PlayerState_Idle"), 0.f);
	}

	DUOLGameEngine::CoroutineHandler PlayerState_Hit::ResetDownPoint()
	{
		co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(RESET_DOWN_POINT);

		_player->_currentDownPoint = 0.f;
	}

	void PlayerState_Hit::SetCurrentAttackType(AttackType attackType)
	{
		_currentAttackType = attackType;
	}
}