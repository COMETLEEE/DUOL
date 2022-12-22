#include "DUOLGame/ProtoType/PlayerStates/PlayerAttack.h"

#include "DUOLGameEngine/Manager/InputManager.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"

#include "DUOLCommon/LogHelper.h"

namespace DUOLGame
{
	PlayerAttack::PlayerAttack() :
		ActionNode(_T("PlayerAttack"))
	{

	}

	PlayerAttack::~PlayerAttack()
	{

	}

	NodeState PlayerAttack::Tick()
	{
		if (InputManager::GetInstance()->GetMouseButtonDown(MouseCode::Left) == true)
		{
			auto player = GetDataFromBlackBoard<GameObject*>(_T("Player"));

			player->GetComponent<Rigidbody>()->SetLinearVelocity(DUOLMath::Vector3::Zero);

			DUOL_INFO("Player Attack");

			return NodeState::SUCCESS;
		}

		return NodeState::FAILURE;
	}

	void PlayerAttack::Stop()
	{

	}
}