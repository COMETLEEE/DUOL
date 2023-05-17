#include "DUOLClient/Player/FSM/PlayerState_Attack.h"

#include "DUOLClient/Camera/MainCameraController.h"
#include "DUOLClient/ECS/Component/Enemy/AI_EnemyBasic.h"
#include "DUOLClient/Manager/GameManager.h"
#include "DUOLClient/Manager/ParticleManager.h"
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
#include "DUOLGameEngine/Util/Coroutine/WaitForSeconds.h"

namespace DUOLClient
{
	PlayerState_Attack::PlayerState_Attack(DUOLClient::Player* player) :
		PlayerStateBase(TEXT("PlayerState_Attack"), player)
		, _currentComboTreeNode(nullptr)
	{
		// ĵ�� ������ ���� �̺�Ʈ �Լ� ���
#pragma region CANCLE_FRAME_EVENT
		_player->AddEventFunction(TEXT("StartCancleFrame"), std::bind(&DUOLClient::PlayerState_Attack::StartCancleFrame, this));

		_player->AddEventFunction(TEXT("StartSlowFrame"), std::bind(&DUOLClient::PlayerState_Attack::StartSlowFrame, this));

		_player->AddEventFunction(TEXT("EndCancleFrame"), std::bind(&DUOLClient::PlayerState_Attack::EndCancleFrame, this));

		_player->AddEventFunction(TEXT("EndAttack"), std::bind(&DUOLClient::PlayerState_Attack::EndAttack, this));
#pragma endregion

		// Ÿ�� ������ �̺�Ʈ �Լ� ���
#pragma region HIT_FRAME_EVENT
		_player->AddEventFunction(TEXT("StartSwordAttackFrame"), std::bind(&DUOLClient::PlayerState_Attack::StartSwordAttackFrame, this));

		_player->AddEventFunction(TEXT("EndSwordAttackFrame"), std::bind(&DUOLClient::PlayerState_Attack::EndSwordAttackFrame, this));

		_player->AddEventFunction(TEXT("FistHit"), std::bind(&DUOLClient::PlayerState_Attack::FistHit, this));

		_player->AddEventFunction(TEXT("WaveHit"), std::bind(&DUOLClient::PlayerState_Attack::WaveHit, this));

		_player->AddEventFunction(TEXT("AreaWaveHit"), std::bind(&DUOLClient::PlayerState_Attack::AreaWaveHit, this));

#pragma endregion

		// �޺� ������ �ʱ�ȭ
#pragma region COMBO_DATA_INITIALIZE
		BuildComboTree();
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

		_nextComboTreeNode = nullptr;
	}

