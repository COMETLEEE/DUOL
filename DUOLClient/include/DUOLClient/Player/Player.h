#pragma once
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"
#include "DUOLGameEngine/FiniteStateMachine/FiniteStateMachine.h"
#include "DUOLGameEngine/Manager/InputManager.h"

#include "DUOLClient/ECS/Component/CharacterBase.h"

namespace DUOLGameEngine
{
	class CapsuleCollider;
	class AudioClip;
	class AudioListener;
	class AudioSource;
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
	constexpr DUOLGameEngine::KeyCode RIGHT_KEY = DUOLGameEngine::KeyCode::D;

	constexpr DUOLGameEngine::MouseCode SWORD_ATTACK_KEY = DUOLGameEngine::MouseCode::Left;
	constexpr DUOLGameEngine::MouseCode FIST_ATTACK_KEY = DUOLGameEngine::MouseCode::Right;
	constexpr DUOLGameEngine::KeyCode RUN_KEY = DUOLGameEngine::KeyCode::LeftShift;

	constexpr DUOLGameEngine::KeyCode ULTIMATE_KEY = DUOLGameEngine::KeyCode::E;

	constexpr DUOLGameEngine::KeyCode DASH_KEY = DUOLGameEngine::KeyCode::Space;
	constexpr DUOLGameEngine::KeyCode LOCKON_KEY = DUOLGameEngine::KeyCode::PageUp;
#pragma endregion

	constexpr int MAX_SOUND_PLAYER = 5;

#pragma region �÷��̾� �뷱��
	// �ʿ� ������ PER ATTACK

	constexpr float MAX_DOWN_POINT = 100.f;

	constexpr float RESET_DOWN_POINT = 4.f;

#pragma endregion

	enum class PlayerSoundTable
	{
		FootStep01 = 0,
		FootStep02,
		avoidSound,
		Slash_One,
		Slash_Two,

		Slash_Three,
		Slash_Final,
		Fist_One,
		Fist_Two,
		Normal_Last_Punch,

		MagnumPunch,
		MagnumPunch_Over,

		Overdrive_Fist_One,
		Overdrive_Fist_Two,
		OverdriveSword01,
		OverdriveSword02,
		OverdriveSwordFinal,

		UltimateMagnumPunch,
		SwordChargingSound01,
		SwordChargingSound02,
		FistChargingSound,
		//���ĺ��� Ÿ�ݻ���

		Hit_Sound,
		AuraSound,
		OverdriveFist01,
		OverdriveFist02,
		Hit_Sound_Effect,

		Hitting_Ground,
		NONE
	};

	enum class PlayerVoiceSoundTable
	{
		Voice_Dash = 0,
		Voice_Hit01,
		Voice_Hit02,
		Voice_Knock01,
		Voice_Knock02,

		Voice_Grogi01,
		Voice_Grogi02,

		Voice_NormalAttack01,
		Voice_NormalAttack03,
		Voice_NormalAttack04,
		Voice_NormalAttack05,
		Voice_NormalAttack06,
		Voice_NormalAttack07,
		Voice_NormalAttack08,
		Voice_NormalLastAttack,

		Voice_FistActive01,
		Voice_FistActive02,

		Voice_LastPunch01,
		Voice_LastPunch02,

		Voice_MagnumPunch01,
		Voice_MagnumPunch01jump,
		Voice_MagnumPunch02,
		Voice_MagnumPunch02jump,

		Voice_UltimatePunch01,
		Voice_UltimatePunch02,

		Voice_UltimateSword01,
		Voice_UltimateSword02,
		Voice_Result_Character,

		Voice_UltimateCharging,

		NONE
	};


	//�� ���ݸ��� ���� �������� �������ִ�. 
	enum class PlayerVoiceSoundSet
	{
		NormalSword_01 = 0, //nomatk04, 06
		NormalSword_02, //nomatk01, 05
		NormalSword_03, //nomatk03, 07
		NormalSword_04, //lastatk;
		NormalSword_SwordCombo_1_2,   //5 8
		NormalSword_SwordCombo_1_3,
		NormalSword_SwordCombo_2_3,
		NormalSword_SwordCombo_23_4Jump,
		NormalSword_SwordCombo_23_4,
		NormalFist_01,
		NormalFist_02,
		NormalFist_03,
		NormalFist_FistCombo_1_2,
		NormalFist_FistCombo_1_3,
		NormalFist_FistCombo_2_3,
		NormalFist_FistCombo_2_4,
		Overdrive_Sword01,
		Overdrive_Sword02,
		Overdrive_SwordFin,
		Overdrive_Fist01,
		Overdrive_Fist02,
		Overdrive_FistUlt,
		Overdrive_SwordUlt,
		Overdrive_Enter,
		NONE
	};

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

		float _downPointResetTime;

		float _currentDownPointResetTime;
		
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

