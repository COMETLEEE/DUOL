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
	class Material;
	class SkinnedMeshRenderer;
	class Rigidbody;
	class CapsuleCollider;
	class Animator;
	class NavMeshAgent;
}

namespace DUOLClient
{
	class EnemyAirborneCheck;
	class EnemyParentObjectObserver;
	class AI_EnemyBasic;
	enum class EnemyCode;
	struct EnemyData;

	class DUOL_CLIENT_API Enemy : public CharacterBase
	{
	public:
		Enemy(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("Enemy"));

		virtual ~Enemy() override;

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

		DUOLGameEngine::CapsuleCollider* _parentCapsuleCollider;

		DUOLGameEngine::Rigidbody* _rigidbody;

		DUOLGameEngine::GameObject* _parentGameObject; // 물리 연산을 수행하기 위한 부모..! 

		DUOLGameEngine::Transform* _transform;

		EnemyAirborneCheck* _enemyAirborneCheck;

		EnemyParentObjectObserver* _parentObserver;

		std::vector<std::pair<DUOLCommon::tstring, DUOLCommon::EventListenerID>> _eventListenerIDs;

		const EnemyData* _enemyData;

		AI_EnemyBasic* _ai;

		DUOLGameEngine::SkinnedMeshRenderer* _skinnedMeshRenderer;

		std::vector<DUOLGameEngine::Material*> _originMaterials;

		bool _isOriginMaterial;
	public:
		void SetPosition(DUOLMath::Vector3 pos);

		void SetPosition(const float& x, const float& y, const float& z);

		void SetEnemyCode(EnemyData* enemyData);

		const EnemyData* GetEnemyData();

		float GetAttackRange() const { return _attackRange; }

		float GetPatrolOffset() const { return _patrolOffset; }

		float GetLookRange() const { return _lookRange; }

		float GetMaxSpeed() const { return _maxSpeed; }

		bool GetIsAirBorne();

		AI_EnemyBasic* GetAIController();

		void ChangeMaterial(bool isDie);
	public:
		virtual void OnEnable() override;

		virtual void Attack(CharacterBase* target, float damage, AttackType attackType) override;

		virtual void OnHit(CharacterBase* other, float damage, AttackType attackType) override;

		virtual void OnAwake() override;

		virtual void OnUpdate(float deltaTime) override;

		friend class AI_EnemyBasic;

		RTTR_ENABLE(CharacterBase)

			RTTR_REGISTRATION_FRIEND
	};
}
