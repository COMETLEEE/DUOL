#include "DUOLClient/Player/FSM/PlayerState_Attack.h"

#include "DUOLClient/Camera/MainCameraController.h"
#include "DUOLClient/ECS/Component/Enemy/AI_EnemyBasic.h"
#include "DUOLClient/Manager/GameManager.h"
#include "DUOLClient/Manager/ParticleManager.h"
#include "DUOLClient/Manager/SystemManager.h"
#include "DUOLClient/Manager/UIDataManager.h"
#include "DUOLClient/Player/Weapon_AreaWave.h"
#include "DUOLGameEngine/Manager/PhysicsManager.h"

#include "DUOLCommon/Log/LogHelper.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Transform.h"
#include "DUOLGameEngine/ECS/Component/BoxCollider.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"
#include "DUOLGameEngine/ECS/Component/ParticleRenderer.h"

#include "DUOLClient/Player/Weapon_Sword.h"
#include "DUOLClient/Player/Weapon_Wave.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"
#include "DUOLGameEngine/Util/Coroutine/WaitForSeconds.h"

namespace DUOLClient
{
	PlayerState_Attack::PlayerState_Attack(DUOLClient::Player* player) :
		PlayerStateBase(TEXT("PlayerState_Attack"), player)
		, _currentComboTreeNode(nullptr)
	{
		// 캔슬 프레임 구간 이벤트 함수 등록
#pragma region CANCLE_FRAME_EVENT
		_player->AddEventFunction(TEXT("StartCancleFrame"), std::bind(&DUOLClient::PlayerState_Attack::StartCancleFrame, this));

		_player->AddEventFunction(TEXT("StartSlowFrame"), std::bind(&DUOLClient::PlayerState_Attack::StartSlowFrame, this));

		_player->AddEventFunction(TEXT("EndCancleFrame"), std::bind(&DUOLClient::PlayerState_Attack::EndCancleFrame, this));

		_player->AddEventFunction(TEXT("EndAttack"), std::bind(&DUOLClient::PlayerState_Attack::EndAttack, this));

		_player->AddEventFunction(TEXT("Overdrive_Attack_Speed_Sword"), std::bind(&DUOLClient::PlayerState_Attack::OverdriveSwordDefaultSpeed, this));

		_player->AddEventFunction(TEXT("Overdrive_Attack_Speed_Fist"), std::bind(&DUOLClient::PlayerState_Attack::OverdriveFistDefaultSpeed, this));

#pragma endregion

		// 타격 프레임 이벤트 함수 등록
#pragma region HIT_FRAME_EVENT
		_player->AddEventFunction(TEXT("StartSwordAttackFrame"), std::bind(&DUOLClient::PlayerState_Attack::StartSwordAttackFrame, this));

		_player->AddEventFunction(TEXT("EndSwordAttackFrame"), std::bind(&DUOLClient::PlayerState_Attack::EndSwordAttackFrame, this));

		_player->AddEventFunction(TEXT("FistHit"), std::bind(&DUOLClient::PlayerState_Attack::FistHit, this));

		_player->AddEventFunction(TEXT("WaveHit"), std::bind(&DUOLClient::PlayerState_Attack::WaveHit, this));

		_player->AddEventFunction(TEXT("AreaWaveHit"), std::bind(&DUOLClient::PlayerState_Attack::AreaWaveHit, this));
#pragma endregion

#pragma region EFFECT
		_player->AddEventFunction(TEXT("StartSwordTrailFrame"), std::bind(&DUOLClient::PlayerState_Attack::StartSwordTrailFrame, this));

		_player->AddEventFunction(TEXT("EndSwordTrailFrame"), std::bind(&DUOLClient::PlayerState_Attack::EndSwordTrailFrame, this));
#pragma endregion


	}

	PlayerState_Attack::~PlayerState_Attack()
	{
	}


	void PlayerState_Attack::StartCancleFrame()
	{
		if (!_isOnStay)
			return;

		// DUOL_INFO(DUOL_CONSOLE, "Start cancel frame.");

		_isInCancle = true;
		_isRotateNow = false;

		_nextComboTreeNode = nullptr;
	}

	void PlayerState_Attack::StartSlowFrame()
	{
		if (!_isOnStay)
			return;

		if (_nextComboTreeNode != nullptr)
		{
			// 그대로. 이미 입력이 있어서 다음 공격이 선택되었다.
		}
		else
		{
			// 재생 속도 감소.
			_animator->SetFloat(TEXT("AnimationSpeed"), 0.8f);
		}
	}

	void PlayerState_Attack::EndCancleFrame()
	{
		if (!_isOnStay)
			return;

		// DUOL_INFO(DUOL_CONSOLE, "End cancel frame.");

		_isInCancle = false;
		_isRotateNow = false;

		CheckCanEnterNextAttack();
	}

	void PlayerState_Attack::StartSwordAttackFrame()
	{
		// 무기 충돌 On
		_player->_currentplayerWeaponSwordCollider->SetIsEnabled(true);
		_player->_currentPlayerWeapon->ResetAttackList();
	}

	void PlayerState_Attack::EndSwordAttackFrame()
	{
		// 무기 충돌 Off
		_player->_currentplayerWeaponSwordCollider->SetIsEnabled(false);
	}

	void PlayerState_Attack::StartSwordTrailFrame()
	{
		// 최대 0.5초 (30 프레임 넘어가는게 없음)
		_normalSwordTrail = DUOLClient::ParticleManager::GetInstance()->Pop(ParticleEnum::SwordTrail, 0.5f);

		if (_normalSwordTrail == nullptr)
			return;

		_normalSwordTrail->GetTransform()->SetParent(_player->_currentPlayerWeapon->GetTransform());

		auto transform = _normalSwordTrail->GetTransform();

		transform->SetLocalPosition(DUOLMath::Vector3(-0.029f, -0.612f, -0.01f));

		transform->SetLocalEulerAngle(DUOLMath::Vector3(0.f, 0.f, 90.f));
	}