		bool _isUltimate;

		float _playerHitAnimationSpeed;
		/**
		 * \brief ���� ���¿� ������ �� �ֽ��ϱ� ? (����, �ĵ� ����)
		 */
		bool _canStartAttack;

		/**
		 * \brief Dash ���� ����
		 */
		bool _canStartDash;

		bool _canInteraction;

		//�ܺο��ο� ���� dash lock���
		bool _isDashCrowdControl;

		float _overdriveDuration;

		float _overdrivePointPerSword;

		float _overdrivePointPerFist;

		float _superArmorDamageDecrease;

		float _jumpHoldTime;

		float _jumpFlyTime;


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

		CharacterBase* _lockOnTargetCharacterBase;

		/**
		* \brief ���� ������Ʈ ���
		*/
		DUOLGameEngine::AudioListener* _audioListener;

		std::vector<DUOLGameEngine::AudioClip*> _attackAudioClips;

		/**
		 * \brief ���� ������Ʈ. Ÿ����(�ִ� 6������)
		 */
		std::vector<DUOLGameEngine::AudioSource*> _attackSoundModules;

		std::vector<DUOLGameEngine::AudioSource*> _soundModules;

		/**
		* \brief ���� ������Ʈ. ������
		*/
		DUOLGameEngine::AudioSource* _attackSource;

		/**
		 * \brief ��������̺�� aura
		 */
		DUOLGameEngine::AudioSource* _auraSource;

		std::vector<DUOLGameEngine::AudioClip*> _voiceAudioClips;

		std::map<PlayerVoiceSoundSet, std::pair<DUOLGameEngine::AudioClip*, DUOLGameEngine::AudioClip*>> _voiceAudioSets;

		/**
		 * \brief ��Ÿ ���̽�
		 */
		DUOLGameEngine::AudioSource* _voiceSource;

		DUOLGameEngine::AudioSource* _scriptSource;

		std::unordered_map<DUOLCommon::tstring, float> _playerDataTable;


#pragma endregion

	private:
		virtual bool Attack(CharacterBase* other /* ���� ������ ��� */, float damage /* ������ ��󿡰� ���� ������ */, AttackType attackType, float downPoint = DOWN_POINT_PER_ATTACK) override;

		virtual bool OnHit(CharacterBase* other /* ���� ������ ��� */, float damage /* ������ ������κ��� ���� ������ */, AttackType attackType, float downPoint = DOWN_POINT_PER_ATTACK) override;

	public:
		virtual void SetDashUsable(bool dash);

	private:
		virtual bool GetIsInvincible() override;

		void InitializeStateMachine();

		// ������Ʈ
		DUOLGameEngine::CoroutineHandler SuperArmorRimLight();

		DUOLGameEngine::SkinnedMeshRenderer* _playerSkinnedMeshRenderer;
		// ������Ʈ

		void SetSuperArmor(bool value);

		void LoadAudioClip();
	public:
		float GetPlayerHitAnimationSpeed() { return _playerHitAnimationSpeed; }

		void SetPlayerHitAnimationSpeed(float value) { _playerHitAnimationSpeed = value; }

		void Jump(float holdTime, float _flyTime);

		void JumpEnd();

		virtual void OnStart() override;

		virtual void OnUpdate(float deltaTime) override;

		virtual void OnLateUpdate(float deltaTime) override;

		virtual void OnFixedUpdate(float fixedTimeStep) override;

		virtual void OnTriggerStay(const std::shared_ptr<DUOLPhysics::Trigger>& trigger) override;

		void SetCanInteraction(bool value);

		//virtual void OnCollisionStay(const std::shared_ptr<DUOLPhysics::Collision>& collision) override;
		//���ϻ������(ex�߰���)
		void PlaySoundClip(PlayerSoundTable soundClip, bool isLoop);

		void PlaySoundClipAndVoice(PlayerSoundTable soundClip, PlayerVoiceSoundSet voiceSet);

		//Ÿ����
		void PlaySoundClipInModule(DUOLGameEngine::AudioClip* soundClip, int idx, bool isLoop);

		//Ÿ����
		void PlaySoundClipInModule(PlayerSoundTable  soundClip, int idx, bool isLoop);

		void PlayVoiceSoundClip(PlayerVoiceSoundTable  soundClip, bool isLoop);


		void PlayScriptSoundClip(DUOLGameEngine::AudioClip* soundClip, bool isLoop);

		void PlayScriptPause();

		void PlayScriptPlay();


		void AddOverdrivePoint(float point);

		bool IsOverdriveMode();

		DUOLGameEngine::AudioSource* GetAuraSoundSource();

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

		friend class PlayerState_Jump;

		friend class Weapon_Sword;

		friend class Weapon_Wave;

		friend class Weapon_AreaWave;
#pragma endregion

		RTTR_REGISTRATION_FRIEND

			RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)
	};
}