	void PlayerState_Attack::StartSlowFrame()
	{
		if (!_isOnStay)
			return;

		if (_nextComboTreeNode != nullptr)
		{
			// �״��. �̹� �Է��� �־ ���� ������ ���õǾ���.
		}
		else
		{
			// ��� �ӵ� ����.
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
		// ���� �浹 On
		_player->_playerWeaponSwordCollider->SetIsEnabled(true);
	}

	void PlayerState_Attack::EndSwordAttackFrame()
	{
		// ���� �浹 Off
		_player->_playerWeaponSwordCollider->SetIsEnabled(false);
	}

	void PlayerState_Attack::StartSwordTrailFrame()
	{
		// �ִ� 0.5�� (30 ������ �Ѿ�°� ����)
		_normalSwordTrail = DUOLClient::ParticleManager::GetInstance()->Pop(ParticleEnum::SwordTrail, 0.5f);

		_normalSwordTrail->GetTransform()->SetParent(_player->_playerWeaponSword->GetTransform());

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

		if (DUOLGameEngine::PhysicsManager::GetInstance()->SpherecastAll(startPos, _transform->GetLook(), comboNodeData._hitRadius, comboNodeData._hitMaxDistance, spherecastHits))
		{
			for (auto hited : spherecastHits)
			{
				DUOLGameEngine::GameObject* gameObject = reinterpret_cast<DUOLGameEngine::GameObject*>(hited._userData);

				// �����Դϴ�. �¾ҽ��ϴ�.
				if (gameObject->GetTag() == TEXT("Enemy"))
				{
					// ���� ��� �������� ���� ���. ���ش�.
					if (hited._hitPosition == DUOLMath::Vector3::Zero)
						continue;

					auto aiEnemy = gameObject->GetComponent<DUOLClient::CharacterBase>();

					_player->Attack(aiEnemy, _player->_currentDamage + 10.f, AttackType::LightAttack);

					auto particleData = ParticleManager::GetInstance()->Pop(ParticleEnum::MonsterHit, 1.0f);

					particleData->GetTransform()->SetPosition(hited._hitPosition, DUOLGameEngine::Space::World);

					// ���� ����̺� ���� �ƴϸ� ��������̺� ����Ʈ �� !
					if (!InOverdriveSwordCheck() && !InOverdriveFistCheck())
						_player->_currentOverdrivePoint += OVERDRIVE_POINT_PER_FIST;
				}
			}
		}
	}

	void PlayerState_Attack::WaveHit()
	{
		// ����� �迭
		if (!_isOnStay)
			return;

		// ���̺� ���� �����ϴ�.
		std::function<DUOLGameEngine::CoroutineHandler(void)> routine = std::bind(&DUOLClient::PlayerState_Attack::LaunchWave, this);

		_player->StartCoroutine(routine);

		// ����Ʈ ����
		auto& nodeName = _currentComboTreeNode->_nodeName;

		if (nodeName == TEXT("Dust"))
		{
			auto particleRenderer = DUOLClient::ParticleManager::GetInstance()->Pop(ParticleEnum::Dust, 5.f);

			particleRenderer->GetTransform()->SetPosition(_transform->GetWorldPosition(), DUOLGameEngine::Space::World);
		}
		else if (nodeName == TEXT("Crack"))
		{
			auto particleRenderer = DUOLClient::ParticleManager::GetInstance()->Pop(ParticleEnum::Crack, 5.f);

			particleRenderer->GetTransform()->SetPosition(_transform->GetWorldPosition(), DUOLGameEngine::Space::World);
		}
		else if (nodeName == TEXT("FistWide"))
		{
			auto particleRenderer = DUOLClient::ParticleManager::GetInstance()->Pop(ParticleEnum::FistWide, 5.f);

			// ����Ʈ ��ġ
			particleRenderer->GetTransform()->SetPosition(_transform->GetWorldPosition() + _transform->GetLook() * 1.5f + DUOLMath::Vector3::Up * 1.f, DUOLGameEngine::Space::World);

			// �÷��̾� ���� �ٶ󺸵��� ȸ��
			DUOLMath::Quaternion rot = DUOLMath::Quaternion::CreateFromAxisAngle(_transform->GetRight(), DUOLMath::MathHelper::DegreeToRadian(-90.f));

			particleRenderer->GetTransform()->SetRotation(rot);
		}

		// (Wave Hit �迭 == ��Ÿ) �� ī�޶� ����ũ�� �ִ�.
		if (_currentComboTreeNode->_data._useCamShake)
			_mainCamController->SetCameraShake(0.25f, _currentComboTreeNode->_data._shakePower * 2.f);

		DUOLClient::GameMessage<float> mes = { GameMessageType::BULLET_TIME_ALL, 1.f };

		DUOLClient::GameManager::GetInstance()->PushGameMessage(std::move(mes));
	}

	void PlayerState_Attack::AreaWaveHit()
	{
		// ����� �迭
		if (!_isOnStay)
			return;

		std::function<DUOLGameEngine::CoroutineHandler(void)> routine = std::bind(&DUOLClient::PlayerState_Attack::LaunchAreaWave, this);

		// �ĵ� ����
		_player->StartCoroutine(routine);

		// ����Ʈ ����
		auto& nodeName = _currentComboTreeNode->_nodeName;

		if (nodeName == TEXT("Dust"))
		{
			auto particleRenderer = DUOLClient::ParticleManager::GetInstance()->Pop(ParticleEnum::Dust, 5.f);

			particleRenderer->GetTransform()->SetPosition(_transform->GetWorldPosition(), DUOLGameEngine::Space::World);
		}
		else if (nodeName == TEXT("Crack"))
		{
			auto particleRenderer = DUOLClient::ParticleManager::GetInstance()->Pop(ParticleEnum::Crack, 5.f);

			particleRenderer->GetTransform()->SetPosition(_transform->GetWorldPosition(), DUOLGameEngine::Space::World);
		}
		else if (nodeName == TEXT("FistWide"))
		{
			auto particleRenderer = DUOLClient::ParticleManager::GetInstance()->Pop(ParticleEnum::FistWide, 5.f);

			particleRenderer->GetTransform()->SetPosition(_transform->GetWorldPosition() + _transform->GetLook() * 1.f, DUOLGameEngine::Space::World);
		}

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

	DUOLGameEngine::CoroutineHandler PlayerState_Attack::LaunchAreaWave()
	{
		auto&& data = _currentComboTreeNode->GetData();

		DUOLGameEngine::Transform* playerTransform = _player->GetTransform();

		_player->_playerWeaponAreaWave->StartAreaWave(playerTransform->GetWorldPosition(), playerTransform->GetWorldRotation(), data._waveTime);

		co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(data._waveTime);

		_player->_playerWeaponAreaWave->EndAreaWave();
	}

	void PlayerState_Attack::CheckCanEnterNextAttack()
	{
		if (!_isOnStay)
			return;

		// ĵ�� ������ �������� ��Ȯ�� ���� Ŀ�ǵ尡 �������� �ʾҴٸ� ��ȯ ..!
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

	void PlayerState_Attack::BuildComboTree()
	{
		// TODO : ����ȭ�� �����͸� �޾Ƽ� �޺� Ʈ���� ������ �� �ֵ��� ¥���´�.
#pragma region SWORD_COMBO_TREE
		// 1 Ÿ
		std::vector<std::tuple<DUOLCommon::tstring, DUOLGameEngine::AnimatorControllerParameterType, std::any>> animatorParameterTable;

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 1 });
		animatorParameterTable.push_back({ TEXT("AnimationSpeed"), DUOLGameEngine::AnimatorControllerParameterType::Float, 1.2f });

		_swordComboTree = BinaryTree<Player_AttackData>({Player_AttackType::SWORD, animatorParameterTable});

		// 2Ÿ
		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 2 });
		animatorParameterTable.push_back({ TEXT("AnimationSpeed"), DUOLGameEngine::AnimatorControllerParameterType::Float, 1.2f });

