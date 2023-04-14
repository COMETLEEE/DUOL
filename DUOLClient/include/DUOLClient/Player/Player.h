#pragma once
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"
#include "DUOLGameEngine/FiniteStateMachine/FiniteStateMachine.h"
#include "DUOLGameEngine/Manager/InputManager.h"

#include "DUOLClient/ECS/Component/CharacterBase.h"



namespace DUOLGameEngine
{
	class AnimatorController;
	class Rigidbody;
	class Animator;
	class BoxCollider;
}

namespace DUOLClient
{
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

	constexpr DUOLGameEngine::MouseCode SWORD_ATTACK_KEY = DUOLGameEngine::MouseCode::Middle;
	constexpr DUOLGameEngine::MouseCode FIST_ATTACK_KEY = DUOLGameEngine::MouseCode::Right;
	constexpr DUOLGameEngine::KeyCode RUN_KEY = DUOLGameEngine::KeyCode::LeftShift;

	constexpr DUOLGameEngine::KeyCode DASH_KEY = DUOLGameEngine::KeyCode::Space;
	constexpr DUOLGameEngine::KeyCode LOCKON_KEY = DUOLGameEngine::KeyCode::V;
#pragma endregion

#pragma region �÷��̾� �뷱��
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
		/**
		 * \brief Sword Animator Controller
		 */
		DUOLGameEngine::AnimatorController* _swordAnimatorController;

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

		bool _isLockOnMode;

		bool _isDash;

		/**
		 * \brief ���� ���¿� ������ �� �ֽ��ϱ� ? (����, �ĵ� ����)
		 */
		bool _canStartAttack;

#pragma region �⺻ ���� ������Ʈ
		DUOLGameEngine::Transform* _playerTransform;

		DUOLGameEngine::Animator* _playerAnimator;

		DUOLGameEngine::Rigidbody* _playerRigidbody;

		/**
		 * \brief �⺻ ���� ���� ��.
		 */
		DUOLClient::Weapon_Sword* _playerWeaponSword;

		DUOLGameEngine::BoxCollider* _playerWeaponSwordCollider;

		DUOLClient::Weapon_Wave* _playerWeaponWave;

		/**
		 * \brief ���� ī�޶� Ʈ������.
		 */
		DUOLGameEngine::Transform* _cameraTransform;

		DUOLClient::MainCameraController* _mainCamController;

		DUOLGameEngine::Transform* _lockOnTargetTransform;
#pragma endregion

	private:
		virtual void Attack(CharacterBase* other /* ���� ������ ��� */, float damage /* ������ ��󿡰� ���� ������ */, AttackType attackType) override;

		virtual void OnHit(CharacterBase* other /* ���� ������ ��� */, float damage /* ������ ������κ��� ���� ������ */, AttackType attackType) override;

		void InitializeStateMachine();

	public:
		virtual void OnStart() override;

		virtual void OnUpdate(float deltaTime) override;

		virtual void OnFixedUpdate(float fixedTimeStep) override;

#pragma region FRIEND_CLASS
		friend class PlayerStateBase;

		friend class PlayerState_Idle;

		friend class PlayerState_Move;

		friend class PlayerState_Run;

		friend class PlayerState_Attack;

		friend class PlayerState_Dash;

		friend class PlayerState_Hit;

		friend class PlayerState_Down;

		friend class Weapon_Sword;

		friend class Weapon_Wave;
#pragma endregion

		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)
	};
}