	void PlayerState_Attack::EndSwordTrailFrame()
	{
		if (_normalSwordTrail != nullptr)
		{
			_normalSwordTrail->GetTransform()->SetParent(nullptr);

			_normalSwordTrail->GetGameObject()->SetIsActiveSelf(false);

			_normalSwordTrail = nullptr;
		}
	}

	void PlayerState_Attack::FistHit()
	{
		if (!_isOnStay)
			return;

		std::vector<DUOLPhysics::RaycastHit> spherecastHits;

		auto&& comboNodeData = _currentComboTreeNode->GetData();

		const DUOLMath::Vector3& hitCenterOffset = comboNodeData._hitCenterOffset;

		DUOLMath::Vector3 startPos = _transform->GetWorldPosition() + _transform->GetRight() * hitCenterOffset.x + _transform->GetUp() * hitCenterOffset.y + _transform->GetLook() * hitCenterOffset.z;

		int mobHitSoundCount = 0;

		if (DUOLGameEngine::PhysicsManager::GetInstance()->SpherecastAll(startPos, _transform->GetLook(), comboNodeData._hitRadius, comboNodeData._hitMaxDistance, spherecastHits))
		{
			for (auto hited : spherecastHits)
			{
				DUOLGameEngine::GameObject* gameObject = reinterpret_cast<DUOLGameEngine::GameObject*>(hited._userData);

				// 적군입니다. 맞았습니다.
				if (gameObject->GetTag() == TEXT("Enemy"))
				{
					// 구의 출발 지역에서 맞은 경우. 없앤다.
					if (hited._hitPosition == DUOLMath::Vector3::Zero)
						continue;

					auto aiEnemy = gameObject->GetComponent<DUOLClient::CharacterBase>();

					if (_player->Attack(aiEnemy, _player->_currentDamage + 10.f, AttackType::LightAttack))
					{
						if (mobHitSoundCount < MAX_SOUND_PLAYER)
						{
							_player->PlaySoundClipInModule(_currentAudioClip, mobHitSoundCount, false);
							//_swordAudioSource->Play();
							mobHitSoundCount++;
						}

						auto particleData = ParticleManager::GetInstance()->Pop(ParticleEnum::MonsterHit, 1.0f);

						particleData->GetTransform()->SetPosition(hited._hitPosition, DUOLGameEngine::Space::World);

						// 오버 드라이브 상태 아니면 오버드라이브 포인트 업 !
						if (!InOverdriveSwordCheck() && !InOverdriveFistCheck())
						{
							_player->AddOverdrivePoint(_player->_overdrivePointPerFist);
						}
					}
				}
			}
		}
	}

	void PlayerState_Attack::WaveHit()
	{
		// 충격파 계열
		if (!_isOnStay)
			return;

		// 웨이브 공격 날립니다.
		std::function<DUOLGameEngine::CoroutineHandler(void)> routine = std::bind(&DUOLClient::PlayerState_Attack::LaunchWave, this);

		_player->StartCoroutine(routine);

		// 이펙트 적용
		auto& nodeName = _currentComboTreeNode->_nodeName;

		if (nodeName == TEXT("Dust"))
		{
			auto particleRenderer = DUOLClient::ParticleManager::GetInstance()->Pop(ParticleEnum::Dust, 5.f);

			particleRenderer->GetTransform()->SetPosition(_transform->GetWorldPosition(), DUOLGameEngine::Space::World);

			particleRenderer->GetTransform()->SetRotation(_transform->GetWorldRotation(), DUOLGameEngine::Space::World);
		}
		else if (nodeName == TEXT("Crack"))
		{
			auto particleRenderer = DUOLClient::ParticleManager::GetInstance()->Pop(ParticleEnum::Crack, 5.f);

			particleRenderer->GetTransform()->SetPosition(_transform->GetWorldPosition(), DUOLGameEngine::Space::World);

			particleRenderer->GetTransform()->SetRotation(_transform->GetWorldRotation(), DUOLGameEngine::Space::World);
		}
		else if (nodeName == TEXT("FistWide"))
		{
			auto particleRenderer = DUOLClient::ParticleManager::GetInstance()->Pop(ParticleEnum::FistWide, 2.f);

			// 이펙트 위치
			particleRenderer->GetTransform()->SetPosition(_transform->GetWorldPosition() + _transform->GetLook() * 1.5f + DUOLMath::Vector3::Up * 1.f, DUOLGameEngine::Space::World);

			// 플레이어 앞을 바라보도록 회전
			DUOLMath::Quaternion rot = DUOLMath::Quaternion::CreateFromAxisAngle(_transform->GetRight(), DUOLMath::MathHelper::DegreeToRadian(-90.f));

			particleRenderer->GetTransform()->SetRotation(rot);
		}
		else if (nodeName == TEXT("ODLastSword"))
		{
			auto particleRenderer = DUOLClient::ParticleManager::GetInstance()->Pop(ParticleEnum::OverdriveLastSword, 5.f);

			particleRenderer->GetTransform()->SetPosition(_transform->GetWorldPosition(), DUOLGameEngine::Space::World);

			particleRenderer->GetTransform()->SetRotation(_transform->GetWorldRotation(), DUOLGameEngine::Space::World);
		}
		else if (nodeName == TEXT("ODLastFist"))
		{
			auto particleRenderer = DUOLClient::ParticleManager::GetInstance()->Pop(ParticleEnum::OverdriveLastPunch, 5.f);

			particleRenderer->GetTransform()->SetPosition(_transform->GetWorldPosition(), DUOLGameEngine::Space::World);

			particleRenderer->GetTransform()->SetRotation(_transform->GetWorldRotation(), DUOLGameEngine::Space::World);
		}

		// (Wave Hit 계열 == 막타) 만 카메라 쉐이크가 있다.
		if (_currentComboTreeNode->_data._useCamShake)
			_mainCamController->SetCameraShake(0.25f, _currentComboTreeNode->_data._shakePower * 2.f);

		// Cannot Dash
		std::function<DUOLGameEngine::CoroutineHandler()> cantDash = std::bind(&DUOLClient::PlayerState_Attack::SetCannotDash, this);

		_player->StartCoroutine(cantDash);
	}

