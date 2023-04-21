#include "DUOLClient/Player/FSM/PlayerState_Attack.h"

#include "DUOLClient/Camera/MainCameraController.h"
#include "DUOLClient/ECS/Component/Enemy/AI_EnemyBasic.h"
#include "DUOLClient/Manager/ParticleManager.h"
#include "DUOLGameEngine/Manager/PhysicsManager.h"

#include "DUOLCommon/Log/LogHelper.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Transform.h"
#include "DUOLGameEngine/ECS/Component/BoxCollider.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"
#include "DUOLGameEngine/ECS/Component/ParticleRenderer.h"

#include "DUOLClient/Player/Weapon_Sword.h"
#include "DUOLClient/Player/Weapon_Wave.h"
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
#pragma endregion

		// 타격 프레임 이벤트 함수 등록
#pragma region HIT_FRAME_EVENT
		_player->AddEventFunction(TEXT("StartSwordAttackFrame"), std::bind(&DUOLClient::PlayerState_Attack::StartSwordAttackFrame, this));

		_player->AddEventFunction(TEXT("EndSwordAttackFrame"), std::bind(&DUOLClient::PlayerState_Attack::EndSwordAttackFrame, this));

		_player->AddEventFunction(TEXT("FistHit"), std::bind(&DUOLClient::PlayerState_Attack::FistHit, this));

		_player->AddEventFunction(TEXT("WaveHit"), std::bind(&DUOLClient::PlayerState_Attack::WaveHit, this));
#pragma endregion

		// 콤보 데이터 초기화
#pragma region COMBO_DATA_INITIALIZE
		BuildComboTree();
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
			_animator->SetFloat(TEXT("AnimationSpeed"), 0.7f);
		}
	}

	void PlayerState_Attack::EndCancleFrame()
	{
		if (!_isOnStay)
			return;

		// DUOL_INFO(DUOL_CONSOLE, "End cancel frame.");

		_isInCancle = false;

		CheckCanEnterNextAttack();
	}

	void PlayerState_Attack::StartSwordAttackFrame()
	{
		_player->_playerWeaponSwordCollider->SetIsEnabled(true);
	}

	void PlayerState_Attack::EndSwordAttackFrame()
	{
		_player->_playerWeaponSwordCollider->SetIsEnabled(false);
	}

	void PlayerState_Attack::FistHit()
	{
		if (!_isOnStay)
			return;

		std::vector<DUOLPhysics::RaycastHit> spherecastHits;

		auto&& comboNodeData = _currentComboTreeNode->GetData();

		const DUOLMath::Vector3& hitCenterOffset = comboNodeData._hitCenterOffset;

		DUOLMath::Vector3 startPos = _transform->GetWorldPosition() + _transform->GetRight() * hitCenterOffset.x + _transform->GetUp() * hitCenterOffset.y + _transform->GetLook() * hitCenterOffset.z;

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

					_player->Attack(aiEnemy, _player->_currentDamage + 10.f, AttackType::LightAttack);

					auto particleData = ParticleManager::GetInstance()->Pop(ParticleEnum::MonsterHit, 1.0f);

					particleData->GetTransform()->SetPosition(hited._hitPosition, DUOLGameEngine::Space::World);

					// 오버 드라이브 상태 아니면 오버드라이브 포인트 업 !
					if (!InOverdriveSwordCheck() && !InOverdriveFistCheck())
						_player->_currentOverdrivePoint += OVERDRIVE_POINT_PER_FIST;
				}
			}
		}

		_mainCamController->SetCameraShake(0.5f, DUOLMath::Vector2(4.f, 4.f));
	}

	void PlayerState_Attack::WaveHit()
	{
		// 충격파 계열
		if (!_isOnStay)
			return;

		std::function<DUOLGameEngine::CoroutineHandler(void)> routine = std::bind(&DUOLClient::PlayerState_Attack::LaunchWave, this);

		// 후딜 적용
		_player->StartCoroutine(routine);

		_mainCamController->SetCameraShake(0.5f, DUOLMath::Vector2(6.f, 6.f));
	}

	DUOLGameEngine::CoroutineHandler PlayerState_Attack::SetPostDelay(float delayTime)
	{
		_player->_canStartAttack = false;

		co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(delayTime);

		_player->_canStartAttack = true;
	}

	DUOLGameEngine::CoroutineHandler PlayerState_Attack::LaunchWave()
	{
		auto&& data = _currentComboTreeNode->GetData();

		DUOLGameEngine::Transform* playerTransform = _player->GetTransform();

		_player->_playerWeaponWave->StartWave(playerTransform->GetWorldPosition() + data._hitCenterOffset, data._waveBoxhalfExtents,
 DUOLMath::Vector3::TransformNormal(data._waveVelocity, playerTransform->GetWorldMatrix()), playerTransform->GetWorldRotation());

		co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(data._waveTime);

		_player->_playerWeaponWave->EndWave();
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

		_animator->SetFloat(TEXT("AnimationSpeed"), 1.0f);
	}

	void PlayerState_Attack::BuildComboTree()
	{
		// TODO : 구조화된 데이터를 받아서 콤보 트리를 빌드할 수 있도록 짜놓는다.
#pragma region SWORD_COMBO_TREE
		// 1 타
		std::vector<std::tuple<DUOLCommon::tstring, DUOLGameEngine::AnimatorControllerParameterType, std::any>> animatorParameterTable;

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 1 });

		_swordComboTree = BinaryTree<Player_AttackData>({Player_AttackType::SWORD, animatorParameterTable});

		// 2타
		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 2 });

		auto swordSecond = _swordComboTree.AddLeftNode({ Player_AttackType::SWORD, animatorParameterTable });

		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 2 });

		auto swordCombo1_2 = _swordComboTree.AddRightNode({ Player_AttackType::FIST, animatorParameterTable
			,DUOLMath::Vector3(0.f, 1.5f, -2.f), 2.f, 4.f });

		// 3타
		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 3 });

		auto swordThird = swordSecond->AddLeftNode({ Player_AttackType::SWORD, animatorParameterTable });

		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 3 });

		auto swordCombo2_3 = swordSecond->AddRightNode({ Player_AttackType::FIST, animatorParameterTable
			, DUOLMath::Vector3(0.f, 1.5f, -2.f), 2.f, 4.f });

		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 3 });

		auto swordCombo1_3 = swordCombo1_2->AddRightNode({ Player_AttackType::FIST_WAVE, animatorParameterTable
			, DUOLMath::Vector3(0.f, 0.5f, 0.2f), 0.f, 6.f, DUOLMath::Vector3::Forward * 20.f, 0.25f, DUOLMath::Vector3(3.f, 2.f, 0.3f) });

		// 4타
		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 4 });

		auto swordFourth = swordThird->AddLeftNode({ Player_AttackType::SWORD_WAVE, animatorParameterTable
			, DUOLMath::Vector3(0.f, 0.5f, 0.2f), 0.f, 6.f, DUOLMath::Vector3::Forward * 20.f, 0.25f, DUOLMath::Vector3(3.f, 2.f, 0.3f) });

		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 4 });

		auto swordCombo2_4 = swordCombo2_3->AddRightNode({ Player_AttackType::FIST_WAVE, animatorParameterTable
			, DUOLMath::Vector3(0.f, 0.5f, 0.2f), 0.f, 6.f, DUOLMath::Vector3::Forward * 20.f, 0.25f, DUOLMath::Vector3(3.f, 2.f, 0.3f) });

		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 4 });

		auto swordCombo3_4 = swordThird->AddRightNode({ Player_AttackType::FIST_WAVE, animatorParameterTable
			, DUOLMath::Vector3(0.f, 0.5f, 0.2f), 0.f, 6.f, DUOLMath::Vector3::Forward * 20.f, 0.25f, DUOLMath::Vector3(3.f, 2.f, 0.3f) });
