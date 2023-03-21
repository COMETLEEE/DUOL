#include "DUOLClient/Player/FSM/PlayerState_Attack.h"

#include "DUOLCommon/Log/LogHelper.h"
#include "DUOLGameEngine/ECS/Component/Transform.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"

namespace DUOLClient
{
	PlayerState_Attack::PlayerState_Attack() :
		PlayerStateBase(TEXT("PlayerState_Attack"))
	{
		_eventListenerIDs.push_back({ TEXT("SwordFirstCancleStart"), DUOLGameEngine::EventManager::GetInstance()->AddEventFunction(TEXT("SwordFirstCancleStart"), std::bind(&DUOLClient::PlayerState_Attack::StartCancleFrame, this))});
		_eventListenerIDs.push_back({ TEXT("SwordSecondCancleStart"), DUOLGameEngine::EventManager::GetInstance()->AddEventFunction(TEXT("SwordSecondCancleStart"), std::bind(&DUOLClient::PlayerState_Attack::StartCancleFrame, this))});
		_eventListenerIDs.push_back({ TEXT("SwordThirdCancleStart"), DUOLGameEngine::EventManager::GetInstance()->AddEventFunction(TEXT("SwordThirdCancleStart"), std::bind(&DUOLClient::PlayerState_Attack::StartCancleFrame, this)) } );

		_eventListenerIDs.push_back({ TEXT("SwordFirstCancleEnd"), DUOLGameEngine::EventManager::GetInstance()->AddEventFunction(TEXT("SwordFirstCancleEnd"), std::bind(&DUOLClient::PlayerState_Attack::EndCancleFrame, this)) });
		_eventListenerIDs.push_back({ TEXT("SwordSecondCancleEnd"), DUOLGameEngine::EventManager::GetInstance()->AddEventFunction(TEXT("SwordSecondCancleEnd"), std::bind(&DUOLClient::PlayerState_Attack::EndCancleFrame, this)) });
		_eventListenerIDs.push_back({ TEXT("SwordThirdCancleEnd"), DUOLGameEngine::EventManager::GetInstance()->AddEventFunction(TEXT("SwordThirdCancleEnd"), std::bind(&DUOLClient::PlayerState_Attack::EndCancleFrame, this)) });

		_eventListenerIDs.push_back({ TEXT("SwordBasicComboEnd"), DUOLGameEngine::EventManager::GetInstance()->AddEventFunction(TEXT("SwordBasicComboEnd"), std::bind(&DUOLClient::PlayerState_Attack::EndAttack, this)) });
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

		if (_isAttackCheckedInCancle)
		{
			// TODO : �ϴ� ��Ʈ����� �����غ���.
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

		if (DieCheck())
		{
			_stateMachine->TransitionTo(TEXT("PlayerState_Die"), deltaTime);
		}
		else if (AttackCheck())
		{
			// ĵ�� Ÿ�� ���߿� �Է��� �Ǿ��ٸ�
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