	void PlayerState_Attack::AreaWaveHit()
	{
		// 충격파 계열
		if (!_isOnStay)
			return;

		std::function<DUOLGameEngine::CoroutineHandler(void)> routine = std::bind(&DUOLClient::PlayerState_Attack::LaunchAreaWave, this);

		// 후딜 적용
		_player->StartCoroutine(routine);

		// 이펙트 적용
		auto& nodeName = _currentComboTreeNode->_nodeName;

		if (nodeName == TEXT("Dust"))
		{
			auto particleRenderer = DUOLClient::ParticleManager::GetInstance()->Pop(ParticleEnum::Dust, 5.f);

			particleRenderer->GetTransform()->SetPosition(_transform->GetWorldPosition(), DUOLGameEngine::Space::World);

			particleRenderer->GetTransform()->SetRotation(_transform->GetWorldRotation(), DUOLGameEngine::Space::World);
		}
		else if (nodeName == TEXT("Crack"))
		{
			auto particleRenderer = DUOLClient::ParticleManager::GetInstance()->Pop(ParticleEnum::Crack, 5.f);

			particleRenderer->GetTransform()->SetPosition(_transform->GetWorldPosition(), DUOLGameEngine::Space::World);

			particleRenderer->GetTransform()->SetRotation(_transform->GetWorldRotation(), DUOLGameEngine::Space::World);
		}
		else if (nodeName == TEXT("FistWide"))
		{
			auto particleRenderer = DUOLClient::ParticleManager::GetInstance()->Pop(ParticleEnum::FistWide, 5.f);

			particleRenderer->GetTransform()->SetPosition(_transform->GetWorldPosition() + _transform->GetLook() * 1.f, DUOLGameEngine::Space::World);
		}
		else if (nodeName == TEXT("ODLastSword"))
		{
			auto particleRenderer = DUOLClient::ParticleManager::GetInstance()->Pop(ParticleEnum::OverdriveLastSword, 5.f);

			particleRenderer->GetTransform()->SetPosition(_transform->GetWorldPosition(), DUOLGameEngine::Space::World);

			particleRenderer->GetTransform()->SetRotation(_transform->GetWorldRotation(), DUOLGameEngine::Space::World);
		}
		else if (nodeName == TEXT("ODLastFist"))
		{
			auto particleRenderer = DUOLClient::ParticleManager::GetInstance()->Pop(ParticleEnum::OverdriveLastPunch, 5.f);

			particleRenderer->GetTransform()->SetPosition(_transform->GetWorldPosition(), DUOLGameEngine::Space::World);

			particleRenderer->GetTransform()->SetRotation(_transform->GetWorldRotation(), DUOLGameEngine::Space::World);
		}
		_mainCamController->SetCameraShake(0.5f, DUOLMath::Vector2(6.f, 6.f));

		// Cannot Dash
		std::function<DUOLGameEngine::CoroutineHandler()> cantDash = std::bind(&DUOLClient::PlayerState_Attack::SetCannotDash, this);

		_player->StartCoroutine(cantDash);
	}

	void PlayerState_Attack::OverdriveSwordDefaultSpeed()
	{
		_animator->SetFloat(TEXT("AnimationSpeed"), OVERDRIVE_ATTACK_SPEED_SWORD);
	}

	void PlayerState_Attack::OverdriveFistDefaultSpeed()
	{
		_animator->SetFloat(TEXT("AnimationSpeed"), OVERDRIVE_ATTACK_SPEED_FIST);
	}

	void PlayerState_Attack::ChangeDirection(float fixedDeltaTime)
	{
		_rotateSlerpValue += fixedDeltaTime * _rotationSpeedSmoothness;

		if (_rotateSlerpValue > _rotationAngle)
		{
			//목적지 도착. 이제 보간을 할 필요는 없습니다.
			_rotateSlerpValue = _rotationAngle;
			_isRotateNow = false;
		}
		if (_rotationAngle > 0)
		{
			auto currentLook = (_fromRotateDirection * sin((_rotationAngle - _rotateSlerpValue)) + _toRotateDirection * sin(_rotateSlerpValue)) / sin(_rotationAngle);
			if (currentLook.Length() > 0)
				_transform->LookAt(_transform->GetWorldPosition() + DUOLMath::Vector3(currentLook.x, 0.f, currentLook.y) * 10.f, DUOLMath::Vector3::Up);
		}
	}

	DUOLGameEngine::CoroutineHandler PlayerState_Attack::SetPostDelay(float delayTime)
	{
		_player->_canStartAttack = false;

		co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(delayTime);

		_player->_canStartAttack = true;
	}

	DUOLGameEngine::CoroutineHandler PlayerState_Attack::SetCannotDash()
	{
		_player->_canStartDash = false;

		co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(1.f);

		_player->_canStartDash = true;
	}

	DUOLGameEngine::CoroutineHandler PlayerState_Attack::LaunchWave()
	{
		auto&& data = _currentComboTreeNode->GetData();

		DUOLGameEngine::Transform* playerTransform = _player->GetTransform();

		_player->_playerWeaponWave->StartWave(playerTransform->GetWorldPosition() + data._hitCenterOffset, data._startWaveBoxHalfExtents,
			(data._endWaveBoxHalfExtents - data._startWaveBoxHalfExtents) / data._waveTime,
			DUOLMath::Vector3::TransformNormal(data._waveVelocity.Normalized(), playerTransform->GetWorldMatrix()) * data._waveVelocity.Length(), playerTransform->GetWorldRotation(), data._waveTime, _currentAudioClip);

		co_yield nullptr;

		//co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(data._waveTime);

		//_player->_playerWeaponWave->EndWave();
	}

	DUOLGameEngine::CoroutineHandler PlayerState_Attack::LaunchAreaWave()
	{
		auto&& data = _currentComboTreeNode->GetData();

		DUOLGameEngine::Transform* playerTransform = _player->GetTransform();

		_player->_playerWeaponAreaWave->StartAreaWave(playerTransform->GetWorldPosition(), playerTransform->GetWorldRotation(), data._waveTime, _currentAudioClip);

		co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(data._waveTime);

		_player->_playerWeaponAreaWave->EndAreaWave();
	}

