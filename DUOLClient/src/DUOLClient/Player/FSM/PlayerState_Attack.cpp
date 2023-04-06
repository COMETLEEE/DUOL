#include "DUOLClient/Player/FSM/PlayerState_Attack.h"

#include "DUOLClient/Camera/MainCameraController.h"
#include "DUOLClient/ECS/Component/Enemy/AI_EnemyBasic.h"
#include "DUOLGameEngine/Manager/PhysicsManager.h"

#include "DUOLCommon/Log/LogHelper.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Transform.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"

#include "DUOLClient/Player/Weapon_Sword.h"

namespace DUOLClient
{
	PlayerState_Attack::PlayerState_Attack(DUOLClient::Player* player) :
		PlayerStateBase(TEXT("PlayerState_Attack"), player)
	{
#pragma region SWORD_CANCLE_FRAME
		_player->AddEventFunction(TEXT("SwordFirstCancleStart"), std::bind(&DUOLClient::PlayerState_Attack::StartCancleFrame, this));
		_player->AddEventFunction(TEXT("SwordSecondCancleStart"), std::bind(&DUOLClient::PlayerState_Attack::StartCancleFrame, this));
		_player->AddEventFunction(TEXT("SwordThirdCancleStart"), std::bind(&DUOLClient::PlayerState_Attack::StartCancleFrame, this));

		_player->AddEventFunction(TEXT("SwordFirstCancleEnd"), std::bind(&DUOLClient::PlayerState_Attack::EndCancleFrame, this));
		_player->AddEventFunction(TEXT("SwordSecondCancleEnd"), std::bind(&DUOLClient::PlayerState_Attack::EndCancleFrame, this));
		_player->AddEventFunction(TEXT("SwordThirdCancleEnd"), std::bind(&DUOLClient::PlayerState_Attack::EndCancleFrame, this));

		_player->AddEventFunction(TEXT("SwordBasicComboEnd"), std::bind(&DUOLClient::PlayerState_Attack::EndAttack, this));
#pragma endregion

#pragma region SWORD_HIT
		_player->AddEventFunction(TEXT("SwordFourthHit"), std::bind(&DUOLClient::PlayerState_Attack::SwordFourthHitFrame, this));
#pragma endregion
	}

	PlayerState_Attack::~PlayerState_Attack()
	{
		for (auto pair : _eventListenerIDs)
		{
			DUOLGameEngine::EventManager::GetInstance()->RemoveEventFunction<void>(pair.first, pair.second);
		}
	}

	void PlayerState_Attack::StartCancleFrame()
	{
		if (!_isOnStay)
			return;

		DUOL_INFO(DUOL_CONSOLE, "Start cancel frame.");

		_isInCancle = true;

		_isAttackCheckedInCancle = false;
	}

	void PlayerState_Attack::EndCancleFrame()
	{
		if (!_isOnStay)
			return;

		DUOL_INFO(DUOL_CONSOLE, "End cancel frame.");

		_isInCancle = false;

		// 캔슬 프레임으로 넘어간다.
		if (_isAttackCheckedInCancle)
		{
			if (_nextSwordAttack)
			{
				// 소드 어택. 테이블 참조
			}
			else if (_nextPunchAttack)
			{
				// 펀치 어택
			}
		}
		// 입력이 없었으면 .. 캔슬 프레임이 끝날 때 Idle 스테이트로 넘어갑니다.
		// TODO : 각각 공격별 애니메이션으로 바뀌면 .. 없어진다 ..!
		else
		{
			_stateMachine->TransitionTo(TEXT("PlayerState_Idle"), 0.f);
		}
	}

	void PlayerState_Attack::SwordFourthHitFrame()
	{
		if (!_isOnStay)
			return;

		std::vector<DUOLPhysics::RaycastHit> boxcastHits;

		const DUOLMath::Vector3& playerPos = _transform->GetWorldPosition();

		const DUOLMath::Vector3& playerLook = _transform->GetLook();

		const DUOLMath::Quaternion boxRotation = DUOLMath::Quaternion::Identity;

		// 충격파식 구현
		if (DUOLGameEngine::PhysicsManager::GetInstance()->BoxcastAll(playerPos + playerLook * 3, playerLook, SWORD_FOURTH_HIT_BOX, boxRotation, SWORD_FOURTH_HIT_RANGE, boxcastHits))
		{
			for (auto hited : boxcastHits)
			{
				DUOLGameEngine::GameObject* gameObject = reinterpret_cast<DUOLGameEngine::GameObject*>(hited._userData);

				// 적군입니다. 맞았씁니다.
				if (gameObject->GetTag() == TEXT("Enemy"))
				{
					auto aiEnemy = gameObject->GetComponent<DUOLClient::CharacterBase>();

					_player->Attack(aiEnemy, _player->_currentDamage + 10.f);
				}
			}
		}

		// Shake
		_mainCamController->SetCameraShake(0.5f, DUOLMath::Vector2(4.f, 4.f));
	}

	void PlayerState_Attack::EndAnimation()
	{
		if (!_isOnStay)
			return;

		// 캔슬 프레임 구간에서 공격 커맨드가 내려오지 않았다면 전환 ..!
		if (!_isAttackCheckedInCancle)
			EndAttack();

		// 소드 어택에 들어갑시다.
		if (_nextSwordAttack)
		{
			
		}
		else if (_nextPunchAttack)
		{
			
		}
	}

	void PlayerState_Attack::SwordAttackStart()
	{
		// 무기 On
		_player->_playerWeaponSword->SetIsEnabled(true);
	}

	void PlayerState_Attack::SwordAttackEnd()
	{
		// 무기 Off
		_player->_playerWeaponSword->SetIsEnabled(false);
	}

	void PlayerState_Attack::EndAttack()
	{
		if (!_isOnStay)
			return;

		DUOL_INFO(DUOL_CONSOLE, "End Attack");

		_stateMachine->TransitionTo(TEXT("PlayerState_Idle"), 0.f);
	}

	void PlayerState_Attack::OnStateEnter(float deltaTime)
	{
		PlayerStateBase::OnStateEnter(deltaTime);

		_animator->SetBool(TEXT("IsAttack"), true);
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
		else if (LeftAttackCheck())
		{
			// 캔슬 타임 도중에 입력이 되었다면
			if (_isInCancle)
			{
				_isAttackCheckedInCancle = true;			// 좌, 우 클릭 구분 ?
			}
		}
		else if (RightAttackCheck())
		{
			if (_isInCancle)
			{
				_isAttackCheckedInCancle = true;

				_nextPunchAttack = true;
			}
		}
	}

	void PlayerState_Attack::OnStateExit(float deltaTime)
	{
		PlayerStateBase::OnStateExit(deltaTime);

		_isInCancle = false;

		_isAttackCheckedInCancle = false;

		_animator->SetBool(TEXT("IsAttack"), false);
	}
}