#include "DUOLClient/Player/FSM/PlayerState_Attack.h"

#include "DUOLClient/Camera/MainCameraController.h"
#include "DUOLClient/ECS/Component/Enemy/AI_EnemyBasic.h"
#include "DUOLGameEngine/Manager/PhysicsManager.h"

#include "DUOLCommon/Log/LogHelper.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Transform.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"

#include "DUOLClient/Player/Weapon_Sword.h"
#include "DUOLGameEngine/Util/Coroutine/WaitForSeconds.h"

namespace DUOLClient
{
	PlayerState_Attack::PlayerState_Attack(DUOLClient::Player* player) :
		PlayerStateBase(TEXT("PlayerState_Attack"), player)
		, _currentComboTreeNode(nullptr)
	{
		// ĵ�� ������ ���� �̺�Ʈ �Լ� ���
#pragma region CANCLE_FRAME_EVENT
		_player->AddEventFunction(TEXT("SwordFirstCancleStart"), std::bind(&DUOLClient::PlayerState_Attack::StartCancleFrame, this));
		_player->AddEventFunction(TEXT("SwordSecondCancleStart"), std::bind(&DUOLClient::PlayerState_Attack::StartCancleFrame, this));
		_player->AddEventFunction(TEXT("SwordThirdCancleStart"), std::bind(&DUOLClient::PlayerState_Attack::StartCancleFrame, this));

		_player->AddEventFunction(TEXT("SwordFirstCancleEnd"), std::bind(&DUOLClient::PlayerState_Attack::EndCancleFrame, this));
		_player->AddEventFunction(TEXT("SwordSecondCancleEnd"), std::bind(&DUOLClient::PlayerState_Attack::EndCancleFrame, this));
		_player->AddEventFunction(TEXT("SwordThirdCancleEnd"), std::bind(&DUOLClient::PlayerState_Attack::EndCancleFrame, this));

		_player->AddEventFunction(TEXT("SwordBasicComboEnd"), std::bind(&DUOLClient::PlayerState_Attack::EndAttack, this));
#pragma endregion

		// ���� ���� (�ִϸ��̼� ��ȯ) ������ �̺�Ʈ �Լ� ���
#pragma region END_FRAME_EVENT

#pragma endregion

		// Ÿ�� ������ �̺�Ʈ �Լ� ���
#pragma region HIT_FRAME
		_player->AddEventFunction(TEXT("SwordFourthHit"), std::bind(&DUOLClient::PlayerState_Attack::SwordFourthHitFrame, this));
#pragma endregion

#pragma region NORMAL_COMBO_DATA_INITIALIZE
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

		DUOL_INFO(DUOL_CONSOLE, "Start cancel frame.");

		_isInCancle = true;

		_nextComboTreeNode = nullptr;
	}

	void PlayerState_Attack::EndCancleFrame()
	{
		if (!_isOnStay)
			return;

		DUOL_INFO(DUOL_CONSOLE, "End cancel frame.");

		_isInCancle = false;

		// TODO : ���� ������ �ȵ� .. ������ ������ �־��ֱ�� �ؾ��մϴ� ..!
		CheckCanEnterNextAttack();
	}

	void PlayerState_Attack::SwordFourthHitFrame()
	{
		if (!_isOnStay)
			return;

		std::vector<DUOLPhysics::RaycastHit> boxcastHits;

		const DUOLMath::Vector3& playerPos = _transform->GetWorldPosition();

		const DUOLMath::Vector3& playerLook = _transform->GetLook();

		const DUOLMath::Quaternion boxRotation = DUOLMath::Quaternion::Identity;

		// ����Ľ� �������� ����
		if (DUOLGameEngine::PhysicsManager::GetInstance()->BoxcastAll(playerPos + playerLook * 3, playerLook, SWORD_FOURTH_HIT_BOX, boxRotation, SWORD_FOURTH_HIT_RANGE, boxcastHits))
		{
			for (auto hited : boxcastHits)
			{
				DUOLGameEngine::GameObject* gameObject = reinterpret_cast<DUOLGameEngine::GameObject*>(hited._userData);

				// �����Դϴ�. �¾Ҿ��ϴ�.
				if (gameObject->GetTag() == TEXT("Enemy"))
				{
					auto aiEnemy = gameObject->GetComponent<DUOLClient::CharacterBase>();

					_player->Attack(aiEnemy, _player->_currentDamage + 10.f, AttackType::HeavyAttack);
				}
			}
		}

		// Shake
		_mainCamController->SetCameraShake(0.5f, DUOLMath::Vector2(4.f, 4.f));
	}