	void PlayerState_Attack::CheckCanEnterNextAttack()
	{
		if (!_isOnStay)
			return;

		// 캔슬 프레임 구간에서 정확한 공격 커맨드가 내려오지 않았다면 전환 ..!
		if (_nextComboTreeNode == nullptr)
		{
			EndAttack();
		}
		else
		{
			_currentComboTreeNode = _nextComboTreeNode;

			SettingCurrentComboNodeState();
		}
	}

	void PlayerState_Attack::BuildComboTree(const std::unordered_map<DUOLCommon::tstring, float>& playerDataTable)
	{


		// TODO : 구조화된 데이터를 받아서 콤보 트리를 빌드할 수 있도록 짜놓는다.
#pragma region SWORD_COMBO_TREE
		// 1 타
		std::vector<std::tuple<DUOLCommon::tstring, DUOLGameEngine::AnimatorControllerParameterType, std::any>> animatorParameterTable;

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 1 });
		animatorParameterTable.push_back({ TEXT("AnimationSpeed"), DUOLGameEngine::AnimatorControllerParameterType::Float, 1.2f });

		DUOLCommon::tstring actionName = TEXT("NormalSword_S");

		if(playerDataTable.contains(actionName))
			animatorParameterTable.push_back({ TEXT("Damage"), DUOLGameEngine::AnimatorControllerParameterType::Damage, playerDataTable.find(actionName)->second});

		_swordComboTree = BinaryTree<Player_AttackData>({ Player_AttackType::SWORD, animatorParameterTable });

		// 2타
		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 2 });
		animatorParameterTable.push_back({ TEXT("AnimationSpeed"), DUOLGameEngine::AnimatorControllerParameterType::Float, 1.2f });

		actionName = TEXT("NormalSword_SS");

		if (playerDataTable.contains(actionName))
			animatorParameterTable.push_back({ TEXT("Damage"), DUOLGameEngine::AnimatorControllerParameterType::Damage, playerDataTable.find(actionName)->second });

		auto swordSecond = _swordComboTree.AddLeftNode({ Player_AttackType::SWORD, animatorParameterTable });

		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 2 });
		animatorParameterTable.push_back({ TEXT("AnimationSpeed"), DUOLGameEngine::AnimatorControllerParameterType::Float, 0.8f });
		animatorParameterTable.push_back({ TEXT("SFX"), DUOLGameEngine::AnimatorControllerParameterType::SFX, PlayerSoundTable::Hit_Sound });
		actionName = TEXT("NormalSword_SF");

		if (playerDataTable.contains(actionName))
			animatorParameterTable.push_back({ TEXT("Damage"), DUOLGameEngine::AnimatorControllerParameterType::Damage, playerDataTable.find(actionName)->second });


		auto swordCombo1_2 = _swordComboTree.AddRightNode({ Player_AttackType::FIST, animatorParameterTable
			,DUOLMath::Vector3(0.f, 1.5f, -2.f), 2.f, 4.f });

		// 3타
		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 3 });
		animatorParameterTable.push_back({ TEXT("AnimationSpeed"), DUOLGameEngine::AnimatorControllerParameterType::Float, 1.3f });

		actionName = TEXT("NormalSword_SSS");
		if (playerDataTable.contains(actionName))
			animatorParameterTable.push_back({ TEXT("Damage"), DUOLGameEngine::AnimatorControllerParameterType::Damage, playerDataTable.find(actionName)->second });


		auto swordThird = swordSecond->AddLeftNode({ Player_AttackType::SWORD, animatorParameterTable });

		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 3 });
		animatorParameterTable.push_back({ TEXT("AnimationSpeed"), DUOLGameEngine::AnimatorControllerParameterType::Float, 0.8f });
		animatorParameterTable.push_back({ TEXT("SFX"), DUOLGameEngine::AnimatorControllerParameterType::SFX, PlayerSoundTable::Hit_Sound });

		actionName = TEXT("NormalSword_SSF");
		if (playerDataTable.contains(actionName))
			animatorParameterTable.push_back({ TEXT("Damage"), DUOLGameEngine::AnimatorControllerParameterType::Damage, playerDataTable.find(actionName)->second });

		auto swordCombo2_3 = swordSecond->AddRightNode({ Player_AttackType::FIST, animatorParameterTable
			, DUOLMath::Vector3(0.f, 1.5f, -2.f), 2.f, 4.f });

		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 3 });
		animatorParameterTable.push_back({ TEXT("AnimationSpeed"), DUOLGameEngine::AnimatorControllerParameterType::Float, 0.7f });
		animatorParameterTable.push_back({ TEXT("SFX"), DUOLGameEngine::AnimatorControllerParameterType::SFX, PlayerSoundTable::Hit_Sound });

		actionName = TEXT("NormalSword_SFF");
		if (playerDataTable.contains(actionName))
			animatorParameterTable.push_back({ TEXT("Damage"), DUOLGameEngine::AnimatorControllerParameterType::Damage, playerDataTable.find(actionName)->second });

		auto swordCombo1_3 = swordCombo1_2->AddRightNode({ Player_AttackType::FIST_WAVE, animatorParameterTable
			, DUOLMath::Vector3(0.f, 0.5f, 0.2f), 0.f, 6.f,
			DUOLMath::Vector3::Forward * 15.f, 0.3f, DUOLMath::Vector3(2.f, 1.5f, 0.6f), DUOLMath::Vector3(2.75f, 2.f, 0.6f), true, DUOLMath::Vector2(1.5f, 1.5f) });

		swordCombo1_3->_nodeName = TEXT("FistWide");

		// 4타
		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 4 });
		animatorParameterTable.push_back({ TEXT("AnimationSpeed"), DUOLGameEngine::AnimatorControllerParameterType::Float, 1.2f });
		animatorParameterTable.push_back({ TEXT("SFX"), DUOLGameEngine::AnimatorControllerParameterType::SFX, PlayerSoundTable::Hit_Sound });

		actionName = TEXT("NormalSword_SSSS");
		if (playerDataTable.contains(actionName))
			animatorParameterTable.push_back({ TEXT("Damage"), DUOLGameEngine::AnimatorControllerParameterType::Damage, playerDataTable.find(actionName)->second });

		auto swordFourth = swordThird->AddLeftNode({ Player_AttackType::SWORD_WAVE, animatorParameterTable
			, DUOLMath::Vector3(0.f, 0.5f, 0.2f), 0.f, 6.f,
			DUOLMath::Vector3::Forward * 50.f, 0.16f, DUOLMath::Vector3(4.f, 2.f, 0.5f), DUOLMath::Vector3(5.f, 2.5f, 0.6f), true, DUOLMath::Vector2(4.f, 4.f) });

		swordFourth->_nodeName = TEXT("Dust");

		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 4 });
		animatorParameterTable.push_back({ TEXT("AnimationSpeed"), DUOLGameEngine::AnimatorControllerParameterType::Float, 1.0f });
		animatorParameterTable.push_back({ TEXT("SFX"), DUOLGameEngine::AnimatorControllerParameterType::SFX, PlayerSoundTable::Hit_Sound });

		actionName = TEXT("NormalSword_SSFF");
		if (playerDataTable.contains(actionName))
			animatorParameterTable.push_back({ TEXT("Damage"), DUOLGameEngine::AnimatorControllerParameterType::Damage, playerDataTable.find(actionName)->second });

		auto swordCombo2_4 = swordCombo2_3->AddRightNode({ Player_AttackType::FIST_WAVE, animatorParameterTable
			, DUOLMath::Vector3(0.f, 0.5f, 0.2f), 0.f, 6.f,
			DUOLMath::Vector3::Forward * 50.f, 0.15f, DUOLMath::Vector3(3.5f, 2.f, 0.5f), DUOLMath::Vector3(4.5f, 2.f, 0.6f), true, DUOLMath::Vector2(3.75f, 3.75f) });

		swordCombo2_4->_nodeName = TEXT("Crack");

		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 4 });
		animatorParameterTable.push_back({ TEXT("AnimationSpeed"), DUOLGameEngine::AnimatorControllerParameterType::Float, 1.f });
		animatorParameterTable.push_back({ TEXT("SFX"), DUOLGameEngine::AnimatorControllerParameterType::SFX, PlayerSoundTable::Hit_Sound });

		actionName = TEXT("NormalSword_SSSF");
		if (playerDataTable.contains(actionName))
			animatorParameterTable.push_back({ TEXT("Damage"), DUOLGameEngine::AnimatorControllerParameterType::Damage, playerDataTable.find(actionName)->second });

		auto swordCombo3_4 = swordThird->AddRightNode({ Player_AttackType::FIST_WAVE, animatorParameterTable
			, DUOLMath::Vector3(0.f, 0.5f, 0.2f), 0.f, 6.f,
			DUOLMath::Vector3::Forward * 50, 0.15f, DUOLMath::Vector3(4.f, 2.f, 0.5f), DUOLMath::Vector3(5.f, 2.f, 0.6f), true, DUOLMath::Vector2(4.5f, 4.5f) });

		swordCombo3_4->_nodeName = TEXT("Crack");
