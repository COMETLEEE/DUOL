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
#pragma region �÷��̾� Ű �Է�
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

#pragma region �÷��̾� �뷱��
	constexpr float OVERDRIVE_POINT_PER_SWORD = 0.5f;

	constexpr float OVERDRIVE_POINT_PER_FIST = 1.f;

	// �ʿ� ������ PER ATTACK

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

		// ���� �� ������ ���� �ĵ�����
		float _inAttackPostDelay;

		// ���� ������ ���� ���� ���� �ĵ�����
		float _endAttackPostDelay;

		/**
		 * \brief ���� �÷��̾��� �ٿ� ����Ʈ.
		 */
		float _currentDownPoint;

		/**
		 * \brief ���� ��������̺� ��ġ (100�� �Ǹ� ����)
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
		 * \brief ���� ���¿� ������ �� �ֽ��ϱ� ? (����, �ĵ� ����)
		 */
		bool _canStartAttack;

		/**
		 * \brief Dash ���� ����
		 */
		bool _canStartDash;

#pragma region �⺻ ���� ������Ʈ
		DUOLGameEngine::Transform* _playerTransform;

		DUOLGameEngine::Animator* _playerAnimator;

		DUOLGameEngine::Rigidbody* _playerRigidbody;

		/**
		 * \brief ���� ����ִ� �÷��̾��� ���Դϴ�
		 */
		DUOLClient::Weapon_Sword* _currentPlayerWeapon;

		DUOLGameEngine::BoxCollider* _currentplayerWeaponSwordCollider;

		/**
		 * \brief �⺻ ���� ���� ��.
		 */
		DUOLClient::Weapon_Sword* _playerWeaponSword;

		DUOLGameEngine::GameObject* _playerLeftFistHolder;

		DUOLGameEngine::GameObject* _playerRightFistHolder;

		DUOLGameEngine::BoxCollider* _playerWeaponSwordCollider;
		/**
		 * \brief ��������̺� ���� ���� ���.
		 */
		DUOLClient::Weapon_Sword* _playerOverdriveWeaponSword;

		DUOLGameEngine::BoxCollider* _playerOverdriveWeaponSwordCollider;

		DUOLClient::Weapon_Wave* _playerWeaponWave;

		DUOLClient::Weapon_AreaWave* _playerWeaponAreaWave;

		/**
		 * \brief ���� ī�޶� Ʈ������.
		 */
		DUOLGameEngine::Transform* _cameraTransform;

		DUOLClient::MainCameraController* _mainCamController;

		DUOLGameEngine::Transform* _lockOnTargetTransform;
#pragma endregion

	private:
		virtual bool Attack(CharacterBase* other /* ���� ������ ��� */, float damage /* ������ ��󿡰� ���� ������ */, AttackType attackType) override;

		virtual bool OnHit(CharacterBase* other /* ���� ������ ��� */, float damage /* ������ ������κ��� ���� ������ */, AttackType attackType) override;

		virtual bool GetIsInvincible() override;

		void InitializeStateMachine();

		// ������Ʈ
		DUOLGameEngine::CoroutineHandler SuperArmorRimLight();

		DUOLGameEngine::SkinnedMeshRenderer* _playerSkinnedMeshRenderer;
		// ������Ʈ

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
