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

	enum class HitEnum // 히트의 종류는 현재 두가지
	{
		Front,
		Back,
	};

	enum class EnemyMaterial
	{
		RED,
		WHITE,
		DIE,
		NORMAL
	};

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

		float _attackDelayTime; // 사정거리안에 플레이어가 들어왔을 때 얼마나 대기하고 공격할지.

		float _attackCancelTime; // 공격 페이크 모션을 할 때 애니메이션을 캔슬하는 시간.

		float _maxSuperArmorGauge; // 슈퍼 아머 사용을 위해 채워야한 목표치.

		float _superArmorCoolTime; // 슈퍼 아머 재사용 대기시간.

		float _superArmorTime; // 슈퍼 아머 지속시간.

		float _currentSuperArmorGauge; // 현재 게이지 충전량.

		bool _isToken; // 토큰을 가지고 있는 객체만 공격한다..

		bool _isSuperArmor;

		bool _isCanSuperArmor; // 쿨타임이 다 지났는가.

		float _chaseRange;

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

		EnemyMaterial _currentMaterial;

		HitEnum _hitEnum; // 이전과 다른 동작을 취해야한다...!

		std::function<void(DUOLClient::Enemy*, CharacterBase*, float, AttackType)> _hitFunc;
	public:
		void InitializeData();

		void SetPosition(DUOLMath::Vector3 pos);

		void SetPosition(const float& x, const float& y, const float& z);

		void SetEnemyCode(EnemyData* enemyData);

		void SetIsHit(bool isHit);

		void EnemyAddEventFunc(const DUOLCommon::tstring& eventName, std::function<void()> functor);

		void SetEnemyHitFunc(std::function<void(DUOLClient::Enemy*, CharacterBase*, float, AttackType)> func);

		void SetSuperArmor(bool isSuperArmor);

		void AddSuperArmorGauge(float addGauge);

		float GetCurrentSuperArmorGauge() const;

		float GetMaxSuperArmorGauge() const;

		float GetSuperArmorTime() const;

		const EnemyData* GetEnemyData();

		float GetAttackRange() const { return _attackRange; }

		float GetPatrolOffset() const { return _patrolOffset; }

		float GetLookRange() const { return _lookRange; }

		float GetMaxSpeed() const { return _maxSpeed; }

		float GetAttackDelayTime() const { return _attackDelayTime; }

		float GetAttackCancelTime() const { return _attackCancelTime; }

		bool GetIsAirBorne();

		bool GetIsSuperArmor();

		void SetIsCanSuperArmor(bool isBool);

		bool GetIsCanSuperArmor();

		AI_EnemyBasic* GetAIController();

		DUOLGameEngine::Rigidbody* GetRigidbody() const;

		DUOLGameEngine::Animator* GetAnimator() const;

		HitEnum GetHitEnum() const;

		void SetHitEnum(HitEnum hitEnum);

		void ChangeMaterial(EnemyMaterial enemyMat);

		void ChangeMaterialOnHit();

		void SetNavOnRigidbodyOff();

		void SetNavOffRigidbodyOn();

		DUOLGameEngine::GameObject* GetTarget() const;

		DUOLGameEngine::Transform* GetParentTransform() const;
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