#pragma endregion

#pragma region FIST_COMBO_TREE
		// 1타
		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 1 });
		animatorParameterTable.push_back({ TEXT("AnimationSpeed"), DUOLGameEngine::AnimatorControllerParameterType::Float, 0.8f });
		animatorParameterTable.push_back({ TEXT("SFX"), DUOLGameEngine::AnimatorControllerParameterType::SFX, PlayerSoundTable::Hit_Sound });

		actionName = TEXT("NormalFist_F");
		if (playerDataTable.contains(actionName))
			animatorParameterTable.push_back({ TEXT("Damage"), DUOLGameEngine::AnimatorControllerParameterType::Damage, playerDataTable.find(actionName)->second });

		_fistComboTree = BinaryTree<Player_AttackData>({ Player_AttackType::FIST, animatorParameterTable
			,DUOLMath::Vector3(0.f, 1.5f, -2.f), 2.f, 4.f });

		// 2타
		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 2 });
		animatorParameterTable.push_back({ TEXT("AnimationSpeed"), DUOLGameEngine::AnimatorControllerParameterType::Float, 0.8f });
		animatorParameterTable.push_back({ TEXT("SFX"), DUOLGameEngine::AnimatorControllerParameterType::SFX, PlayerSoundTable::Hit_Sound });

		actionName = TEXT("NormalFist_FF");
		if (playerDataTable.contains(actionName))
			animatorParameterTable.push_back({ TEXT("Damage"), DUOLGameEngine::AnimatorControllerParameterType::Damage, playerDataTable.find(actionName)->second });

		auto fistSecond = _fistComboTree.AddRightNode({ Player_AttackType::FIST, animatorParameterTable
			,DUOLMath::Vector3(0.f, 1.5f, -2.f), 2.f, 4.f });

		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 2 });
		animatorParameterTable.push_back({ TEXT("AnimationSpeed"), DUOLGameEngine::AnimatorControllerParameterType::Float, 1.2f });

		actionName = TEXT("NormalFist_FS");
		if (playerDataTable.contains(actionName))
			animatorParameterTable.push_back({ TEXT("Damage"), DUOLGameEngine::AnimatorControllerParameterType::Damage, playerDataTable.find(actionName)->second });


		auto fistCombo1_2 = _fistComboTree.AddLeftNode({ Player_AttackType::SWORD, animatorParameterTable });

		// 3타
		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 3 });
		animatorParameterTable.push_back({ TEXT("AnimationSpeed"), DUOLGameEngine::AnimatorControllerParameterType::Float, 0.8f });
		animatorParameterTable.push_back({ TEXT("SFX"), DUOLGameEngine::AnimatorControllerParameterType::SFX, PlayerSoundTable::Hit_Sound });

		actionName = TEXT("NormalFist_FFF");
		if (playerDataTable.contains(actionName))
			animatorParameterTable.push_back({ TEXT("Damage"), DUOLGameEngine::AnimatorControllerParameterType::Damage, playerDataTable.find(actionName)->second });

		auto fistThird = fistSecond->AddRightNode({ Player_AttackType::FIST_WAVE, animatorParameterTable
			, DUOLMath::Vector3(0.f, 0.5f, 0.2f), 0.f, 6.f,
			DUOLMath::Vector3::Forward * 16.f, 0.3f, DUOLMath::Vector3(2.f, 1.5f, 0.6f), DUOLMath::Vector3(2.5f, 2.f, 0.6f), true, DUOLMath::Vector2(1.5f, 1.5f) });

		fistThird->_nodeName = TEXT("FistWide");

		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 3 });
		animatorParameterTable.push_back({ TEXT("AnimationSpeed"), DUOLGameEngine::AnimatorControllerParameterType::Float, 0.8f });
		animatorParameterTable.push_back({ TEXT("SFX"), DUOLGameEngine::AnimatorControllerParameterType::SFX, PlayerSoundTable::Hit_Sound });

		actionName = TEXT("NormalFist_FSS");
		if (playerDataTable.contains(actionName))
			animatorParameterTable.push_back({ TEXT("Damage"), DUOLGameEngine::AnimatorControllerParameterType::Damage, playerDataTable.find(actionName)->second });

		auto fistCombo1_3 = fistCombo1_2->AddLeftNode({ Player_AttackType::SWORD_WAVE, animatorParameterTable
			, DUOLMath::Vector3(0.f, 0.5f, 0.2f), 0.f, 6.f,
			DUOLMath::Vector3::Forward * 50.f, 0.18f, DUOLMath::Vector3(3.5f, 2.f, 0.35f), DUOLMath::Vector3(4.25f, 2.f, 0.45f), true, DUOLMath::Vector2(2.f, 2.f) });

		fistCombo1_3->_nodeName = TEXT("Dust");

		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 3 });
		animatorParameterTable.push_back({ TEXT("AnimationSpeed"), DUOLGameEngine::AnimatorControllerParameterType::Float, 1.f });

		actionName = TEXT("NormalFist_FFS");
		if (playerDataTable.contains(actionName))
			animatorParameterTable.push_back({ TEXT("Damage"), DUOLGameEngine::AnimatorControllerParameterType::Damage, playerDataTable.find(actionName)->second });

		auto fistCombo2_3 = fistSecond->AddLeftNode({ Player_AttackType::SWORD, animatorParameterTable });

		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 4 });
		animatorParameterTable.push_back({ TEXT("AnimationSpeed"), DUOLGameEngine::AnimatorControllerParameterType::Float, 1.f });
		animatorParameterTable.push_back({ TEXT("SFX"), DUOLGameEngine::AnimatorControllerParameterType::SFX, PlayerSoundTable::Hit_Sound });

		actionName = TEXT("NormalFist_FFSS");
		if (playerDataTable.contains(actionName))
			animatorParameterTable.push_back({ TEXT("Damage"), DUOLGameEngine::AnimatorControllerParameterType::Damage, playerDataTable.find(actionName)->second });

		auto fistCombo2_4 = fistCombo2_3->AddLeftNode({ Player_AttackType::SWORD_WAVE, animatorParameterTable
			, DUOLMath::Vector3(0.f, 0.5f, 0.2f), 0.f, 6.f,
			DUOLMath::Vector3::Forward * 50.f, 0.15f, DUOLMath::Vector3(4.f, 2.f, 0.5f), DUOLMath::Vector3(5.f, 2.f, 0.6f),true, DUOLMath::Vector2(1.5f, 1.5f) });

		fistCombo2_4->_nodeName = TEXT("Dust");
