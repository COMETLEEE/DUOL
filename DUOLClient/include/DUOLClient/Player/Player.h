#pragma once
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"
#include "DUOLGameEngine/FiniteStateMachine/FiniteStateMachine.h"
#include "DUOLGameEngine/Manager/InputManager.h"

#include "DUOLClient/ECS/Component/CharacterBase.h"

namespace DUOLGameEngine
{
	class SkinnedMeshRenderer;
	class AnimatorController;
	class Rigidbody;
	class Animator;
	class BoxCollider;
}

namespace DUOLClient
{
	class Weapon_AreaWave;
	class Weapon_Wave;
	class Weapon_Sword;
	class MainCameraController;
}

namespace DUOLClient
{
#pragma region 플레이어 키 입력
	constexpr DUOLGameEngine::KeyCode UP_KEY = DUOLGameEngine::KeyCode::W;
	constexpr DUOLGameEngine::KeyCode LEFT_KEY = DUOLGameEngine::KeyCode::A;
	constexpr DUOLGameEngine::KeyCode DOWN_KEY = DUOLGameEngine::KeyCode::S;
	constexpr DUOLGameEngine::KeyCode RIGHT_KEY= DUOLGameEngine::KeyCode::D;

	constexpr DUOLGameEngine::MouseCode SWORD_ATTACK_KEY = DUOLGameEngine::MouseCode::Left;
	constexpr DUOLGameEngine::MouseCode FIST_ATTACK_KEY = DUOLGameEngine::MouseCode::Right;
	constexpr DUOLGameEngine::KeyCode RUN_KEY = DUOLGameEngine::KeyCode::LeftShift;

	constexpr DUOLGameEngine::KeyCode ULTIMATE_KEY = DUOLGameEngine::KeyCode::E;

	constexpr DUOLGameEngine::KeyCode DASH_KEY = DUOLGameEngine::KeyCode::Space;
	constexpr DUOLGameEngine::KeyCode LOCKON_KEY = DUOLGameEngine::KeyCode::V;
#pragma endregion

#pragma region 플레이어 밸런스
	constexpr float OVERDRIVE_POINT_PER_SWORD = 0.5f;

	constexpr float OVERDRIVE_POINT_PER_FIST = 1.f;

	// 초월 게이지 PER ATTACK

	constexpr float DOWN_POINT_PER_ATTACK = 3.f;

	constexpr float MAX_DOWN_POINT = 12.f;

	constexpr float RESET_DOWN_POINT = 3.f;
#pragma endregion

	/**
	 * \brief Player Total Controller.
	 */
	class Player : public DUOLClient::CharacterBase
	{
	public:
		Player(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("Player"));

		virtual ~Player();

	private:
		DUOLGameEngine::FiniteStateMachine _playerStateMachine;

		float _defaultSwordDamage;

		float _defaultPunchDamage;

		float _defaultOverdriveDamage;

		float _defaultSwordAttackSpeed;

		float _defaultPunchAttackSpeed;

		float _defaultOverdriveAttackSpeed;

		float _defaultMaxMoveSpeed;

		float _defaultMaxRunSpeed;

		float _defaultMaxLockOnMoveSpeed;

		float _defaultMaxLockOnRunSpeed;

		float _currentDamage;

		float _currentMoveSpeed;

		// 공격 중 끊겼을 때의 후딜레이
		float _inAttackPostDelay;

		// 공격 마무리 까지 했을 때의 후딜레이
		float _endAttackPostDelay;

		/**
		 * \brief 현재 플레이어의 다운 포인트.
		 */
		float _currentDownPoint;

		/**
		 * \brief 현재 오버드라이브 수치 (100이 되면 변신)
		 */
		float _currentOverdrivePoint;

		bool _isOverdriveSwordMode;

		bool _isOverdriveFistMode;

		bool _isUseSpecialMove;

		bool _isLockOnMode;

		bool _isDash;

		bool _isSuperArmor;

		bool _isInSuperArmorRimLight;

		/**
		 * \brief 공격 상태에 진입할 수 있습니까 ? (선딜, 후딜 적용)
		 */
		bool _canStartAttack;

		/**
		 * \brief Dash 가능 여부
		 */
		bool _canStartDash;

#pragma region 기본 참조 컴포넌트
		DUOLGameEngine::Transform* _playerTransform;

		DUOLGameEngine::Animator* _playerAnimator;

		DUOLGameEngine::Rigidbody* _playerRigidbody;

		/**
		 * \brief 현재 쥐고있는 플레이어의 검입니다
		 */
		DUOLClient::Weapon_Sword* _currentPlayerWeapon;

		DUOLGameEngine::BoxCollider* _currentplayerWeaponSwordCollider;

		/**
		 * \brief 기본 공격 무기 검.
		 */
		DUOLClient::Weapon_Sword* _playerWeaponSword;

		DUOLGameEngine::GameObject* _playerLeftFistHolder;

		DUOLGameEngine::GameObject* _playerRightFistHolder;

		DUOLGameEngine::BoxCollider* _playerWeaponSwordCollider;
		/**
		 * \brief 오버드라이브 상태 무기 대검.
		 */
		DUOLClient::Weapon_Sword* _playerOverdriveWeaponSword;

		DUOLGameEngine::BoxCollider* _playerOverdriveWeaponSwordCollider;

		DUOLClient::Weapon_Wave* _playerWeaponWave;

		DUOLClient::Weapon_AreaWave* _playerWeaponAreaWave;

		/**
		 * \brief 메인 카메라 트랜스폼.
		 */
		DUOLGameEngine::Transform* _cameraTransform;

		DUOLClient::MainCameraController* _mainCamController;

		DUOLGameEngine::Transform* _lockOnTargetTransform;
#pragma endregion

	private:
		virtual bool Attack(CharacterBase* other /* 내가 공격할 대상 */, float damage /* 공격할 대상에게 가할 데미지 */, AttackType attackType) override;

		virtual bool OnHit(CharacterBase* other /* 나를 공격한 대상 */, float damage /* 공격한 대상으로부터 받은 데미지 */, AttackType attackType) override;

		virtual bool GetIsInvincible() override;

		void InitializeStateMachine();

		// 림라이트
		DUOLGameEngine::CoroutineHandler SuperArmorRimLight();

		DUOLGameEngine::SkinnedMeshRenderer* _playerSkinnedMeshRenderer;
		// 림라이트

		void SetSuperArmor(bool value);

	public:
		virtual void OnStart() override;

		virtual void OnUpdate(float deltaTime) override;

		virtual void OnLateUpdate(float deltaTime) override;

		virtual void OnFixedUpdate(float fixedTimeStep) override;

		virtual void OnTriggerStay(const std::shared_ptr<DUOLPhysics::Trigger>& trigger) override;

		virtual void OnCollisionStay(const std::shared_ptr<DUOLPhysics::Collision>& collision) override;
#pragma region FRIEND_CLASS
		friend class PlayerStateBase;

		friend class PlayerState_Idle;

		friend class PlayerState_Move;

		friend class PlayerState_Run;

		friend class PlayerState_Attack;

		friend class PlayerState_Dash;

		friend class PlayerState_Hit;

		friend class PlayerState_Down;

		friend class PlayerState_Overdrive;

		friend class PlayerState_Ultimate;

		friend class Weapon_Sword;

		friend class Weapon_Wave;

		friend class Weapon_AreaWave;
#pragma endregion

		RTTR_REGISTRATION_FRIEND

		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)
	};
}