#pragma endregion

#pragma region FIST_COMBO_TREE
		// 1타
		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 1 });

		_fistComboTree = BinaryTree<Player_AttackData>({ Player_AttackType::FIST, animatorParameterTable
			,DUOLMath::Vector3(0.f, 1.5f, -2.f), 2.f, 4.f});

		// 2타
		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 2 });

		auto fistSecond = _fistComboTree.AddRightNode({ Player_AttackType::FIST, animatorParameterTable
			,DUOLMath::Vector3(0.f, 1.5f, -2.f), 2.f, 4.f });

		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 2 });

		auto fistCombo1_2 = _fistComboTree.AddLeftNode({ Player_AttackType::SWORD, animatorParameterTable });

		// 3타
		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 3 });

		auto fistThird = fistSecond->AddRightNode({ Player_AttackType::FIST_WAVE, animatorParameterTable
			, DUOLMath::Vector3(0.f, 0.5f, 0.2f), 0.f, 6.f, DUOLMath::Vector3::Forward * 15.f, 0.5f, DUOLMath::Vector3(2.f, 1.f, 0.3f) });

		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 3 });

		auto fistCombo1_3 = fistCombo1_2->AddLeftNode({ Player_AttackType::SWORD_WAVE, animatorParameterTable
			, DUOLMath::Vector3(0.f, 0.5f, 0.2f), 0.f, 6.f, DUOLMath::Vector3::Forward * 15.f, 0.5f, DUOLMath::Vector3(2.f, 1.f, 0.3f) });

		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 3 });

		auto fistCombo2_3 = fistSecond->AddLeftNode({ Player_AttackType::SWORD, animatorParameterTable });

		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 4 });

		auto fistCombo2_4 = fistCombo2_3->AddLeftNode({ Player_AttackType::SWORD_WAVE, animatorParameterTable
			, DUOLMath::Vector3(0.f, 0.5f, 0.2f), 0.f, 6.f, DUOLMath::Vector3::Forward * 15.f, 0.5f, DUOLMath::Vector3(2.f, 1.f, 0.3f) });
