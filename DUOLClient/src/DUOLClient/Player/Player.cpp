#include "DUOLClient/Player/Player.h"

#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"
#include "DUOLGameEngine/Manager/ResourceManager.h"
#include "DUOLGameEngine/Manager/InputManager.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Transform.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"
#include "DUOLGameEngine/ECS/Component/BoxCollider.h"
#include "DUOLGameEngine/ECS/Component/ParticleRenderer.h"
#include "DUOLGameEngine/ECS/Component/SkinnedMeshRenderer.h"

#include "DUOLClient/Player/Weapon_Sword.h"
#include "DUOLClient/Player/Weapon_Wave.h"
#include "DUOLClient/Player/Weapon_AreaWave.h"
#include "DUOLClient/Player/FSM/PlayerState_Idle.h"
#include "DUOLClient/Player/FSM/PlayerState_Move.h"
#include "DUOLClient/Player/FSM/PlayerState_Attack.h"
#include "DUOLClient/Player/FSM//PlayerState_Hit.h"
#include "DUOLClient/Player/FSM//PlayerState_Dash.h"
#include "DUOLClient/Player/FSM//PlayerState_Run.h"
#include "DUOLClient/Player/FSM//PlayerState_Die.h"
#include "DUOLClient/Player/FSM//PlayerState_Interaction.h"

#include "DUOLClient/Camera/MainCameraController.h"
#include "DUOLClient/Manager/ParticleManager.h"

#include <rttr/registration>

#include "DUOLClient/ECS/Component/Enemy/Enemy.h"
#include "DUOLClient/Manager/GameManager.h"
#include "DUOLClient/Manager/SystemManager.h"
#include "DUOLClient/Manager/UIDataManager.h"
#include "DUOLClient/Player/FSM/PlayerState_Down.h"
#include "DUOLClient/Player/FSM/PlayerState_Overdrive.h"
#include "DUOLClient/Player/FSM/PlayerState_Ultimate.h"
#include "DUOLCommon/MetaDataType.h"
#include "DUOLGameEngine/ECS/Component/AudioSource.h"
#include "DUOLGameEngine/ECS/Component/AudioListener.h"
#include "DUOLGameEngine/ECS/Component/MeshRenderer.h"


#include "DUOLGameEngine/Manager/SoundManager.h"
#include "DUOLGameEngine/Manager/CameraEventManager.h"
#include "DUOLGameEngine/Manager/TimeManager.h"

using namespace rttr;

RTTR_REGISTRATION
{
	rttr::registration::class_<DUOLClient::Player>("Player")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.property("_currentOverdrivePoint", &DUOLClient::Player::_currentOverdrivePoint)
	(
		metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
	);
}

