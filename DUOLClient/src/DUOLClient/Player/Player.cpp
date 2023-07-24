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
#include "DUOLClient/Player/FSM//PlayerState_Jump.h"
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
#include "DUOLJson/JsonReader.h"

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
	
	float DUOLClient::Player::_currentOverdrivePoint = 0.f;

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
		, _currentDamage(10)
		, _currentMoveSpeed(0.f)
		, _inAttackPostDelay(0.2f)
		, _endAttackPostDelay(0.5f)
		, _currentDownPoint(0.f)
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
		, _playerHitAnimationSpeed(1.0f)
		, _isDashCrowdControl(false)
		, _canInteraction(true)
		, _superArmorDamageDecrease(0.8f)
		, _overdriveDuration(10.f)
		, _overdrivePointPerSword(3.f)
		, _overdrivePointPerFist(4.f)
		, _downPointResetTime(4.f)
		, _currentDownPointResetTime(0.f)
		, _isOneTimeUltimate(false)
	{
	}

	Player::~Player()
	{
	}

	bool Player::Attack(CharacterBase* other, float damage, AttackType attackType, float downPoint)
	{
		// OnHit 호출
		return other->OnHit(this, damage, attackType, downPoint);

	}

	bool Player::OnHit(CharacterBase* other, float damage, AttackType attackType, float downPoint)
	{
		auto& currentStateName = _playerStateMachine.GetCurrentState()->GetName();

		//다운포인트 리셋타임을 초기화합니다.
		_currentDownPointResetTime = 0;

		// 무적인 상황에 대해서는 넘어가 ..!
		if (currentStateName == TEXT("PlayerState_Die") || currentStateName == TEXT("PlayerState_Down")
			|| currentStateName == TEXT("PlayerState_Dash") || currentStateName == TEXT("PlayerState_Interaction") 
			|| currentStateName == TEXT("PlayerState_Ultimate") || currentStateName == TEXT("PlayerState_Overdrive"))
			return false;

		DUOLGameEngine::StateBase* prevState = _playerStateMachine.GetPrevState();

		auto& prevStateName = prevState->GetName();

		// 아직 트랜지션 중이고 현재 상태는 Idle, 이전 상태가 Hit 이면 .. 무적 ! 혹은 궁극기를 쓰고있다면...
		if ((_playerAnimator->IsOnTransition() == true && currentStateName == TEXT("PlayerState_Idle") && prevStateName == TEXT("PlayerState_Hit")) || _isUltimate )
		{
			return false;
		}

		if(_isSuperArmor)
			_hp -= damage * _superArmorDamageDecrease;
		else
			_hp -= damage;

		PlaySoundClip(PlayerSoundTable::Hit_Sound_Effect, false);

		_currentDownPoint += downPoint;

		// 슈퍼아머인 경우도 있을텐데 ..
		DUOLClient::PlayerState_Hit* hitState = reinterpret_cast<DUOLClient::PlayerState_Hit*>(_playerStateMachine.GetState(TEXT("PlayerState_Hit")));

		// HP 가 0보다 작아졌다.
		if (_hp <= 0)
		{
			int randnum = rand();
			if (randnum % 2)
				PlayVoiceSoundClip(PlayerVoiceSoundTable::Voice_Grogi01, false);
			else
				PlayVoiceSoundClip(PlayerVoiceSoundTable::Voice_Grogi02, false);

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
			int randnum = rand();
			if (randnum % 2)
				PlayVoiceSoundClip(PlayerVoiceSoundTable::Voice_Knock01, false);
			else
				PlayVoiceSoundClip(PlayerVoiceSoundTable::Voice_Knock02, false);

			_playerStateMachine.TransitionTo(TEXT("PlayerState_Down"), 0.f);
		}
		else if (currentStateName != TEXT("PlayerState_Hit"))
		{
			// Hit state 가 아니라면 Hit state로 ..!
			hitState->SetCurrentAttackType(attackType);
			int randnum = rand();
			if (randnum % 2)
				PlayVoiceSoundClip(PlayerVoiceSoundTable::Voice_Hit01, false);
			else
				PlayVoiceSoundClip(PlayerVoiceSoundTable::Voice_Hit02, false);

			_playerStateMachine.TransitionTo(TEXT("PlayerState_Hit"), 0.f);
		}
		// 중복 히트 (원래는 애니메이션 변동이였는데 .. 이제 안 바뀐다 ..!)
		else
		{
			hitState->SetCurrentAttackType(attackType);

			// Hit state 라면 중복 히트 적용
			hitState->AccumulateHit();
		}

		if(_hp<=0)
			DUOLClient::UIDataManager::GetInstance()->SetPlayerHPUI(0.f);
		else
			DUOLClient::UIDataManager::GetInstance()->SetPlayerHPUI(_hp);

		// UI Change
		//DUOLClient::UIDataManager::GetInstance()->SetPlayerHPUI(_hp);
		DUOLClient::UIDataManager::GetInstance()->SetPlayerOverDriveUI(_currentOverdrivePoint);



		return true;
	}

	void Player::SetDashUsable(bool dash)
	{
		_isDashCrowdControl = dash;
	}

	void Player::SetCurrentOverdrivePoint(float value)
	{
		_currentOverdrivePoint = value;
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
				_currentplayerWeaponSwordCollider->OnDisable();
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
			else if (gameObject->GetName() == TEXT("player"))
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
		_playerRigidbody->SetMass(10.f);

		_playerRigidbody->SetMaxLinearVelocity(30.f);
		_playerRigidbody->SetMaxDepenetrationVelocity(4.f);
		_playerRigidbody->SetCenterOfMass({ 0, 1.25f, 0 });
		_playerRigidbody->SetIsInterpolate(true);

#pragma region PlayerData Load
		auto playerDamageTable = DUOLJson::JsonReader::GetInstance()->LoadJson(TEXT("Asset/DataTable/PlayerDataTable.json"));

		const TCHAR* attackName = _T("Name");
		const TCHAR* damageName = _T("Damage");

		auto tableArray = playerDamageTable->GetArray();
		_playerDataTable.reserve(tableArray.Size());

		for (auto& damageList : tableArray)
		{
			DUOLCommon::tstring name;
			float damage = 0;

			if (damageList.HasMember(attackName))
			{
				name = damageList[attackName].GetString();
			}

			if (damageList.HasMember(damageName))
			{
				damage = damageList[damageName].GetFloat();
			}

			_playerDataTable.emplace(name, damage);
		}

		DUOLJson::JsonReader::GetInstance()->UnloadJson(TEXT("Asset/DataTable/PlayerDataTable.json"));
#pragma endregion

#pragma region PlayerData Set
		if (_playerDataTable.contains(TEXT("HP")))
			_hp = _playerDataTable.find(TEXT("HP"))->second;
		if (_playerDataTable.contains(TEXT("MoveSpeed")))
			_defaultMaxMoveSpeed = _playerDataTable.find(TEXT("MoveSpeed"))->second;
		if (_playerDataTable.contains(TEXT("RunSpeed")))
			_defaultMaxRunSpeed = _playerDataTable.find(TEXT("RunSpeed"))->second;
		if (_playerDataTable.contains(TEXT("MaxLockOnMoveSpeed")))
			_defaultMaxLockOnMoveSpeed = _playerDataTable.find(TEXT("MaxLockOnMoveSpeed"))->second;
		if (_playerDataTable.contains(TEXT("MaxLockOnRunSpeed")))
			_defaultMaxLockOnRunSpeed = _playerDataTable.find(TEXT("MaxLockOnRunSpeed"))->second;
		if (_playerDataTable.contains(TEXT("OverdriveTime")))
			_overdriveDuration = _playerDataTable.find(TEXT("OverdriveTime"))->second;
		if (_playerDataTable.contains(TEXT("OverdrivePointPerSword")))
			_overdrivePointPerSword = _playerDataTable.find(TEXT("OverdrivePointPerSword"))->second;
		if (_playerDataTable.contains(TEXT("OverdrivePointPerFist")))
			_overdrivePointPerFist = _playerDataTable.find(TEXT("OverdrivePointPerFist"))->second;
		if (_playerDataTable.contains(TEXT("SuperArmorDamageDecrease")))
			_superArmorDamageDecrease = _playerDataTable.find(TEXT("SuperArmorDamageDecrease"))->second;
#pragma endregion

#pragma region ADD_ALL_STATE
		PlayerState_Idle* idle = _playerStateMachine.AddState<PlayerState_Idle>(this);

		PlayerState_Move* move = _playerStateMachine.AddState<PlayerState_Move>(this);

		PlayerState_Run* run = _playerStateMachine.AddState<PlayerState_Run>(this);

		PlayerState_Dash* dash = _playerStateMachine.AddState<PlayerState_Dash>(this);

		PlayerState_Attack* attack = _playerStateMachine.AddState<PlayerState_Attack>(this);
		//콤보트리 이니셜라이즈
		attack->BuildComboTree(_playerDataTable);

		PlayerState_Hit* hit = _playerStateMachine.AddState<PlayerState_Hit>(this);

		PlayerState_Down* down = _playerStateMachine.AddState<PlayerState_Down>(this);

		PlayerState_Die* die = _playerStateMachine.AddState<PlayerState_Die>(this);

		PlayerState_Overdrive* overdrive = _playerStateMachine.AddState<PlayerState_Overdrive>(this);

		PlayerState_Ultimate* ult = _playerStateMachine.AddState<PlayerState_Ultimate>(this);
		if (_playerDataTable.contains(TEXT("UltimateSword_S")))
			ult->SetUltimateSwordDamage(_playerDataTable.find(TEXT("UltimateSword_S"))->second);
		if (_playerDataTable.contains(TEXT("UltimateSword_SSSSSS")))
			ult->SetUltimateSwordWaveDamage(_playerDataTable.find(TEXT("UltimateSword_SSSSSS"))->second);
		if (_playerDataTable.contains(TEXT("UltimateFist_F")))
			ult->SetUltimateFistDamage(_playerDataTable.find(TEXT("UltimateFist_F"))->second);

		PlayerState_Jump* jump = _playerStateMachine.AddState<PlayerState_Jump>(this);
#pragma endregion

#pragma region 
		DUOLClient::UIDataManager::GetInstance()->SetPlayerHPMaxUI(_hp);
		DUOLClient::UIDataManager::GetInstance()->SetPlayerHPUI(_hp);

		DUOLClient::UIDataManager::GetInstance()->SetPlayerOverDriveMaxUI(100.f);
		DUOLClient::UIDataManager::GetInstance()->SetPlayerOverDriveUI(_currentOverdrivePoint);

		DUOLClient::UIDataManager::GetInstance()->SetUltimateMaxUI(100.f);
		DUOLClient::UIDataManager::GetInstance()->SetUltimateUI(0.f);
#pragma endregion
	}

	DUOLGameEngine::CoroutineHandler Player::SuperArmorRimLight()
	{
		_isInSuperArmorRimLight = true;

		_playerSkinnedMeshRenderer->SetRimColor(DUOLMath::Vector3(13.f/255.f, 75 / 255.f, 101.f / 255.f));

		_playerSkinnedMeshRenderer->SetRimLight(true);

		float upTime = 0.25f;

		float downTime = 0.25f;

		float powerPerSecond = 30.f;

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
		_attackAudioClips.reserve(static_cast<int>(PlayerSoundTable::NONE));
		//순서대로 넣을 것.
		_attackAudioClips.push_back(soundManager->GetAudioClip(TEXT("FootStep01")));
		_attackAudioClips.push_back(soundManager->GetAudioClip(TEXT("FootStep02")));
		_attackAudioClips.push_back(soundManager->GetAudioClip(TEXT("avoidSound")));
		_attackAudioClips.push_back(soundManager->GetAudioClip(TEXT("Slash_One")));
		_attackAudioClips.push_back(soundManager->GetAudioClip(TEXT("Slash_Two")));

		_attackAudioClips.push_back(soundManager->GetAudioClip(TEXT("Slash_Three")));
		_attackAudioClips.push_back(soundManager->GetAudioClip(TEXT("Slash_Final")));
		_attackAudioClips.push_back(soundManager->GetAudioClip(TEXT("Fist_One")));
		_attackAudioClips.push_back(soundManager->GetAudioClip(TEXT("Fist_Two")));
		_attackAudioClips.push_back(soundManager->GetAudioClip(TEXT("Normal_Last_Punch")));

		_attackAudioClips.push_back(soundManager->GetAudioClip(TEXT("MagnumPunch")));
		_attackAudioClips.push_back(soundManager->GetAudioClip(TEXT("MagnumPunch_Over")));

		_attackAudioClips.push_back(soundManager->GetAudioClip(TEXT("Overdrive_Fist_One")));
		_attackAudioClips.push_back(soundManager->GetAudioClip(TEXT("Overdrive_Fist_Two")));
		_attackAudioClips.push_back(soundManager->GetAudioClip(TEXT("OverdriveSword01")));
		_attackAudioClips.push_back(soundManager->GetAudioClip(TEXT("OverdriveSword02")));
		_attackAudioClips.push_back(soundManager->GetAudioClip(TEXT("OverdriveSwordFinal")));

		_attackAudioClips.push_back(soundManager->GetAudioClip(TEXT("UltimateMagnumPunch")));
		_attackAudioClips.push_back(soundManager->GetAudioClip(TEXT("SwordChargingSound01")));
		_attackAudioClips.push_back(soundManager->GetAudioClip(TEXT("SwordChargingSound02")));
		_attackAudioClips.push_back(soundManager->GetAudioClip(TEXT("FistChargingSound")));
		_attackAudioClips.push_back(soundManager->GetAudioClip(TEXT("Hit_Sound")));

		_attackAudioClips.push_back(soundManager->GetAudioClip(TEXT("AuraSound")));
		_attackAudioClips.push_back(soundManager->GetAudioClip(TEXT("OverdriveFist01")));
		_attackAudioClips.push_back(soundManager->GetAudioClip(TEXT("OverdriveFist02")));
		_attackAudioClips.push_back(soundManager->GetAudioClip(TEXT("Hit_Sound_Effect")));
		_attackAudioClips.push_back(soundManager->GetAudioClip(TEXT("Hitting_Ground")));

		_voiceAudioClips.reserve(static_cast<int>(PlayerVoiceSoundTable::NONE));

		_voiceAudioClips.push_back(soundManager->GetAudioClip(TEXT("Voice_Dash")));
		_voiceAudioClips.push_back(soundManager->GetAudioClip(TEXT("Voice_Hit01")));
		_voiceAudioClips.push_back(soundManager->GetAudioClip(TEXT("Voice_Hit02")));
		_voiceAudioClips.push_back(soundManager->GetAudioClip(TEXT("Voice_Knock01")));
		_voiceAudioClips.push_back(soundManager->GetAudioClip(TEXT("Voice_Knock02")));

		_voiceAudioClips.push_back(soundManager->GetAudioClip(TEXT("Voice_Grogi01")));
		_voiceAudioClips.push_back(soundManager->GetAudioClip(TEXT("Voice_Grogi02")));

		_voiceAudioClips.push_back(soundManager->GetAudioClip(TEXT("Voice_NormalAttack01")));
		_voiceAudioClips.push_back(soundManager->GetAudioClip(TEXT("Voice_NormalAttack03")));
		_voiceAudioClips.push_back(soundManager->GetAudioClip(TEXT("Voice_NormalAttack04")));
		_voiceAudioClips.push_back(soundManager->GetAudioClip(TEXT("Voice_NormalAttack05")));
		_voiceAudioClips.push_back(soundManager->GetAudioClip(TEXT("Voice_NormalAttack06")));
		_voiceAudioClips.push_back(soundManager->GetAudioClip(TEXT("Voice_NormalAttack07")));
		_voiceAudioClips.push_back(soundManager->GetAudioClip(TEXT("Voice_NormalAttack08")));
		_voiceAudioClips.push_back(soundManager->GetAudioClip(TEXT("Voice_NormalLastAttack")));

		_voiceAudioClips.push_back(soundManager->GetAudioClip(TEXT("Voice_FistActive01")));
		_voiceAudioClips.push_back(soundManager->GetAudioClip(TEXT("Voice_FistActive02")));

		_voiceAudioClips.push_back(soundManager->GetAudioClip(TEXT("Voice_LastPunch01")));
		_voiceAudioClips.push_back(soundManager->GetAudioClip(TEXT("Voice_LastPunch02")));

		_voiceAudioClips.push_back(soundManager->GetAudioClip(TEXT("Voice_MagnumPunch01")));
		_voiceAudioClips.push_back(soundManager->GetAudioClip(TEXT("Voice_MagnumPunch01jump")));
		_voiceAudioClips.push_back(soundManager->GetAudioClip(TEXT("Voice_MagnumPunch02")));
		_voiceAudioClips.push_back(soundManager->GetAudioClip(TEXT("Voice_MagnumPunch02jump")));

		_voiceAudioClips.push_back(soundManager->GetAudioClip(TEXT("Voice_UltimatePunch01")));
		_voiceAudioClips.push_back(soundManager->GetAudioClip(TEXT("Voice_UltimatePunch02")));

		_voiceAudioClips.push_back(soundManager->GetAudioClip(TEXT("Voice_UltimateSword01")));
		_voiceAudioClips.push_back(soundManager->GetAudioClip(TEXT("Voice_UltimateSword02")));
		_voiceAudioClips.push_back(soundManager->GetAudioClip(TEXT("Voice_Result_Character")));

		_voiceAudioClips.push_back(soundManager->GetAudioClip(TEXT("Voice_UltimateCharging")));


		_audioListener = GetGameObject()->GetComponent<DUOLGameEngine::AudioListener>();
		_attackSource = GetGameObject()->GetComponent<DUOLGameEngine::AudioSource>();

		///VoiceAudioClips
		_voiceAudioSets.emplace(PlayerVoiceSoundSet::NormalSword_01, std::pair{ _voiceAudioClips[static_cast<int>(PlayerVoiceSoundTable::Voice_NormalAttack04)], _voiceAudioClips[static_cast<int>(PlayerVoiceSoundTable::Voice_NormalAttack06)] });
		_voiceAudioSets.emplace(PlayerVoiceSoundSet::NormalSword_02, std::pair{ _voiceAudioClips[static_cast<int>(PlayerVoiceSoundTable::Voice_NormalAttack01)], _voiceAudioClips[static_cast<int>(PlayerVoiceSoundTable::Voice_NormalAttack05)] });
		_voiceAudioSets.emplace(PlayerVoiceSoundSet::NormalSword_03, std::pair{ _voiceAudioClips[static_cast<int>(PlayerVoiceSoundTable::Voice_NormalAttack03)], _voiceAudioClips[static_cast<int>(PlayerVoiceSoundTable::Voice_NormalAttack07)] });
		_voiceAudioSets.emplace(PlayerVoiceSoundSet::NormalSword_04, std::pair{ _voiceAudioClips[static_cast<int>(PlayerVoiceSoundTable::Voice_NormalLastAttack)], _voiceAudioClips[static_cast<int>(PlayerVoiceSoundTable::Voice_NormalLastAttack)] });

		_voiceAudioSets.emplace(PlayerVoiceSoundSet::NormalSword_SwordCombo_1_2, std::pair{ _voiceAudioClips[static_cast<int>(PlayerVoiceSoundTable::Voice_NormalAttack05)], _voiceAudioClips[static_cast<int>(PlayerVoiceSoundTable::Voice_NormalAttack08)] });
		_voiceAudioSets.emplace(PlayerVoiceSoundSet::NormalSword_SwordCombo_1_3, std::pair{ _voiceAudioClips[static_cast<int>(PlayerVoiceSoundTable::Voice_LastPunch01)], _voiceAudioClips[static_cast<int>(PlayerVoiceSoundTable::Voice_LastPunch02)] });
		_voiceAudioSets.emplace(PlayerVoiceSoundSet::NormalSword_SwordCombo_2_3, std::pair{ _voiceAudioClips[static_cast<int>(PlayerVoiceSoundTable::Voice_NormalAttack08)], _voiceAudioClips[static_cast<int>(PlayerVoiceSoundTable::Voice_NormalAttack04)] });
		_voiceAudioSets.emplace(PlayerVoiceSoundSet::NormalSword_SwordCombo_23_4Jump, std::pair{ _voiceAudioClips[static_cast<int>(PlayerVoiceSoundTable::Voice_MagnumPunch01jump)], _voiceAudioClips[static_cast<int>(PlayerVoiceSoundTable::Voice_MagnumPunch02jump)] });
		_voiceAudioSets.emplace(PlayerVoiceSoundSet::NormalSword_SwordCombo_23_4, std::pair{ _voiceAudioClips[static_cast<int>(PlayerVoiceSoundTable::Voice_MagnumPunch01)], _voiceAudioClips[static_cast<int>(PlayerVoiceSoundTable::Voice_MagnumPunch02)] });

		_voiceAudioSets.emplace(PlayerVoiceSoundSet::NormalFist_01, std::pair{ _voiceAudioClips[static_cast<int>(PlayerVoiceSoundTable::Voice_NormalAttack05)], _voiceAudioClips[static_cast<int>(PlayerVoiceSoundTable::Voice_NormalAttack06)] });
		_voiceAudioSets.emplace(PlayerVoiceSoundSet::NormalFist_02, std::pair{ _voiceAudioClips[static_cast<int>(PlayerVoiceSoundTable::Voice_NormalAttack01)], _voiceAudioClips[static_cast<int>(PlayerVoiceSoundTable::Voice_NormalAttack08)] });
		_voiceAudioSets.emplace(PlayerVoiceSoundSet::NormalFist_03, std::pair{ _voiceAudioClips[static_cast<int>(PlayerVoiceSoundTable::Voice_LastPunch01)], _voiceAudioClips[static_cast<int>(PlayerVoiceSoundTable::Voice_LastPunch02)] });

		_voiceAudioSets.emplace(PlayerVoiceSoundSet::NormalFist_FistCombo_1_2, std::pair{ _voiceAudioClips[static_cast<int>(PlayerVoiceSoundTable::Voice_NormalAttack03)], _voiceAudioClips[static_cast<int>(PlayerVoiceSoundTable::Voice_NormalAttack07)] });
		_voiceAudioSets.emplace(PlayerVoiceSoundSet::NormalFist_FistCombo_1_3, std::pair{ _voiceAudioClips[static_cast<int>(PlayerVoiceSoundTable::Voice_NormalLastAttack)], _voiceAudioClips[static_cast<int>(PlayerVoiceSoundTable::Voice_NormalLastAttack)] });
		_voiceAudioSets.emplace(PlayerVoiceSoundSet::NormalFist_FistCombo_2_3, std::pair{ _voiceAudioClips[static_cast<int>(PlayerVoiceSoundTable::Voice_NormalAttack04)], _voiceAudioClips[static_cast<int>(PlayerVoiceSoundTable::Voice_NormalAttack06)] });
		_voiceAudioSets.emplace(PlayerVoiceSoundSet::NormalFist_FistCombo_2_4, std::pair{ _voiceAudioClips[static_cast<int>(PlayerVoiceSoundTable::Voice_NormalLastAttack)], _voiceAudioClips[static_cast<int>(PlayerVoiceSoundTable::Voice_NormalLastAttack)] });

		_voiceAudioSets.emplace(PlayerVoiceSoundSet::Overdrive_Sword01, std::pair{ _voiceAudioClips[static_cast<int>(PlayerVoiceSoundTable::Voice_NormalAttack04)], _voiceAudioClips[static_cast<int>(PlayerVoiceSoundTable::Voice_NormalAttack03)] });
		_voiceAudioSets.emplace(PlayerVoiceSoundSet::Overdrive_Sword02, std::pair{ _voiceAudioClips[static_cast<int>(PlayerVoiceSoundTable::Voice_NormalAttack01)], _voiceAudioClips[static_cast<int>(PlayerVoiceSoundTable::Voice_NormalAttack07)] });
		_voiceAudioSets.emplace(PlayerVoiceSoundSet::Overdrive_SwordFin, std::pair{ _voiceAudioClips[static_cast<int>(PlayerVoiceSoundTable::Voice_MagnumPunch01)], _voiceAudioClips[static_cast<int>(PlayerVoiceSoundTable::Voice_MagnumPunch02)] });
		_voiceAudioSets.emplace(PlayerVoiceSoundSet::Overdrive_SwordUlt, std::pair{ _voiceAudioClips[static_cast<int>(PlayerVoiceSoundTable::Voice_UltimateSword01)], _voiceAudioClips[static_cast<int>(PlayerVoiceSoundTable::Voice_UltimateSword02)] });

		_voiceAudioSets.emplace(PlayerVoiceSoundSet::Overdrive_Fist01, std::pair{ _voiceAudioClips[static_cast<int>(PlayerVoiceSoundTable::Voice_NormalAttack03)], _voiceAudioClips[static_cast<int>(PlayerVoiceSoundTable::Voice_NormalAttack01)] });
		_voiceAudioSets.emplace(PlayerVoiceSoundSet::Overdrive_Fist02, std::pair{ _voiceAudioClips[static_cast<int>(PlayerVoiceSoundTable::Voice_NormalAttack05)], _voiceAudioClips[static_cast<int>(PlayerVoiceSoundTable::Voice_NormalAttack01)] });
		_voiceAudioSets.emplace(PlayerVoiceSoundSet::Overdrive_FistUlt, std::pair{ _voiceAudioClips[static_cast<int>(PlayerVoiceSoundTable::Voice_UltimatePunch01)], _voiceAudioClips[static_cast<int>(PlayerVoiceSoundTable::Voice_UltimatePunch02)] });
		_voiceAudioSets.emplace(PlayerVoiceSoundSet::Overdrive_Enter, std::pair{ _voiceAudioClips[static_cast<int>(PlayerVoiceSoundTable::Voice_FistActive02)], _voiceAudioClips[static_cast<int>(PlayerVoiceSoundTable::Voice_FistActive01)] });

#pragma region ATTACK_SFX_EVENT
		AddEventFunction(TEXT("ATK_SFX_Slash1"), std::bind(&DUOLClient::Player::PlaySoundClipAndVoice, this, PlayerSoundTable::Slash_One, PlayerVoiceSoundSet::NormalSword_01));
		AddEventFunction(TEXT("ATK_SFX_Slash2"), std::bind(&DUOLClient::Player::PlaySoundClipAndVoice, this, PlayerSoundTable::Slash_Two, PlayerVoiceSoundSet::NormalSword_02));
		AddEventFunction(TEXT("ATK_SFX_Slash3"), std::bind(&DUOLClient::Player::PlaySoundClipAndVoice, this, PlayerSoundTable::Slash_Three, PlayerVoiceSoundSet::NormalSword_03));
		AddEventFunction(TEXT("ATK_SFX_Slash4"), std::bind(&DUOLClient::Player::PlaySoundClipAndVoice, this, PlayerSoundTable::Slash_Final, PlayerVoiceSoundSet::NormalSword_04));

		AddEventFunction(TEXT("ATK_SFX_Fist1"), std::bind(&DUOLClient::Player::PlaySoundClipAndVoice, this, PlayerSoundTable::Fist_One, PlayerVoiceSoundSet::NormalFist_01));
		AddEventFunction(TEXT("ATK_SFX_Fist2"), std::bind(&DUOLClient::Player::PlaySoundClipAndVoice, this, PlayerSoundTable::Fist_Two, PlayerVoiceSoundSet::NormalFist_02));
		AddEventFunction(TEXT("ATK_SFX_LP"), std::bind(&DUOLClient::Player::PlaySoundClipAndVoice, this, PlayerSoundTable::Normal_Last_Punch, PlayerVoiceSoundSet::NormalFist_03));

		AddEventFunction(TEXT("ATK_SFX_MAGPJP"), std::bind(&DUOLClient::Player::PlaySoundClipAndVoice, this, PlayerSoundTable::NONE, PlayerVoiceSoundSet::NormalSword_SwordCombo_23_4Jump));
		AddEventFunction(TEXT("ATK_SFX_MAGP"), std::bind(&DUOLClient::Player::PlaySoundClipAndVoice, this, PlayerSoundTable::MagnumPunch, PlayerVoiceSoundSet::NormalSword_SwordCombo_23_4));
#pragma endregion

#pragma region Overdrive_SFX_Effect
		AddEventFunction(TEXT("ATK_SFX_ODS1"), std::bind(&DUOLClient::Player::PlaySoundClipAndVoice, this, PlayerSoundTable::OverdriveSword01, PlayerVoiceSoundSet::Overdrive_Sword01));
		AddEventFunction(TEXT("ATK_SFX_ODS2"), std::bind(&DUOLClient::Player::PlaySoundClipAndVoice, this, PlayerSoundTable::OverdriveSword02, PlayerVoiceSoundSet::Overdrive_Sword02));
		AddEventFunction(TEXT("ATK_SFX_ODS3"), std::bind(&DUOLClient::Player::PlaySoundClipAndVoice, this, PlayerSoundTable::OverdriveSwordFinal, PlayerVoiceSoundSet::Overdrive_SwordFin));

		AddEventFunction(TEXT("ATK_SFX_ODF1"), std::bind(&DUOLClient::Player::PlaySoundClipAndVoice, this, PlayerSoundTable::Overdrive_Fist_One, PlayerVoiceSoundSet::Overdrive_Fist01));
		AddEventFunction(TEXT("ATK_SFX_ODF2"), std::bind(&DUOLClient::Player::PlaySoundClipAndVoice, this, PlayerSoundTable::Overdrive_Fist_Two, PlayerVoiceSoundSet::Overdrive_Fist02));
		AddEventFunction(TEXT("ATK_SFX_ODF3"), std::bind(&DUOLClient::Player::PlaySoundClipAndVoice, this, PlayerSoundTable::MagnumPunch_Over, PlayerVoiceSoundSet::NormalSword_SwordCombo_23_4));

		AddEventFunction(TEXT("ATK_SFX_ODSUlt1"), std::bind(&DUOLClient::Player::PlaySoundClipAndVoice, this, PlayerSoundTable::OverdriveSword01, PlayerVoiceSoundSet::NONE));
		AddEventFunction(TEXT("ATK_SFX_ODSUlt2"), std::bind(&DUOLClient::Player::PlaySoundClipAndVoice, this, PlayerSoundTable::OverdriveSword02, PlayerVoiceSoundSet::NONE));
		//AddEventFunction(TEXT("ATK_SFX_ODSUlt3"), std::bind(&DUOLClient::Player::PlaySoundClipAndVoice, this, PlayerSoundTable::OverdriveSwordFinal, PlayerVoiceSoundSet::Overdrive_SwordUlt));

		AddEventFunction(TEXT("ATK_SFX_SCS1"), std::bind(&DUOLClient::Player::PlaySoundClipAndVoice, this, PlayerSoundTable::SwordChargingSound01, PlayerVoiceSoundSet::NONE));

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

			_attackSoundModules.push_back(comp);
		}

		auto object = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->CreateEmpty();
		object->GetTransform()->SetParent(holder->GetTransform());
		object->SetName(TEXT("AuraSoundModule"));
		auto comp = object->AddComponent<DUOLGameEngine::AudioSource>();
		_auraSource = comp;


		object = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->CreateEmpty();
		object->GetTransform()->SetParent(holder->GetTransform());
		object->SetName(TEXT("VoiceSoundModule"));
		comp = object->AddComponent<DUOLGameEngine::AudioSource>();
		_voiceSource = comp;


		_auraSource->SetAudioClip(_attackAudioClips[static_cast<int>(PlayerSoundTable::AuraSound)]);

		object = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->CreateEmpty();
		object->GetTransform()->SetParent(holder->GetTransform());
		object->SetName(TEXT("ScriptSoundModule"));
		comp = object->AddComponent<DUOLGameEngine::AudioSource>();
		_scriptSource = comp;


		_auraSource->SetAudioClip(_voiceAudioClips[static_cast<int>(PlayerSoundTable::AuraSound)]);