	DUOLGameEngine::CoroutineHandler PlayerState_Attack::SetPostDelay(float delayTime)
	{
		_player->_canStartAttack = false;

		co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(delayTime);

		_player->_canStartAttack = true;
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

		_swordComboTree = BinaryTree<Player_NormalAttack>({Player_AttackType::SWORD, animatorParameterTable});

		// 2Ÿ
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

		auto swordCombo1_2 = _swordComboTree.AddRightNode({ Player_AttackType::FIST, animatorParameterTable });

		// 3Ÿ
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

		auto swordCombo2_3 = swordSecond->AddRightNode({ Player_AttackType::FIST, animatorParameterTable });

		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 3 });

		auto swordCombo1_3 = swordCombo1_2->AddRightNode({ Player_AttackType::FIST, animatorParameterTable });

		// 4Ÿ
		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 4 });

		auto swordFourth = swordThird->AddLeftNode({ Player_AttackType::SWORD, animatorParameterTable });

		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 4 });

		auto swordCombo2_4 = swordCombo2_3->AddRightNode({ Player_AttackType::FIST, animatorParameterTable });

		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 4 });

		auto swordCombo3_4 = swordThird->AddRightNode({ Player_AttackType::FIST, animatorParameterTable });
#pragma endregion

#pragma region FIST_COMBO_TREE
		// 1Ÿ
		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 1 });

		_fistComboTree = BinaryTree<Player_NormalAttack>({ Player_AttackType::SWORD, animatorParameterTable });

		// 2Ÿ
		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 2 });

		auto fistSecond = _fistComboTree.AddRightNode({ Player_AttackType::FIST, animatorParameterTable });

		// 3Ÿ
		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 3 });

		auto fistThird = fistSecond->AddRightNode({ Player_AttackType::FIST, animatorParameterTable });

		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 3 });

		auto fistCombo2_3 = fistSecond->AddLeftNode({ Player_AttackType::SWORD, animatorParameterTable });

		// 4Ÿ
		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 4 });

		auto fistFourth = fistSecond->AddRightNode({ Player_AttackType::FIST, animatorParameterTable });

		animatorParameterTable.clear();

		animatorParameterTable.push_back({ TEXT("IsAttack"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsSword"), DUOLGameEngine::AnimatorControllerParameterType::Bool, true });
		animatorParameterTable.push_back({ TEXT("IsFist"), DUOLGameEngine::AnimatorControllerParameterType::Bool, false });
		animatorParameterTable.push_back({ TEXT("AttackCount"), DUOLGameEngine::AnimatorControllerParameterType::Int, 4 });

		auto fistCombo2_4 = fistCombo2_3->AddLeftNode({ Player_AttackType::SWORD, animatorParameterTable });
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
	}

	void PlayerState_Attack::EndAttack()
	{
		if (!_isOnStay)
			return;

		DUOL_INFO(DUOL_CONSOLE, "End Attack State ..");

		// ������ ������ Idle �� �Ѿ��.
		_stateMachine->TransitionTo(TEXT("PlayerState_Idle"), 0.f);
	}

	void PlayerState_Attack::OnStateEnter(float deltaTime)
	{
		PlayerStateBase::OnStateEnter(deltaTime);

		if (SwordAttackCheck())
			_currentComboTreeNode = &_swordComboTree;
		else if (FistAttackCheck())
			_currentComboTreeNode = &_fistComboTree;

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

		if (DieCheck())
		{
			_stateMachine->TransitionTo(TEXT("PlayerState_Die"), deltaTime);
		}
		else if (DashCheck())
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

		// �ĵ� ����
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
	}
}