namespace DUOLClient
{
	Player::Player(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		CharacterBase(owner, name)
		, _defaultSwordDamage(2.f)
		, _defaultPunchDamage(2.f)
		, _defaultOverdriveDamage(3.f)
		, _defaultSwordAttackSpeed(1.f)
		, _defaultPunchAttackSpeed(1.f)
		, _defaultOverdriveAttackSpeed(1.f)
		, _defaultMaxMoveSpeed(4.f)
		, _defaultMaxRunSpeed(6.f)
		, _defaultMaxLockOnMoveSpeed(2.f)
		, _defaultMaxLockOnRunSpeed(4.f)
		, _currentDamage(20.f * 2.f)
		, _currentMoveSpeed(0.f)
		, _inAttackPostDelay(0.2f)
		, _endAttackPostDelay(0.5f)
		, _currentDownPoint(0.f)
		, _currentOverdrivePoint(0.f)
		, _canStartAttack(true)
		, _canStartDash(true)
		, _isLockOnMode(false)
		, _isOverdriveSwordMode(false)
		, _isOverdriveFistMode(false)
		, _isDash(false)
		, _isSuperArmor(false)
		, _playerTransform(nullptr)
		, _playerAnimator(nullptr)
		, _playerRigidbody(nullptr)
		, _cameraTransform(nullptr)
	{
		SetHP(1000.f);
	}

	Player::~Player()
	{
	}

	bool Player::Attack(CharacterBase* other, float damage, AttackType attackType)
	{
		// OnHit 호출
		return other->OnHit(this, damage, attackType);
	}

	bool Player::OnHit(CharacterBase* other, float damage, AttackType attackType)
	{
		auto& currentStateName = _playerStateMachine.GetCurrentState()->GetName();

		// 무적인 상황에 대해서는 넘어가 ..!
		if (currentStateName == TEXT("PlayerState_Die") || currentStateName == TEXT("PlayerState_Down")
			|| currentStateName == TEXT("PlayerState_Dash") || currentStateName == TEXT("PlayerState_Interaction"))
			return false;

		DUOLGameEngine::StateBase* prevState = _playerStateMachine.GetPrevState();

		auto& prevStateName = prevState->GetName();

		// 아직 트랜지션 중이고 현재 상태는 Idle, 이전 상태가 Hit 이면 .. 무적 !
		if (_playerAnimator->IsOnTransition() == true && currentStateName == TEXT("PlayerState_Idle")
			&& prevStateName == TEXT("PlayerState_Hit"))
		{
			return false;
		}

		_hp -= damage;

		_currentDownPoint += DOWN_POINT_PER_ATTACK;

		// 슈퍼아머인 경우도 있을텐데 ..
		DUOLClient::PlayerState_Hit* hitState = reinterpret_cast<DUOLClient::PlayerState_Hit*>(_playerStateMachine.GetState(TEXT("PlayerState_Hit")));

		// HP 가 0보다 작아졌다.
		if (_hp <= 0)
		{
			_playerStateMachine.TransitionTo(TEXT("PlayerState_Die"), 0.f);
		}
		// 슈퍼아머 ..!
		else if (_isSuperArmor)
		{
			// 피격 모션 노노
			/*if (!_isInSuperArmorRimLight)
			{
				StartCoroutine(&DUOLClient::Player::SuperArmorRimLight);
			}*/
		}
		// 다운 게이지가 꽉 차면 Down state로 ..!
		else if (_currentDownPoint >= MAX_DOWN_POINT)
		{
			_playerStateMachine.TransitionTo(TEXT("PlayerState_Down"), 0.f);
		}
		else if (currentStateName != TEXT("PlayerState_Hit"))
		{
			// Hit state 가 아니라면 Hit state로 ..!
			hitState->SetCurrentAttackType(attackType);

			_playerStateMachine.TransitionTo(TEXT("PlayerState_Hit"), 0.f);
		}
		// 중복 히트 (원래는 애니메이션 변동이였는데 .. 이제 안 바뀐다 ..!)
		else
		{
			hitState->SetCurrentAttackType(attackType);

			// Hit state 라면 중복 히트 적용
			hitState->AccumulateHit();
		}

		// UI Change
		DUOLClient::UIDataManager::GetInstance()->SetPlayerHPUI(_hp);
		DUOLClient::UIDataManager::GetInstance()->SetPlayerOverDriveUI(_currentOverdrivePoint);
		//DUOLClient::UIDataManager::GetInstance()->SetUltimateUI(_currentDownPoint);

		return true;
	}

	bool Player::GetIsInvincible()
	{
		auto& currentStateName = _playerStateMachine.GetCurrentState()->GetName();

		if (currentStateName == TEXT("PlayerState_Die") || currentStateName == TEXT("PlayerState_Down")
			|| currentStateName == TEXT("PlayerState_Dash") || currentStateName == TEXT("PlayerState_Interaction"))
			return true;

		DUOLGameEngine::StateBase* prevState = _playerStateMachine.GetPrevState();

		auto& prevStateName = prevState->GetName();

		if (_playerAnimator->IsOnTransition() == true && currentStateName == TEXT("PlayerState_Idle")
			&& prevStateName == TEXT("PlayerState_Hit"))
		{
			return true;
		}

		return _isDash;
	}

	void Player::InitializeStateMachine()
	{
		// Using other game object (e.x. main camera's transform) caching
		auto& allGOs = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

		// Main cameras transform.
		for (auto gameObject : allGOs)
		{
			// 카메라 오브젝트
			if (gameObject->GetTag() == TEXT("MainCamera"))
			{
				_cameraTransform = gameObject->GetTransform();

				// Main Camera Controller 는 여기에 달려있습니다.
				_mainCamController = gameObject->GetTransform()->GetParent()->GetGameObject()->GetComponent<DUOLClient::MainCameraController>();
			}
			// 검 오브젝트
			else if (gameObject->GetTag() == TEXT("Weapon_Sword"))
			{
				_playerWeaponSword = gameObject->GetComponent<DUOLClient::Weapon_Sword>();
				_currentPlayerWeapon = _playerWeaponSword;
				_playerWeaponSwordCollider = gameObject->GetComponent<DUOLGameEngine::BoxCollider>();
				_currentplayerWeaponSwordCollider = _playerWeaponSwordCollider;
			}
			else if (gameObject->GetTag() == TEXT("Weapon_Sword_Overdrive"))
			{
				_playerOverdriveWeaponSword = gameObject->GetComponent<DUOLClient::Weapon_Sword>();
				_playerOverdriveWeaponSwordCollider = gameObject->GetComponent<DUOLGameEngine::BoxCollider>();
				_playerOverdriveWeaponSwordCollider->OnDisable();
				_playerOverdriveWeaponSword->OffSword();
			}
			else if (gameObject->GetTag() == TEXT("FistFormHolder_L"))
			{
				_playerLeftFistHolder = gameObject;
			}
			else if (gameObject->GetTag() == TEXT("FistFormHolder_R"))
			{
				_playerRightFistHolder = gameObject;
			}
			else if (gameObject->GetName() == TEXT("player_test"))
			{
				_playerSkinnedMeshRenderer = gameObject->GetComponent<DUOLGameEngine::SkinnedMeshRenderer>();
			}
		}

		// 충격파 오브젝트
		DUOLGameEngine::GameObject* weaponWave = GetGameObject()->GetScene()->CreateEmpty();

		weaponWave->SetName(TEXT("Weapon_Wave"));

		_playerWeaponWave = weaponWave->AddComponent<DUOLClient::Weapon_Wave>();

		_playerWeaponWave->_player = this;

		// 영역 충격파 오브젝트
		DUOLGameEngine::GameObject* weaponAreaWave = GetGameObject()->GetScene()->CreateEmpty();

		weaponAreaWave->SetName(TEXT("Weapon_AreaWave"));

		_playerWeaponAreaWave = weaponAreaWave->AddComponent<DUOLClient::Weapon_AreaWave>();

		_playerWeaponAreaWave->_player = this;

		_playerTransform = GetGameObject()->GetTransform();

		_playerAnimator = GetGameObject()->GetComponent<DUOLGameEngine::Animator>();

		_playerRigidbody = GetGameObject()->GetComponent<DUOLGameEngine::Rigidbody>();

#pragma region ADD_ALL_STATE
		PlayerState_Idle* idle = _playerStateMachine.AddState<PlayerState_Idle>(this);

		PlayerState_Move* move = _playerStateMachine.AddState<PlayerState_Move>(this);

		PlayerState_Run* run = _playerStateMachine.AddState<PlayerState_Run>(this);

		PlayerState_Dash* dash = _playerStateMachine.AddState<PlayerState_Dash>(this);

		PlayerState_Attack* attack = _playerStateMachine.AddState<PlayerState_Attack>(this);

		PlayerState_Hit* hit = _playerStateMachine.AddState<PlayerState_Hit>(this);

		PlayerState_Down* down = _playerStateMachine.AddState<PlayerState_Down>(this);

		PlayerState_Die* die = _playerStateMachine.AddState<PlayerState_Die>(this);

		PlayerState_Overdrive* overdrive = _playerStateMachine.AddState<PlayerState_Overdrive>(this);

		PlayerState_Ultimate* ult = _playerStateMachine.AddState<PlayerState_Ultimate>(this);
#pragma endregion

#pragma region 
		DUOLClient::UIDataManager::GetInstance()->SetPlayerHPMaxUI(_hp);
		DUOLClient::UIDataManager::GetInstance()->SetPlayerHPUI(_hp);

		DUOLClient::UIDataManager::GetInstance()->SetPlayerOverDriveMaxUI(100.f);
		DUOLClient::UIDataManager::GetInstance()->SetPlayerOverDriveUI(_currentOverdrivePoint);
#pragma endregion
	}

	DUOLGameEngine::CoroutineHandler Player::SuperArmorRimLight()
	{
		_isInSuperArmorRimLight = true;

		_playerSkinnedMeshRenderer->SetRimColor(DUOLMath::Vector3(0.f, 211.f, 255.f));

		_playerSkinnedMeshRenderer->SetRimLight(true);

		float upTime = 0.25f;

		float downTime = 0.25f;

		float powerPerSecond = 100.f;

		while (true)
		{
			float deltaTime = DUOLGameEngine::TimeManager::GetInstance()->GetDeltaTime();

			if (upTime >= 0.f)
			{
				upTime -= deltaTime;

				_playerSkinnedMeshRenderer->SetRimPower(_playerSkinnedMeshRenderer->GetRimPower() + powerPerSecond * deltaTime);
			}
			else
			{
				downTime -= deltaTime;

				_playerSkinnedMeshRenderer->SetRimPower(_playerSkinnedMeshRenderer->GetRimPower() - powerPerSecond * deltaTime);
			}

			// 끝.
			if (downTime <= 0.f)
				break;

			co_yield nullptr;
		}

		_playerSkinnedMeshRenderer->SetRimPower(0.f);

		_playerSkinnedMeshRenderer->SetRimLight(false);

		_isInSuperArmorRimLight = false;
	}

	void Player::SetSuperArmor(bool value)
	{
		_isSuperArmor = value;
	}

	void Player::LoadAudioClip()
	{
		auto soundManager = DUOLGameEngine::SoundManager::GetInstance();

		//순서대로 넣을 것.
		_audioClips.push_back(soundManager->GetAudioClip(TEXT("FootStep01")));
		_audioClips.push_back(soundManager->GetAudioClip(TEXT("FootStep02")));
		_audioClips.push_back(soundManager->GetAudioClip(TEXT("avoidSound")));
		_audioClips.push_back(soundManager->GetAudioClip(TEXT("Slash_One")));
		_audioClips.push_back(soundManager->GetAudioClip(TEXT("Slash_Two")));
		_audioClips.push_back(soundManager->GetAudioClip(TEXT("Slash_Three")));
		_audioClips.push_back(soundManager->GetAudioClip(TEXT("Slash_Final")));
		_audioClips.push_back(soundManager->GetAudioClip(TEXT("Fist_One")));
		_audioClips.push_back(soundManager->GetAudioClip(TEXT("Fist_Two")));
		_audioClips.push_back(soundManager->GetAudioClip(TEXT("Normal_Last_Punch")));
		_audioClips.push_back(soundManager->GetAudioClip(TEXT("MagnumPunch")));
		_audioClips.push_back(soundManager->GetAudioClip(TEXT("Overdrive_Fist_One")));
		_audioClips.push_back(soundManager->GetAudioClip(TEXT("Overdrive_Fist_Two")));
		_audioClips.push_back(soundManager->GetAudioClip(TEXT("OverdriveSword01")));
		_audioClips.push_back(soundManager->GetAudioClip(TEXT("OverdriveSword02")));
		_audioClips.push_back(soundManager->GetAudioClip(TEXT("OverdriveSwordFinal")));
		_audioClips.push_back(soundManager->GetAudioClip(TEXT("UltimateMagnumPunch")));
		_audioClips.push_back(soundManager->GetAudioClip(TEXT("SwordChargingSound01")));
		_audioClips.push_back(soundManager->GetAudioClip(TEXT("SwordChargingSound02")));
		_audioClips.push_back(soundManager->GetAudioClip(TEXT("Sword_FinalAttack_Preset01")));
		_audioClips.push_back(soundManager->GetAudioClip(TEXT("Sword_FinalAttack_Preset02")));
		_audioClips.push_back(soundManager->GetAudioClip(TEXT("Sword_FinalAttack")));
		_audioClips.push_back(soundManager->GetAudioClip(TEXT("FFF_First_Preset01")));
		_audioClips.push_back(soundManager->GetAudioClip(TEXT("SFF_Second_Preset01")));
		_audioClips.push_back(soundManager->GetAudioClip(TEXT("SFF_Second_Preset02")));
		_audioClips.push_back(soundManager->GetAudioClip(TEXT("SFF_Third_Preset01")));
		_audioClips.push_back(soundManager->GetAudioClip(TEXT("SFF_Third_Preset02")));
		_audioClips.push_back(soundManager->GetAudioClip(TEXT("SSFF_Third_Preset01")));
		_audioClips.push_back(soundManager->GetAudioClip(TEXT("SSFF_Third_Preset02")));
		_audioClips.push_back(soundManager->GetAudioClip(TEXT("SSFF_Fourth_Preset01")));
		_audioClips.push_back(soundManager->GetAudioClip(TEXT("SSFF_Fourth_Preset02")));
		_audioClips.push_back(soundManager->GetAudioClip(TEXT("SSSF_Preset01")));
		_audioClips.push_back(soundManager->GetAudioClip(TEXT("SSSF_Preset02")));
		_audioClips.push_back(soundManager->GetAudioClip(TEXT("Overdrive_Fist_Preset01_FirstCombo")));
		_audioClips.push_back(soundManager->GetAudioClip(TEXT("Overdrive_Fist_Preset02_SecondCombo")));
		_audioClips.push_back(soundManager->GetAudioClip(TEXT("Overdrive_Fist_Preset03_ThirdCombo")));
		_audioClips.push_back(soundManager->GetAudioClip(TEXT("AuraSound")));
		_audioClips.push_back(soundManager->GetAudioClip(TEXT("OverdriveFist01")));
		_audioClips.push_back(soundManager->GetAudioClip(TEXT("OverdriveFist02")));

		_audioListener = GetGameObject()->GetComponent<DUOLGameEngine::AudioListener>();
		_audioSource = GetGameObject()->GetComponent<DUOLGameEngine::AudioSource>();

#pragma region ATTACK_SFX_EVENT
		AddEventFunction(TEXT("ATK_SFX_Slash1"), std::bind(&DUOLClient::Player::PlaySoundClip, this, PlayerSoundTable::Slash_One, false));
		AddEventFunction(TEXT("ATK_SFX_Slash2"), std::bind(&DUOLClient::Player::PlaySoundClip, this, PlayerSoundTable::Slash_Two, false));
		AddEventFunction(TEXT("ATK_SFX_Slash3"), std::bind(&DUOLClient::Player::PlaySoundClip, this, PlayerSoundTable::Slash_Three, false));
		AddEventFunction(TEXT("ATK_SFX_Slash4"), std::bind(&DUOLClient::Player::PlaySoundClip, this, PlayerSoundTable::Slash_Final, false));

		AddEventFunction(TEXT("ATK_SFX_Fist1"), std::bind(&DUOLClient::Player::PlaySoundClip, this, PlayerSoundTable::Fist_One, false));
		AddEventFunction(TEXT("ATK_SFX_Fist2"), std::bind(&DUOLClient::Player::PlaySoundClip, this, PlayerSoundTable::Fist_Two, false));
		AddEventFunction(TEXT("ATK_SFX_LP"), std::bind(&DUOLClient::Player::PlaySoundClip, this, PlayerSoundTable::Normal_Last_Punch, false));

		AddEventFunction(TEXT("ATK_SFX_MAGP"), std::bind(&DUOLClient::Player::PlaySoundClip, this, PlayerSoundTable::MagnumPunch, false));
#pragma endregion

#pragma region Overdrive_SFX_Effect
		AddEventFunction(TEXT("ATK_SFX_ODS1"), std::bind(&DUOLClient::Player::PlaySoundClip, this, PlayerSoundTable::OverdriveSword01, false));
		AddEventFunction(TEXT("ATK_SFX_ODS2"), std::bind(&DUOLClient::Player::PlaySoundClip, this, PlayerSoundTable::OverdriveSword02, false));
		AddEventFunction(TEXT("ATK_SFX_ODS3"), std::bind(&DUOLClient::Player::PlaySoundClip, this, PlayerSoundTable::OverdriveSwordFinal, false));

		AddEventFunction(TEXT("ATK_SFX_ODF1"), std::bind(&DUOLClient::Player::PlaySoundClip, this, PlayerSoundTable::Overdrive_Fist_One, false));
		AddEventFunction(TEXT("ATK_SFX_ODF2"), std::bind(&DUOLClient::Player::PlaySoundClip, this, PlayerSoundTable::Overdrive_Fist_Two, false));

		AddEventFunction(TEXT("ATK_SFX_SCS1"), std::bind(&DUOLClient::Player::PlaySoundClip, this, PlayerSoundTable::SwordChargingSound01, false));
		AddEventFunction(TEXT("ATK_SFX_SCS2"), std::bind(&DUOLClient::Player::PlaySoundClip, this, PlayerSoundTable::SwordChargingSound02, false));
#pragma endregion

#pragma region CreateSoundModule

		auto holder = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->CreateEmpty();
		holder->GetTransform()->SetParent(GetTransform(), false);
		holder->GetTransform()->SetPosition({ 0,0,0 });
		holder->SetName(TEXT("PlayerSoundModule"));

		for (int idx = 0; idx < MAX_SOUND_PLAYER; idx++)
		{
			DUOLCommon::tstring name = TEXT("SoundModule");

			auto object = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->CreateEmpty();
			object->GetTransform()->SetParent(holder->GetTransform());
			object->SetName(name + DUOLCommon::StringHelper::ToTString(idx));
			auto comp = object->AddComponent<DUOLGameEngine::AudioSource>();

			_soundModules.push_back(comp);
		}

		auto object = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->CreateEmpty();
		object->GetTransform()->SetParent(holder->GetTransform());
		object->SetName(TEXT("AuraSoundModule"));
		auto comp = object->AddComponent<DUOLGameEngine::AudioSource>();
		_auraSource = comp;

		_auraSource->SetAudioClip(_audioClips[static_cast<int>(PlayerSoundTable::AuraSound)]);

#pragma endregion
	}

	void Player::OnStart()
	{

		LoadAudioClip();
		// State Machine 을 초기화합니다.
		InitializeStateMachine();
	}

	void Player::OnUpdate(float deltaTime)
	{

		if (DUOLClient::GameManager::GetInstance()->IsInUIMode())
			return;

		// 모든 기타 사항에 대해서 갱신을 마무리하고, 플레이어의 스테이트 머신을 갱신합니다.
		_playerStateMachine.UpdateStateMachine(deltaTime);

		// 슈퍼 아머이고, 림라이트 안 켜져 있으면 온.
		if (_isSuperArmor && !_isInSuperArmorRimLight)
			StartCoroutine(&DUOLClient::Player::SuperArmorRimLight);
	}

	void Player::OnLateUpdate(float deltaTime)
	{
		if (DUOLClient::GameManager::GetInstance()->IsInUIMode())
			return;

		_playerStateMachine.LateUpdateStateMachine(deltaTime);
	}

	void Player::OnFixedUpdate(float fixedTimeStep)
	{
		if (DUOLClient::GameManager::GetInstance()->IsInUIMode())
			return;

		// 스테이트 머신의 물리 갱신
		_playerStateMachine.FixedUpdateStateMachine(fixedTimeStep);
	}

	void Player::OnTriggerStay(const std::shared_ptr<DUOLPhysics::Trigger>& trigger)
	{
		DUOLGameEngine::GameObject* otherGameObject = reinterpret_cast<DUOLGameEngine::GameObject*>(trigger->_other);

		if (otherGameObject->GetLayer() == TEXT("Enemy"))
		{
			Enemy* enemy = otherGameObject->GetComponent<Enemy>();

			if (enemy)
				enemy->PushedOut(GetTransform()->GetWorldPosition());
		}
	}

	void Player::PlaySoundClip(PlayerSoundTable soundClip, bool isLoop)
	{
		int soundIdx = static_cast<int>(soundClip);
		if (0 > soundIdx || soundIdx > (static_cast<int>(PlayerSoundTable::NONE) - 1))
			return;

		_audioSource->SetAudioClip(_audioClips[soundIdx]);
		_audioSource->SetIsLoop(isLoop);
		_audioSource->Play();
	}

	void Player::PlaySoundClipInModule(DUOLGameEngine::AudioClip* soundClip, int idx, bool isLoop)
	{
		_soundModules[idx]->SetAudioClip(soundClip);
		_soundModules[idx]->SetIsLoop(isLoop);
		_soundModules[idx]->Play();
	}

	void Player::PlaySoundClipInModule(PlayerSoundTable soundClip, int idx, bool isLoop)
	{
		int soundIdx = static_cast<int>(soundClip);
		if (0 > soundIdx || soundIdx > (static_cast<int>(PlayerSoundTable::NONE) - 1))
			return;

		_soundModules[idx]->SetAudioClip(_audioClips[soundIdx]);
		_soundModules[idx]->SetIsLoop(isLoop);
		_soundModules[idx]->Play();
	}

	void Player::AddOverdrivePoint(float point)
	{
		_currentOverdrivePoint += point;
		_currentOverdrivePoint = std::clamp(_currentOverdrivePoint, 0.f, 100.f);

		DUOLClient::UIDataManager::GetInstance()->SetPlayerOverDriveUI(_currentOverdrivePoint);
	}

	DUOLGameEngine::AudioSource* Player::GetAuraSoundSource()
	{
		if (_auraSource != nullptr)
			return _auraSource;

		return nullptr;
	}
}