		auto swordSecond = _swordComboTree.AddLeftNode({ Player_AttackType::SWORD, animatorParameterTable });

		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 2 });
		animatorParameterTable.push_back({ TEXT("AnimationSpeed"), DUOLGameEngine::AnimatorControllerParameterType::Float, 0.8f });

		auto swordCombo1_2 = _swordComboTree.AddRightNode({ Player_AttackType::FIST, animatorParameterTable
			,DUOLMath::Vector3(0.f, 1.5f, -2.f), 2.f, 4.f });

		// 3Ÿ
		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 3 });
		animatorParameterTable.push_back({ TEXT("AnimationSpeed"), DUOLGameEngine::AnimatorControllerParameterType::Float, 1.2f });

		auto swordThird = swordSecond->AddLeftNode({ Player_AttackType::SWORD, animatorParameterTable });

		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 3 });
		animatorParameterTable.push_back({ TEXT("AnimationSpeed"), DUOLGameEngine::AnimatorControllerParameterType::Float, 0.8f });

		auto swordCombo2_3 = swordSecond->AddRightNode({ Player_AttackType::FIST, animatorParameterTable
			, DUOLMath::Vector3(0.f, 1.5f, -2.f), 2.f, 4.f });

		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 3 });
		animatorParameterTable.push_back({ TEXT("AnimationSpeed"), DUOLGameEngine::AnimatorControllerParameterType::Float, 0.7f });

		auto swordCombo1_3 = swordCombo1_2->AddRightNode({ Player_AttackType::FIST_WAVE, animatorParameterTable
			, DUOLMath::Vector3(0.f, 0.5f, 0.2f), 0.f, 6.f,
			DUOLMath::Vector3::Forward * 20.f, 0.25f, DUOLMath::Vector3(3.f, 2.f, 0.3f), true, DUOLMath::Vector2(1.5f, 1.5f) });

		swordCombo1_3->_nodeName = TEXT("FistWide");

		// 4Ÿ
		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 4 });
		animatorParameterTable.push_back({ TEXT("AnimationSpeed"), DUOLGameEngine::AnimatorControllerParameterType::Float, 1.2f });

		auto swordFourth = swordThird->AddLeftNode({ Player_AttackType::SWORD_WAVE, animatorParameterTable
			, DUOLMath::Vector3(0.f, 0.5f, 0.2f), 0.f, 6.f,
			DUOLMath::Vector3::Forward * 20.f, 0.25f, DUOLMath::Vector3(3.f, 2.f, 0.3f), true, DUOLMath::Vector2(4.f, 4.f)});

		swordFourth->_nodeName = TEXT("Dust");

		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 4 });
		animatorParameterTable.push_back({ TEXT("AnimationSpeed"), DUOLGameEngine::AnimatorControllerParameterType::Float, 1.0f });

		auto swordCombo2_4 = swordCombo2_3->AddRightNode({ Player_AttackType::FIST_WAVE, animatorParameterTable
			, DUOLMath::Vector3(0.f, 0.5f, 0.2f), 0.f, 6.f,
			DUOLMath::Vector3::Forward * 20.f, 0.25f, DUOLMath::Vector3(3.f, 2.f, 0.3f), true, DUOLMath::Vector2(3.75f, 3.75f) });

		swordCombo2_4->_nodeName = TEXT("Crack");

		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 4 });
		animatorParameterTable.push_back({ TEXT("AnimationSpeed"), DUOLGameEngine::AnimatorControllerParameterType::Float, 1.f });

		auto swordCombo3_4 = swordThird->AddRightNode({ Player_AttackType::FIST_WAVE, animatorParameterTable
			, DUOLMath::Vector3(0.f, 0.5f, 0.2f), 0.f, 6.f,
			DUOLMath::Vector3::Forward * 20.f, 0.25f, DUOLMath::Vector3(3.f, 2.f, 0.3f), true, DUOLMath::Vector2(4.5f, 4.5f) });

		swordCombo3_4->_nodeName = TEXT("Crack");