#pragma endregion

#pragma region OVERDRIVE_SWORD_COMBO_TREE
		// 1 타
		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 1 });

		actionName = TEXT("OverdriveSword_S");
		if (playerDataTable.contains(actionName))
			animatorParameterTable.push_back({ TEXT("Damage"), DUOLGameEngine::AnimatorControllerParameterType::Damage, playerDataTable.find(actionName)->second });

		_overdriveSwordComboTree = BinaryTree<Player_AttackData>({ Player_AttackType::SWORD, animatorParameterTable });

		// 2타
		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 2 });

		actionName = TEXT("OverdriveSword_SSS");
		if (playerDataTable.contains(actionName))
			animatorParameterTable.push_back({ TEXT("Damage"), DUOLGameEngine::AnimatorControllerParameterType::Damage, playerDataTable.find(actionName)->second });


		auto overdriveSwordSecond = _overdriveSwordComboTree.AddLeftNode({ Player_AttackType::SWORD, animatorParameterTable });

		// 3타
		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 3 });
		animatorParameterTable.push_back({ TEXT("SFX"), DUOLGameEngine::AnimatorControllerParameterType::SFX, PlayerSoundTable::Hit_Sound });

		actionName = TEXT("OverdriveSword_SSSSS");
		if (playerDataTable.contains(actionName))
			animatorParameterTable.push_back({ TEXT("Damage"), DUOLGameEngine::AnimatorControllerParameterType::Damage, playerDataTable.find(actionName)->second });

		auto overdriveSwordThird = overdriveSwordSecond->AddLeftNode({ Player_AttackType::SWORD_WAVE, animatorParameterTable
			, DUOLMath::Vector3(0.f, 0.5f, 0.2f), 0.f, 6.f, DUOLMath::Vector3::Forward * 40.f, 0.5f, DUOLMath::Vector3(2.f, 1.f, 0.3f) });

		overdriveSwordThird->_nodeName = TEXT("ODLastSword");
#pragma endregion

