#include "DUOLClient/Player/FSM/PlayerState_Attack.h"

#include "DUOLClient/ECS/Component/AI_Enemy.h"
#include "DUOLGameEngine/Manager/PhysicsManager.h"

#include "DUOLCommon/Log/LogHelper.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Transform.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"


namespace DUOLClient
{
	PlayerState_Attack::PlayerState_Attack() :
		PlayerStateBase(TEXT("PlayerState_Attack"))
	{
#pragma region SWORD_CANCLE_FRAME
		_eventListenerIDs.push_back({ TEXT("SwordFirstCancleStart"), 
			DUOLGameEngine::EventManager::GetInstance()->AddEventFunction(TEXT("SwordFirstCancleStart"), std::bind(&DUOLClient::PlayerState_Attack::StartCancleFrame, this))});
		_eventListenerIDs.push_back({ TEXT("SwordSecondCancleStart"), 
			DUOLGameEngine::EventManager::GetInstance()->AddEventFunction(TEXT("SwordSecondCancleStart"), std::bind(&DUOLClient::PlayerState_Attack::StartCancleFrame, this))});
		_eventListenerIDs.push_back({ TEXT("SwordThirdCancleStart"), 
			DUOLGameEngine::EventManager::GetInstance()->AddEventFunction(TEXT("SwordThirdCancleStart"), std::bind(&DUOLClient::PlayerState_Attack::StartCancleFrame, this)) } );

		_eventListenerIDs.push_back({ TEXT("SwordFirstCancleEnd"), 
			DUOLGameEngine::EventManager::GetInstance()->AddEventFunction(TEXT("SwordFirstCancleEnd"), std::bind(&DUOLClient::PlayerState_Attack::EndCancleFrame, this)) });
		_eventListenerIDs.push_back({ TEXT("SwordSecondCancleEnd"), 
			DUOLGameEngine::EventManager::GetInstance()->AddEventFunction(TEXT("SwordSecondCancleEnd"), std::bind(&DUOLClient::PlayerState_Attack::EndCancleFrame, this)) });
		_eventListenerIDs.push_back({ TEXT("SwordThirdCancleEnd"),
			DUOLGameEngine::EventManager::GetInstance()->AddEventFunction(TEXT("SwordThirdCancleEnd"), std::bind(&DUOLClient::PlayerState_Attack::EndCancleFrame, this)) });

		_eventListenerIDs.push_back({ TEXT("SwordBasicComboEnd"), 
			DUOLGameEngine::EventManager::GetInstance()->AddEventFunction(TEXT("SwordBasicComboEnd"), std::bind(&DUOLClient::PlayerState_Attack::EndAttack, this)) });
#pragma endregion

#pragma region SWORD_HIT
		_eventListenerIDs.push_back({ TEXT("SwordFirstHit"),
			DUOLGameEngine::EventManager::GetInstance()->AddEventFunction(TEXT("SwordFirstHit"), std::bind(&DUOLClient::PlayerState_Attack::SwordFirstHitFrame, this)) });

		_eventListenerIDs.push_back({ TEXT("SwordSecondHit"),
			DUOLGameEngine::EventManager::GetInstance()->AddEventFunction(TEXT("SwordSecondHit"), std::bind(&DUOLClient::PlayerState_Attack::SwordSecondHitFrame, this)) });

		_eventListenerIDs.push_back({ TEXT("SwordThirdHit"),
			DUOLGameEngine::EventManager::GetInstance()->AddEventFunction(TEXT("SwordThirdHit"), std::bind(&DUOLClient::PlayerState_Attack::SwordThirdHitFrame, this)) });

		_eventListenerIDs.push_back({ TEXT("SwordFourthHit"),
			DUOLGameEngine::EventManager::GetInstance()->AddEventFunction(TEXT("SwordFourthHit"), std::bind(&DUOLClient::PlayerState_Attack::SwordFourthHitFrame, this)) });
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

	void PlayerState_Attack::SwordFirstHitFrame()
	{
		std::vector<DUOLPhysics::RaycastHit> boxOverlapHits;

		const DUOLMath::Vector3& playerPos = _transform->GetWorldPosition();

		const DUOLMath::Vector3& playerLook = _transform->GetLook();

		const DUOLMath::Quaternion boxRotation = DUOLMath::Quaternion::Identity;

		if (DUOLGameEngine::PhysicsManager::GetInstance()->OverlapBoxAll(playerPos + playerLook * 3, SWORD_HIT_BOX, boxRotation, boxOverlapHits))
		{
			for (auto hited : boxOverlapHits)
			{
				DUOLGameEngine::GameObject* gameObject = reinterpret_cast<DUOLGameEngine::GameObject*>(hited._userData);

				// 적군입니다. 맞았씁니다.
				if (gameObject->GetTag() == TEXT("Enemy"))
				{
					auto aiEnemy = gameObject->GetComponent<DUOLClient::AI_Enemy>();

					aiEnemy->SetIsHit(true);
				}
			}
		}
	}

	void PlayerState_Attack::SwordSecondHitFrame()
	{
		std::vector<DUOLPhysics::RaycastHit> boxOverlapHits;

		const DUOLMath::Vector3& playerPos = _transform->GetWorldPosition();

		const DUOLMath::Vector3& playerLook = _transform->GetLook();

		const DUOLMath::Quaternion boxRotation = DUOLMath::Quaternion::Identity;

		if (DUOLGameEngine::PhysicsManager::GetInstance()->OverlapBoxAll(playerPos + playerLook * 3, SWORD_HIT_BOX, boxRotation, boxOverlapHits))
		{
			for (auto hited : boxOverlapHits)
			{
				DUOLGameEngine::GameObject* gameObject = reinterpret_cast<DUOLGameEngine::GameObject*>(hited._userData);

				// 적군입니다. 맞았씁니다.
				if (gameObject->GetTag() == TEXT("Enemy"))
				{
					auto aiEnemy = gameObject->GetComponent<DUOLClient::AI_Enemy>();

					aiEnemy->SetIsHit(true);
				}
			}
		}
	}

	void PlayerState_Attack::SwordThirdHitFrame()
	{
		std::vector<DUOLPhysics::RaycastHit> boxOverlapHits;

		const DUOLMath::Vector3& playerPos = _transform->GetWorldPosition();

		const DUOLMath::Vector3& playerLook = _transform->GetLook();

		const DUOLMath::Quaternion boxRotation = DUOLMath::Quaternion::Identity;

		if (DUOLGameEngine::PhysicsManager::GetInstance()->OverlapBoxAll(playerPos + playerLook * 3, SWORD_HIT_BOX, boxRotation, boxOverlapHits))
		{
			for (auto hited : boxOverlapHits)
			{
				DUOLGameEngine::GameObject* gameObject = reinterpret_cast<DUOLGameEngine::GameObject*>(hited._userData);

				// 적군입니다. 맞았씁니다.
				if (gameObject->GetTag() == TEXT("Enemy"))
				{
					auto aiEnemy = gameObject->GetComponent<DUOLClient::AI_Enemy>();

					aiEnemy->SetIsHit(true);
				}
			}
		}
	}

	void PlayerState_Attack::SwordFourthHitFrame()
	{
		std::vector<DUOLPhysics::RaycastHit> boxcastHits;

		const DUOLMath::Vector3& playerPos = _transform->GetWorldPosition();

		const DUOLMath::Vector3& playerLook = _transform->GetLook();

		const DUOLMath::Quaternion boxRotation = DUOLMath::Quaternion::Identity;

		if (DUOLGameEngine::PhysicsManager::GetInstance()->BoxcastAll(playerPos + playerLook * 3, playerLook, SWORD_FOURTH_HIT_BOX, boxRotation, SWORD_FOURTH_HIT_RANGE, boxcastHits))
		{
			for (auto hited : boxcastHits)
			{
				DUOLGameEngine::GameObject* gameObject = reinterpret_cast<DUOLGameEngine::GameObject*>(hited._userData);

				// 적군입니다. 맞았씁니다.
				if (gameObject->GetTag() == TEXT("Enemy"))
				{
					auto aiEnemy = gameObject->GetComponent<DUOLClient::AI_Enemy>();

					aiEnemy->SetIsHit(true);
				}
			}
		}
	}

	void PlayerState_Attack::EndCancleFrame()
	{
		if (!_isOnStay)
			return;

		DUOL_INFO(DUOL_CONSOLE, "End cancel frame.");

		_isInCancle = false;

		if (_isAttackCheckedInCancle)
		{
			// TODO : 일단 루트모션을 적용해보자.
			// _transform->LookAt(_transform->GetWorldPosition() + _desiredLook);

			return;
		}
		else
			_stateMachine->TransitionTo(TEXT("PlayerState_Idle"), 0.f);
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
		else if (AttackCheck())
		{
			// 캔슬 타임 도중에 입력이 되었다면
			if (_isInCancle)
			{
				_isAttackCheckedInCancle = true;
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