#pragma endregion

#pragma region FIST_COMBO_TREE
		// 1Ÿ
		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 1 });
		animatorParameterTable.push_back({ TEXT("AnimationSpeed"), DUOLGameEngine::AnimatorControllerParameterType::Float, 0.8f});

		_fistComboTree = BinaryTree<Player_AttackData>({ Player_AttackType::FIST, animatorParameterTable
			,DUOLMath::Vector3(0.f, 1.5f, -2.f), 2.f, 4.f});

		// 2Ÿ
		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 2 });
		animatorParameterTable.push_back({ TEXT("AnimationSpeed"), DUOLGameEngine::AnimatorControllerParameterType::Float, 0.8f });

		auto fistSecond = _fistComboTree.AddRightNode({ Player_AttackType::FIST, animatorParameterTable
			,DUOLMath::Vector3(0.f, 1.5f, -2.f), 2.f, 4.f });

		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 2 });
		animatorParameterTable.push_back({ TEXT("AnimationSpeed"), DUOLGameEngine::AnimatorControllerParameterType::Float, 1.2f });

		auto fistCombo1_2 = _fistComboTree.AddLeftNode({ Player_AttackType::SWORD, animatorParameterTable });

		// 3Ÿ
		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 3 });
		animatorParameterTable.push_back({ TEXT("AnimationSpeed"), DUOLGameEngine::AnimatorControllerParameterType::Float, 0.8f });

		auto fistThird = fistSecond->AddRightNode({ Player_AttackType::FIST_WAVE, animatorParameterTable
			, DUOLMath::Vector3(0.f, 0.5f, 0.2f), 0.f, 6.f,
			DUOLMath::Vector3::Forward * 15.f, 0.5f, DUOLMath::Vector3(2.f, 1.f, 0.3f), true, DUOLMath::Vector2(1.5f, 1.5f) });

		fistThird->_nodeName = TEXT("FistWide");

		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 3 });
		animatorParameterTable.push_back({ TEXT("AnimationSpeed"), DUOLGameEngine::AnimatorControllerParameterType::Float, 1.f });

		auto fistCombo1_3 = fistCombo1_2->AddLeftNode({ Player_AttackType::SWORD_WAVE, animatorParameterTable
			, DUOLMath::Vector3(0.f, 0.5f, 0.2f), 0.f, 6.f,
			DUOLMath::Vector3::Forward * 15.f, 0.5f, DUOLMath::Vector3(2.f, 1.f, 0.3f), true, DUOLMath::Vector2(2.f, 2.f) });

		fistCombo1_3->_nodeName = TEXT("Dust");

		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 3 });
		animatorParameterTable.push_back({ TEXT("AnimationSpeed"), DUOLGameEngine::AnimatorControllerParameterType::Float, 1.f });

		auto fistCombo2_3 = fistSecond->AddLeftNode({ Player_AttackType::SWORD, animatorParameterTable });

		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 4 });
		animatorParameterTable.push_back({ TEXT("AnimationSpeed"), DUOLGameEngine::AnimatorControllerParameterType::Float, 1.f });

		auto fistCombo2_4 = fistCombo2_3->AddLeftNode({ Player_AttackType::SWORD_WAVE, animatorParameterTable
			, DUOLMath::Vector3(0.f, 0.5f, 0.2f), 0.f, 6.f,
			DUOLMath::Vector3::Forward * 15.f, 0.5f, DUOLMath::Vector3(2.f, 1.f, 0.3f), true, DUOLMath::Vector2(1.5f, 1.5f) });

		fistCombo2_4->_nodeName = TEXT("Dust");
