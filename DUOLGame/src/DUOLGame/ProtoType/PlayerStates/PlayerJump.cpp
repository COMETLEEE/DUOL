#include "DUOLGame/ProtoType/PlayerStates/PlayerJump.h"

#include "DUOLGameEngine/Manager/InputManager.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"

#include "DUOLGame/ProtoType/Scripts/PlayerState.h"

#include "DUOLCommon/LogHelper.h"

namespace DUOLGame
{
	PlayerJump::PlayerJump() :
		ActionNode(_T("PlayerJump"))
	{

	}

	PlayerJump::~PlayerJump()
	{

	}

	NodeState PlayerJump::Tick()
	{
		if (InputManager::GetInstance()->GetKeyDown(KeyCode::Space) == true)
		{
			auto player = GetDataFromBlackBoard<GameObject*>(_T("Player"));
			auto jumpPower = GetDataFromBlackBoard<float>(_T("JumpPower"));

			player->GetComponent<Rigidbody>()->AddImpulse(DUOLMath::Vector3::Up * jumpPower);

			player->GetComponent<PlayerState>()->_isGround = false;

			DUOL_INFO("Player Jump.");

			return NodeState::SUCCESS;
		}

		return NodeState::FAILURE;
	}

	void PlayerJump::Stop()
	{

	}
}