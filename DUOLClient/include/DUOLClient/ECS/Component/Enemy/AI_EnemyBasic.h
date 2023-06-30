/**

	@file      AI_Enemy.h
	@brief     BehaviorTree와 함께 사용하여야하는 클래스. BehaviorTree에서 다른 클래스에 접근하기 위해서 해당 클래스를 거친다.
	@details   ~ Controller와 같은 역할.
				Character 클래스를 기반으로 조종하는 클래스!
	@author    SinSeongHyeon
	@date      10.03.2023
	@copyright © SinSeongHyeon, 2023. All right reserved.

**/
#pragma once
#include "Enemy.h"
#include "DUOLClient/Export_Client.h"
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLGameEngine
{
	class Animator;
	class NavMeshAgent;
	class BehaviortreeController;
}

namespace DUOLClient
{
	enum class EnemyMaterial;
	enum class EnemyCode;
	class Enemy;
	class EnemyGroupController;

	class DUOL_CLIENT_API AI_EnemyBasic : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		AI_EnemyBasic(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("AI_EnemyBasic"));

		virtual ~AI_EnemyBasic() override;
	private:
		// 변수 선언
		DUOLGameEngine::BehaviortreeController* _behaviortreeController; // 캐싱해서 사용.

		EnemyGroupController* _enemyGroupController;

		Enemy* _enemy; // 내가 조종할 객체, 캐싱

		DUOLGameEngine::GameObject* _parentGameObject;

		DUOLGameEngine::Transform* _parentTransform;

		DUOLGameEngine::Transform* _targetTransform;

		bool _dieOnce;

		DUOLGameEngine::AudioSource* _audioSource;

		DUOLGameEngine::AudioSource* _voiceAudioSource;

	public:
		void Initialize();

		void Reset();

		void SetAnimConditionReset();

		bool GetIsHitCheck();

		void SetIsHit(bool isHit);

		bool GetIsGroupCheck();

		void SetIsGroupCheck();

		void SetGroupController(EnemyGroupController* enemyGroupController);

		void SetIsDie();

		void UseToken();

		void TakeToken();

		void SetTarget(DUOLGameEngine::GameObject* target);

		void SetNavOnRigidbodyOff();

		void SetNavOffRigidbodyOn();

		void SetSuperArmor(bool isSuperArmor, float time = std::numeric_limits<float>::max());

		void SetColliderEnable(bool isBool);

		void SetNavEnable(bool isBool);

		bool GetIsToken() const;

		bool GetIsDie() const;

		bool GetIsAirborne() const;

		bool GetIsChase() const;

		float GetLookRange() const;

		float GetAttackRange() const;

		float GetPatrolRange() const;

		float GetMaxSpeed() const;

		bool GetIsSuperArmor() const;

		void AddSuperArmorGauge(float addGauge);

		float GetCurrentSuperArmorGauge() const;

		float GetMaxSuperArmorGauge() const;

		float GetSuperArmorTime() const;

		void ChangeMaterial(EnemyMaterial enemyMaterial);

		void LerpLookTarget();

		Enemy* GetEnemy();

		DUOLGameEngine::AudioSource* GetAudioSource();

		DUOLGameEngine::AudioSource* GetVoiceAudioSource();

		void PlaySound(EnemyAudioEnum audioEnum, bool isOverride, unsigned offset = 0);

		void PlayVoiceSound(EnemyAudioEnum audioEnum, bool isOverride, unsigned offset = 0);

		DUOLGameEngine::GameObject* GetParentGameObject() const;

		DUOLGameEngine::Transform* GetParentTransform() const;

		DUOLGameEngine::GameObject* GetTarget() const;

		DUOLGameEngine::Transform* GetTargetTransform() const;

		EnemyGroupController* GetGroupController() const;

		DUOLGameEngine::NavMeshAgent* GetNavMeshAgent() const;

		DUOLGameEngine::Animator* GetAnimator() const;

		DUOLGameEngine::BehaviortreeController* GetBehaviorTreeController() const;

		template<class T>
		bool GetContainsParameter(DUOLCommon::tstring key);

		template<class T>
		void SetParameter(DUOLCommon::tstring key, T value);

		template<class T>
		T GetParameter(DUOLCommon::tstring key);
	public:
		virtual void OnAwake() override;

		friend class EnemyGroupController;

		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)
			RTTR_REGISTRATION_FRIEND
	};



	template <>
	inline bool AI_EnemyBasic::GetContainsParameter<bool>(DUOLCommon::tstring key)
	{
		return _enemy->GetContainsParameter<bool>(key);
	}

	template <>
	inline bool AI_EnemyBasic::GetContainsParameter<float>(DUOLCommon::tstring key)
	{
		return _enemy->GetContainsParameter<float>(key);
	}

	template <>
	inline void AI_EnemyBasic::SetParameter(DUOLCommon::tstring key, float value)
	{
		_enemy->SetParameter(key, value);
	}

	template <>
	inline void AI_EnemyBasic::SetParameter(DUOLCommon::tstring key, bool value)
	{
		_enemy->SetParameter(key, value);
	}

	template <>
	inline bool AI_EnemyBasic::GetParameter(DUOLCommon::tstring key)
	{
		return _enemy->GetParameter<bool>(key);
	}

	template <>
	inline float AI_EnemyBasic::GetParameter(DUOLCommon::tstring key)
	{
		return _enemy->GetParameter<float>(key);
	}
}