#pragma region OVERDRIVE_FIST_COMBO_TREE
		// 1타
		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 1 });
		animatorParameterTable.push_back({ TEXT("AnimationSpeed"), DUOLGameEngine::AnimatorControllerParameterType::Float, 0.9f });
		animatorParameterTable.push_back({ TEXT("SFX"), DUOLGameEngine::AnimatorControllerParameterType::SFX, PlayerSoundTable::Hit_Sound });

		actionName = TEXT("OverdriveFist_F");
		if (playerDataTable.contains(actionName))
			animatorParameterTable.push_back({ TEXT("Damage"), DUOLGameEngine::AnimatorControllerParameterType::Damage, playerDataTable.find(actionName)->second });

		_overdriveFistComboTree = BinaryTree<Player_AttackData>({ Player_AttackType::FIST, animatorParameterTable
			,DUOLMath::Vector3(0.f, 1.5f, -2.f), 2.f, 4.f });

		// 2타
		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 2 });
		animatorParameterTable.push_back({ TEXT("AnimationSpeed"), DUOLGameEngine::AnimatorControllerParameterType::Float, 0.9f });
		animatorParameterTable.push_back({ TEXT("SFX"), DUOLGameEngine::AnimatorControllerParameterType::SFX, PlayerSoundTable::Hit_Sound });

		actionName = TEXT("OverdriveFist_FF");
		if (playerDataTable.contains(actionName))
			animatorParameterTable.push_back({ TEXT("Damage"), DUOLGameEngine::AnimatorControllerParameterType::Damage, playerDataTable.find(actionName)->second });

		auto overdriveFistSecond = _overdriveFistComboTree.AddRightNode({ Player_AttackType::FIST, animatorParameterTable
			,DUOLMath::Vector3(0.f, 1.5f, -2.f), 2.f, 4.f });

		// 3타
		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 3 });
		animatorParameterTable.push_back({ TEXT("AnimationSpeed"), DUOLGameEngine::AnimatorControllerParameterType::Float, 0.9f });
		animatorParameterTable.push_back({ TEXT("SFX"), DUOLGameEngine::AnimatorControllerParameterType::SFX, PlayerSoundTable::Hit_Sound });

		actionName = TEXT("OverdriveFist_FFF");
		if (playerDataTable.contains(actionName))
			animatorParameterTable.push_back({ TEXT("Damage"), DUOLGameEngine::AnimatorControllerParameterType::Damage, playerDataTable.find(actionName)->second });

		auto overdriveFistThird = overdriveFistSecond->AddRightNode({ Player_AttackType::FIST_WAVE, animatorParameterTable
			, DUOLMath::Vector3(0.f, 0.5f, 0.2f), 0.f, 6.f,
			DUOLMath::Vector3::Forward * 15.f, 0.5f, DUOLMath::Vector3(2.f, 1.f, 0.3f) });

		overdriveFistThird->_nodeName = TEXT("ODLastFist");
