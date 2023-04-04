/**

	@file      Enemy.h
	@brief     Enemy의 현재 정보 및 기능을 가지고 있는 클래스.
	@details   ~
	@author    Cool Guy
	@date      23.03.2023
	@copyright © Cool Guy, 2023. All right reserved.

**/
#pragma once

#include "DUOLClient/ECS/Component/CharacterBase.h"

namespace DUOLGameEngine
{
	class CapsuleCollider;
	class Animator;
	class NavMeshAgent;
}

namespace DUOLClient
{
	enum class EnemyCode;
	struct EnemyData;

	class DUOL_CLIENT_API Enemy : public CharacterBase
	{
	public:
		Enemy(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("Enemy"));

		virtual ~Enemy() override = default;

	private:
		bool _isHit; // 히트를 당했는가.

		float _attackRange; // 공격 사정거리

		float _patrolOffset; // 정렬할 때 타겟과의 거리

		float _lookRange; // 시야거리

		float _maxSpeed; // 달리기 속도.

		bool _isToken; // 토큰을 가지고 있는 객체만 공격한다..

		DUOLGameEngine::GameObject* _target = nullptr;

		DUOLGameEngine::NavMeshAgent* _navMeshAgent;

		DUOLGameEngine::Animator* _animator;

		DUOLGameEngine::CapsuleCollider* _capsuleCollider;

		const EnemyData* _enemyData;

	public:
		void SetEnemyCode(EnemyCode enemyCode);

		const EnemyData* GetEnemyData();

		float GetAttackRange() const { return _attackRange; }

		float GetPatrolOffset() const { return _patrolOffset; }

		float GetLookRange() const { return _lookRange; }

		float GetMaxSpeed() const { return _maxSpeed; }

	public:
		virtual void Attack(CharacterBase* target, float damage) override;

		virtual void OnHit(CharacterBase* other, float damage) override;

		virtual void OnAwake() override;

		friend class AI_EnemyBasic;

		RTTR_ENABLE(CharacterBase)
			RTTR_REGISTRATION_FRIEND
	};
}