#pragma endregion
	}
	
	void Player::Jump(float holdTime, float _flyTime)
	{
		_currentPlayerWeapon->HouseSword();
		_jumpHoldTime = holdTime;
		_jumpFlyTime = _flyTime;

		_playerStateMachine.TransitionTo(TEXT("PlayerState_Jump"), 0.f);
	}

	void Player::JumpEnd()
	{
		_playerStateMachine.TransitionTo(TEXT("PlayerState_Idle"), 0.f);
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

		//lock On Check
		if(_isLockOnMode)
		{
			if (_lockOnTargetCharacterBase != nullptr && _lockOnTargetCharacterBase->GetIsDie())
			{
				_mainCamController->SetViewTransform(nullptr);

				// Lock Off
				_playerAnimator->SetBool(TEXT("IsLockOn"), false);
				_lockOnTargetTransform = nullptr;
				_lockOnTargetCharacterBase = nullptr;
				_isLockOnMode = false;
			}
		}

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

	void Player::SetCanInteraction(bool value)
	{
		_canInteraction = value;
	}

	void Player::PlaySoundClip(PlayerSoundTable soundClip, bool isLoop)
	{
		int soundIdx = static_cast<int>(soundClip);
		if (0 > soundIdx || soundIdx > (static_cast<int>(PlayerSoundTable::NONE) - 1))
			return;

		_attackSource->SetAudioClip(_attackAudioClips[soundIdx]);
		_attackSource->SetIsLoop(isLoop);
		_attackSource->Play();
	}

	void Player::PlaySoundClipAndVoice(PlayerSoundTable soundClip, PlayerVoiceSoundSet voiceSet)
	{
		int soundIdx = static_cast<int>(soundClip);
		if (0 > soundIdx || soundIdx > (static_cast<int>(PlayerSoundTable::NONE) - 1))
		{
		}
		else
		{
			_attackSource->SetAudioClip(_attackAudioClips[soundIdx]);
			_attackSource->SetIsLoop(false);
			_attackSource->Play();
		}

		int voiceIdx = static_cast<int>(voiceSet);
		if (0 > voiceIdx || voiceIdx > (static_cast<int>(PlayerVoiceSoundSet::NONE) - 1))
		{
		}
		else
		{
			//간단하게 그냥 rand로 하자.
			int voicePreset = rand() % 2;

			if (voicePreset)
				_voiceSource->SetAudioClip(_voiceAudioSets[voiceSet].first);
			else
				_voiceSource->SetAudioClip(_voiceAudioSets[voiceSet].second);

			_voiceSource->SetIsLoop(false);
			_voiceSource->Play();
		}
	}

	void Player::PlaySoundClipInModule(DUOLGameEngine::AudioClip* soundClip, int idx, bool isLoop)
	{
		_attackSoundModules[idx]->SetAudioClip(soundClip);
		_attackSoundModules[idx]->SetIsLoop(isLoop);
		_attackSoundModules[idx]->Play();
	}

	void Player::PlaySoundClipInModule(PlayerSoundTable soundClip, int idx, bool isLoop)
	{
		int soundIdx = static_cast<int>(soundClip);
		if (0 > soundIdx || soundIdx > (static_cast<int>(PlayerSoundTable::NONE) - 1))
			return;

		_attackSoundModules[idx]->SetAudioClip(_attackAudioClips[soundIdx]);
		_attackSoundModules[idx]->SetIsLoop(isLoop);
		_attackSoundModules[idx]->Play();
	}

	void Player::PlayVoiceSoundClip(PlayerVoiceSoundTable soundClip, bool isLoop)
	{
		int soundIdx = static_cast<int>(soundClip);
		if (0 > soundIdx || soundIdx > (static_cast<int>(PlayerVoiceSoundTable::NONE) - 1))
			return;

		_voiceSource->SetAudioClip(_voiceAudioClips[soundIdx]);
		_voiceSource->SetIsLoop(isLoop);
		_voiceSource->Play();
	}

	void Player::PlayScriptSoundClip(DUOLGameEngine::AudioClip* soundClip, bool isLoop)
	{
		_scriptSource->SetAudioClip(soundClip);
		_scriptSource->SetIsLoop(isLoop);
		_scriptSource->Play();
	}

	void Player::PlayScriptPause()
	{
		_scriptSource->Pause();
	}

	void Player::PlayScriptPlay()
	{
		_scriptSource->UnPause();
	}

	void Player::AddOverdrivePoint(float point)
	{
		_currentOverdrivePoint += point;
		_currentOverdrivePoint = std::clamp(_currentOverdrivePoint, 0.f, 100.f);

		DUOLClient::UIDataManager::GetInstance()->SetPlayerOverDriveUI(_currentOverdrivePoint);
	}

	bool Player::IsOverdriveMode()
	{
		if (_isOverdriveSwordMode || _isOverdriveFistMode)
			return true;
		else
			return false;
	}

	DUOLGameEngine::AudioSource* Player::GetAuraSoundSource()
	{
		if (_auraSource != nullptr)
			return _auraSource;

		return nullptr;
	}
}