#pragma endregion
	}

	void PlayerState_Attack::SettingCurrentComboNodeState()
	{
		auto&& animatorParameterTable = _currentComboTreeNode->_data._animatorParameterTable;
		_currentAudioClip = PlayerSoundTable::NONE;

		for (auto parameter : animatorParameterTable)
		{
			const DUOLCommon::tstring& parameterName = std::get<0>(parameter);

			DUOLGameEngine::AnimatorControllerParameterType parameterType = std::get<1>(parameter);

			std::any parameterValue = std::get<2>(parameter);

			switch (parameterType)
			{
			case DUOLGameEngine::AnimatorControllerParameterType::Bool:
			{
				_animator->SetBool(parameterName, std::any_cast<bool>(parameterValue));

				break;
			}

			case DUOLGameEngine::AnimatorControllerParameterType::Int:
			{
				_animator->SetInt(parameterName, std::any_cast<int>(parameterValue));

				break;
			}

			case DUOLGameEngine::AnimatorControllerParameterType::Float:
			{
				_animator->SetFloat(parameterName, std::any_cast<float>(parameterValue));

				break;
			}

			case DUOLGameEngine::AnimatorControllerParameterType::Trigger:
			{
				break;
			}
			case DUOLGameEngine::AnimatorControllerParameterType::SFX:
			{

				_currentAudioClip = std::any_cast<PlayerSoundTable>(parameterValue);

				break;
			}
			case DUOLGameEngine::AnimatorControllerParameterType::Damage:
			{

				_player->_currentDamage = std::any_cast<float>(parameterValue);
				break;
			}
			}
		}

		auto playerAttackType = _currentComboTreeNode->GetData()._attackType;

		if (playerAttackType == Player_AttackType::SWORD || playerAttackType == Player_AttackType::SWORD_WAVE)
		{
			if (_leftFistFormAura != nullptr)
			{
				// TODO : 서서히 사라지기
				_leftFistFormAura->Stop();

				_rightFistFormAura->Stop();

				_leftFistFormAura = nullptr;

				_rightFistFormAura = nullptr;
			}

			_player->_currentPlayerWeapon->HoldSword();
		}
		else if (playerAttackType == Player_AttackType::FIST || playerAttackType == Player_AttackType::FIST_WAVE)
		{
			// 오버드라이브 피스트 모드에는 이미 주먹에 뭐 달려있음.
			if (!InOverdriveFistCheck() && (_leftFistFormAura == nullptr))
			{
				_leftFistFormAura = DUOLClient::ParticleManager::GetInstance()->Pop(ParticleEnum::FistForm);

				_leftFistFormAura->GetTransform()->SetParent(_player->_playerLeftFistHolder->GetTransform(), false);

				_leftFistFormAura->GetTransform()->SetLocalPosition(DUOLMath::Vector3::Zero);

				_rightFistFormAura = DUOLClient::ParticleManager::GetInstance()->Pop(ParticleEnum::FistForm);

				_rightFistFormAura->GetTransform()->SetParent(_player->_playerRightFistHolder->GetTransform(), false);

				_rightFistFormAura->GetTransform()->SetLocalPosition(DUOLMath::Vector3::Zero);
			}

			_player->_currentPlayerWeapon->HouseSword();
			_player->_currentplayerWeaponSwordCollider->SetIsEnabled(false);
		}

		// 기본 상태 막타 시작 시, 슈퍼 아머 옵션 걸어줍니다.
		if (playerAttackType == Player_AttackType::FIST_WAVE || playerAttackType == Player_AttackType::SWORD_WAVE)
			_player->SetSuperArmor(true);
	}

	void PlayerState_Attack::EndAttack()
	{
		if (!_isOnStay)
			return;

		// 공격이 끝나면 Idle 로 넘어간다.
		_stateMachine->TransitionTo(TEXT("PlayerState_Idle"), 0.f);
	}

	void PlayerState_Attack::OnStateEnter(float deltaTime)
	{
		PlayerStateBase::OnStateEnter(deltaTime);

		//로테이션은 굳이..? 보고있던방향으로 그냥 공격한다.
		//_transform->LookAt(_transform->GetWorldPosition() + _desiredLook * 10.f);

		if (InOverdriveSwordCheck() && SwordAttackCheck())
			_currentComboTreeNode = &_overdriveSwordComboTree;
		else if (InOverdriveFistCheck() && FistAttackCheck())
			_currentComboTreeNode = &_overdriveFistComboTree;
		else if (!InOverdriveSwordCheck() && !InOverdriveFistCheck())
		{
			if (SwordAttackCheck())
			{
				_currentComboTreeNode = &_swordComboTree;
			}
			else if (FistAttackCheck())
			{
				_currentComboTreeNode = &_fistComboTree;
			}
		}



		SettingCurrentComboNodeState();
	}

	void PlayerState_Attack::OnStateStayFixed(float fixedTimeStep)
	{
		if (MoveCheck() && (SwordAttackCheck() || FistAttackCheck()))
		{
			if (_isInCancle && _nextComboTreeNode == nullptr)
			{
				if (!_player->_isLockOnMode)
				{
					_isRotateNow = true;

					float value = _rotateSlerpValue * _rotationSpeedSmoothness;

					auto& dir = _transform->GetLook();
					_fromRotateDirection = DUOLMath::Vector2{ dir.x, dir.z };
					_fromRotateDirection.Normalize();

					_toRotateDirection = DUOLMath::Vector2{ _desiredLook.x, _desiredLook.z };
					_toRotateDirection.Normalize();
					_rotateSlerpValue = 0;

					_rotationAngle = acos(_toRotateDirection.Dot(_fromRotateDirection));
					if (_rotationAngle > DUOLMath::PI - 0.1f)
					{
						_rotationAngle -= 0.1f;
					}
				}
			}
		}


		if (_isRotateNow)
		{
			//로테이트를합니다.
			ChangeDirection(fixedTimeStep);
		}

	}

	void PlayerState_Attack::OnStateStay(float deltaTime)
	{
		PlayerStateBase::OnStateStay(deltaTime);
		LookDirectionUpdate();

		if (LockOnCheck())
		{
			FindLockOnTarget();
		}

		if (DashCheck())
		{
			_stateMachine->TransitionTo(TEXT("PlayerState_Dash"), deltaTime);
		}
		else if (SwordAttackCheck())
		{
			// 캔슬 타임 도중에 입력이 되었다면. 만약, 캔슬 프레임 중 입력 한 번만 받고 싶으면 (무쌍 게임이랑은 맞지 않을지도 ..) 변수 추가할 것
			if (_isInCancle && _nextComboTreeNode == nullptr)
				_nextComboTreeNode = _currentComboTreeNode->GetLeftNode();
		}
		else if (FistAttackCheck())
		{
			if (_isInCancle && _nextComboTreeNode == nullptr)
				_nextComboTreeNode = _currentComboTreeNode->GetRightNode();
		}
		else if (UltimateCheck())
		{
			if (_player->_isOverdriveSwordMode)
				DUOLClient::SystemManager::GetInstance()->PlayerCameraAction("Player_Sword_Final", _player->GetTransform());

			else if (_player->_isOverdriveFistMode)
				DUOLClient::SystemManager::GetInstance()->PlayerCameraAction("Player_Ultimate_Fist_Final", _player->GetTransform());

			UIDataManager::GetInstance()->SetUltimateUI(0.f);

			_stateMachine->TransitionTo(TEXT("PlayerState_Ultimate"), deltaTime);

			DUOL_TRACE(DUOL_CONSOLE, "Attack | UltStart ()");
		}
	}

	void PlayerState_Attack::OnStateExit(float deltaTime)
	{
		PlayerStateBase::OnStateExit(deltaTime);

		std::function<DUOLGameEngine::CoroutineHandler(float)> routine = std::bind(&DUOLClient::PlayerState_Attack::SetPostDelay, this, std::placeholders::_1);

		// TODO : 후딜 적용
		if (_currentComboTreeNode->GetLeftNode() == nullptr && _currentComboTreeNode->GetRightNode() == nullptr)
		{
			_player->StartCoroutine(routine, _player->_endAttackPostDelay);

			// 오버 드라이브 상태가 아니면, 막타 슈퍼아머 해제
			if (!InOverdriveCheck())
				_player->SetSuperArmor(false);
		}
		else
		{
			_player->StartCoroutine(routine, _player->_inAttackPostDelay);
		}

		EndSwordTrailFrame();

		_isInCancle = false;
		_isRotateNow = false;
		_currentComboTreeNode = nullptr;

		_nextComboTreeNode = nullptr;

		_animator->SetBool(TEXT("IsAttack"), false);
		_animator->SetBool(TEXT("IsSword"), false);
		_animator->SetBool(TEXT("IsFist"), false);
		_animator->SetFloat(TEXT("AnimationSpeed"), 1.f);
		_animator->SetInt(TEXT("AttackCount"), 0);

		// 오버드라이브 상태가 아닐 때, 공격이 끝나면 장비 중인 한손검을 들게한다.
		if (!InOverdriveCheck())
		{
			//_player->_playerWeaponSword->HoldSword();

			if (_leftFistFormAura != nullptr)
			{
				_leftFistFormAura->Stop();

				_rightFistFormAura->Stop();

				_leftFistFormAura = nullptr;

				_rightFistFormAura = nullptr;
			}
		}
	}
}