#pragma endregion

#pragma region OVERDRIVE_SWORD_COMBO_TREE
		// 1 타
		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 1 });

		_overdriveSwordComboTree = BinaryTree<Player_AttackData>({ Player_AttackType::SWORD, animatorParameterTable });

		// 2타
		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 2 });

		auto overdriveSwordSecond = _overdriveSwordComboTree.AddLeftNode({ Player_AttackType::SWORD, animatorParameterTable });

		// 3타
		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 3 });

		auto overdriveSwordThird = overdriveSwordSecond->AddLeftNode({ Player_AttackType::SWORD_WAVE, animatorParameterTable
			, DUOLMath::Vector3(0.f, 0.5f, 0.2f), 0.f, 6.f, DUOLMath::Vector3::Forward * 15.f, 0.5f, DUOLMath::Vector3(2.f, 1.f, 0.3f) });
#pragma endregion

#pragma region OVERDRIVE_FIST_COMBO_TREE
		// 1타
		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 1 });

		_overdriveFistComboTree = BinaryTree<Player_AttackData>({ Player_AttackType::FIST, animatorParameterTable
			,DUOLMath::Vector3(0.f, 1.5f, -2.f), 2.f, 4.f });

		// 2타
		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 2 });

		auto overdriveFistSecond = _overdriveFistComboTree.AddRightNode({ Player_AttackType::FIST, animatorParameterTable
			,DUOLMath::Vector3(0.f, 1.5f, -2.f), 2.f, 4.f });

		// 3타
		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 3 });

		auto overdriveFistThird = overdriveFistSecond->AddRightNode({ Player_AttackType::FIST_WAVE, animatorParameterTable
			, DUOLMath::Vector3(0.f, 0.5f, 0.2f), 0.f, 6.f, DUOLMath::Vector3::Forward * 15.f, 0.5f, DUOLMath::Vector3(2.f, 1.f, 0.3f) });
#pragma endregion
	}

	void PlayerState_Attack::SettingCurrentComboNodeState()
	{
		auto&& animatorParameterTable = _currentComboTreeNode->_data._animatorParameterTable;

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
			}
		}

		_currentComboTreeNode->GetData()._attackType == Player_AttackType::SWORD || _currentComboTreeNode->GetData()._attackType == Player_AttackType::SWORD_WAVE
			? _player->_playerWeaponSword->HoldSword()
			: _player->_playerWeaponSword->HouseWeapon();
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

		if (InOverdriveSwordCheck() && SwordAttackCheck())
			_currentComboTreeNode = &_overdriveSwordComboTree;
		else if (InOverdriveFistCheck() && FistAttackCheck())
			_currentComboTreeNode = &_overdriveFistComboTree;
		else if (!InOverdriveSwordCheck() && !InOverdriveFistCheck())
		{
			if (SwordAttackCheck())
				_currentComboTreeNode = &_swordComboTree;
			else if (FistAttackCheck())
				_currentComboTreeNode = &_fistComboTree;
		}

		SettingCurrentComboNodeState();
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
	}

	void PlayerState_Attack::OnStateExit(float deltaTime)
	{
		PlayerStateBase::OnStateExit(deltaTime);

		std::function<DUOLGameEngine::CoroutineHandler(float)> routine = std::bind(&DUOLClient::PlayerState_Attack::SetPostDelay, this, std::placeholders::_1);

		// 후딜 적용
		(_currentComboTreeNode->GetLeftNode() == nullptr && _currentComboTreeNode->GetRightNode() == nullptr)
			? _player->StartCoroutine(routine, _player->_endAttackPostDelay)
			: _player->StartCoroutine(routine, _player->_inAttackPostDelay);

		_isInCancle = false;

		_currentComboTreeNode = nullptr;

		_nextComboTreeNode = nullptr;

		_animator->SetBool(TEXT("IsAttack"), false);
		_animator->SetBool(TEXT("IsSword"), false);
		_animator->SetBool(TEXT("IsFist"), false);
		_animator->SetInt(TEXT("AttackCount"), 0);

		// 오버드라이브 상태가 아닐 때, 공격이 끝나면 장비 중인 한손검을 들게한다.
		if (!InOverdriveCheck())
			_player->_playerWeaponSword->HoldSword();
	}
}