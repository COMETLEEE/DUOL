#include "DUOLClient/Player/FSM/PlayerState_Die.h"

#include "DUOLClient/Manager/GameManager.h"
#include "DUOLClient/Manager/SystemManager.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"
#include "DUOLGameEngine/Util/Coroutine/WaitForSeconds.h"

namespace DUOLClient
{
	PlayerState_Die::PlayerState_Die(DUOLClient::Player* player) :
		PlayerStateBase(TEXT("PlayerState_Die"), player)
	{
#pragma region DIE_EVENT
		_player->AddEventFunction(TEXT("EndDieStart"), std::bind(&DUOLClient::PlayerState_Die::EndDieStart, this));
#pragma endregion
	}

	PlayerState_Die::~PlayerState_Die()
	{
	}

	void PlayerState_Die::EndDieStart()
	{
		if (!_isOnStay)
			return;

		_animator->SetBool(TEXT("IsDie"), false);

	}

	void PlayerState_Die::OnStateEnter(float deltaTime)
	{
		PlayerStateBase::OnStateEnter(deltaTime);

		_animator->SetBool(TEXT("IsDie"), true);
		std::function<DUOLGameEngine::CoroutineHandler(void)> routine = std::bind(&DUOLClient::PlayerState_Die::CallGameOver, this);
		_player->StartCoroutine(routine);
	}

	void PlayerState_Die::OnStateStay(float deltaTime)
	{
		PlayerStateBase::OnStateStay(deltaTime);
	}

	void PlayerState_Die::OnStateExit(float deltaTime)
	{
		PlayerStateBase::OnStateExit(deltaTime);
	}

	DUOLGameEngine::CoroutineHandler PlayerState_Die::CallGameOver()
	{
		SystemManager::GetInstance()->Die();
		co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(13.f);

		GameManager::GetInstance()->GameOverUIMode();
	}
}