#pragma endregion

#pragma region OVERDRIVE_SWORD_COMBO_TREE
		// 1 Ÿ
		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 1 });

		_overdriveSwordComboTree = BinaryTree<Player_AttackData>({ Player_AttackType::SWORD, animatorParameterTable });

		// 2Ÿ
		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 2 });

		auto overdriveSwordSecond = _overdriveSwordComboTree.AddLeftNode({ Player_AttackType::SWORD, animatorParameterTable });

		// 3Ÿ
		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 3 });

		auto overdriveSwordThird = overdriveSwordSecond->AddLeftNode({ Player_AttackType::SWORD_WAVE, animatorParameterTable
			, DUOLMath::Vector3(0.f, 0.5f, 0.2f), 0.f, 6.f, DUOLMath::Vector3::Forward * 15.f, 0.5f, DUOLMath::Vector3(2.f, 1.f, 0.3f) });

		overdriveSwordThird->_nodeName = TEXT("Crack");
#pragma endregion

#pragma region OVERDRIVE_FIST_COMBO_TREE
		// 1Ÿ
		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 1 });
		animatorParameterTable.push_back({ TEXT("AnimationSpeed"), DUOLGameEngine::AnimatorControllerParameterType::Float, 0.9f });

		_overdriveFistComboTree = BinaryTree<Player_AttackData>({ Player_AttackType::FIST, animatorParameterTable
			,DUOLMath::Vector3(0.f, 1.5f, -2.f), 2.f, 4.f });

		// 2Ÿ
		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 2 });
		animatorParameterTable.push_back({ TEXT("AnimationSpeed"), DUOLGameEngine::AnimatorControllerParameterType::Float, 0.9f });

		auto overdriveFistSecond = _overdriveFistComboTree.AddRightNode({ Player_AttackType::FIST, animatorParameterTable
			,DUOLMath::Vector3(0.f, 1.5f, -2.f), 2.f, 4.f });

		// 3Ÿ
		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 3 });
		animatorParameterTable.push_back({ TEXT("AnimationSpeed"), DUOLGameEngine::AnimatorControllerParameterType::Float, 0.9f });

		auto overdriveFistThird = overdriveFistSecond->AddRightNode({ Player_AttackType::FIST_WAVE, animatorParameterTable
			, DUOLMath::Vector3(0.f, 0.5f, 0.2f), 0.f, 6.f, 
			DUOLMath::Vector3::Forward * 15.f, 0.5f, DUOLMath::Vector3(2.f, 1.f, 0.3f) });

		overdriveFistThird->_nodeName = TEXT("Crack");
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

		auto playerAttackType = _currentComboTreeNode->GetData()._attackType;

		if (playerAttackType == Player_AttackType::SWORD || playerAttackType == Player_AttackType::SWORD_WAVE)
		{
			if (_leftFistFormAura != nullptr)
			{
				// TODO : ������ �������
				_leftFistFormAura->Stop();

				_rightFistFormAura->Stop();

				_leftFistFormAura = nullptr;

				_rightFistFormAura = nullptr;
			}

			_player->_playerWeaponSword->HoldSword();
		}
		else if (playerAttackType == Player_AttackType::FIST || playerAttackType == Player_AttackType::FIST_WAVE)
		{
			// ��������̺� �ǽ�Ʈ ��忡�� �̹� �ָԿ� �� �޷�����.
			if (!InOverdriveFistCheck() && (_leftFistFormAura == nullptr))
			{
				_leftFistFormAura = DUOLClient::ParticleManager::GetInstance()->Pop(ParticleEnum::FistForm);

				_leftFistFormAura->GetTransform()->SetParent(_player->_playerLeftFistHolder->GetTransform(), false);

				_leftFistFormAura->GetTransform()->SetLocalPosition(DUOLMath::Vector3::Zero);

				_rightFistFormAura = DUOLClient::ParticleManager::GetInstance()->Pop(ParticleEnum::FistForm);

				_rightFistFormAura->GetTransform()->SetParent(_player->_playerRightFistHolder->GetTransform(), false);

				_rightFistFormAura->GetTransform()->SetLocalPosition(DUOLMath::Vector3::Zero);
			}

			_player->_playerWeaponSword->HouseSword();
		}
	}

	void PlayerState_Attack::EndAttack()
	{
		if (!_isOnStay)
			return;

		// ������ ������ Idle �� �Ѿ��.
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
			// ĵ�� Ÿ�� ���߿� �Է��� �Ǿ��ٸ�. ����, ĵ�� ������ �� �Է� �� ���� �ް� ������ (���� �����̶��� ���� �������� ..) ���� �߰��� ��
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

		// TODO : �ĵ� ����
		(_currentComboTreeNode->GetLeftNode() == nullptr && _currentComboTreeNode->GetRightNode() == nullptr)
			? _player->StartCoroutine(routine, _player->_endAttackPostDelay)
			: _player->StartCoroutine(routine, _player->_inAttackPostDelay);

		_isInCancle = false;

		_currentComboTreeNode = nullptr;

		_nextComboTreeNode = nullptr;

		_animator->SetBool(TEXT("IsAttack"), false);
		_animator->SetBool(TEXT("IsSword"), false);
		_animator->SetBool(TEXT("IsFist"), false);
		_animator->SetFloat(TEXT("AnimationSpeed"), 1.f);
		_animator->SetInt(TEXT("AttackCount"), 0);

		// ��������̺� ���°� �ƴ� ��, ������ ������ ��� ���� �Ѽհ��� ����Ѵ�.
		if (!InOverdriveCheck())
		{
			_player->_playerWeaponSword->HoldSword();

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