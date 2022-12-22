#include "DUOLGame/ProtoType/PlayerStates/PlayerMove.h"

#include "DUOLGameEngine/Manager/InputManager.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"


#include "DUOLCommon/LogHelper.h"

namespace DUOLGame
{
	PlayerMove::PlayerMove() :
		ActionNode(_T("PlayerMove"))
	{

	}

	PlayerMove::~PlayerMove()
	{

	}

	NodeState PlayerMove::Tick()
	{
		auto player = GetDataFromBlackBoard<GameObject*>(_T("Player"));

		DUOLMath::Vector3 force = DUOLMath::Vector3::Zero;
		DUOLMath::Vector3 beforeVelocity = player->GetComponent<Rigidbody>()->GetLinearVelocity();

		if (InputManager::GetInstance()->GetKeyPressed(KeyCode::W) == true)
			force += player->GetComponent<Transform>()->GetLook();

		if (InputManager::GetInstance()->GetKeyPressed(KeyCode::S) == true)
			force -= player->GetComponent<Transform>()->GetLook();

		if (InputManager::GetInstance()->GetKeyPressed(KeyCode::A) == true)
			force -= player->GetComponent<Transform>()->GetRight();

		if (InputManager::GetInstance()->GetKeyPressed(KeyCode::D) == true)
			force += player->GetComponent<Transform>()->GetRight();

		if (force == DUOLMath::Vector3::Zero)
		{
			player->GetComponent<Rigidbody>()->SetLinearVelocity({ 0.0f, beforeVelocity.y, 0.0f });

			return NodeState::FAILURE;
		}

		auto deltaTime = GetDataFromBlackBoard<float>(_T("DeltaTime"));
		auto moveSpeed = GetDataFromBlackBoard<float>(_T("MoveSpeed"));

		player->GetComponent<Rigidbody>()->SetLinearVelocity(force * moveSpeed);

		static float coolTime = 0.0f;

		if (coolTime <= 0.0f)
		{
			coolTime = 1.0f;

			DUOL_INFO("Player Move : {0}, {1}, {2}", force.x * moveSpeed, force.y * moveSpeed, force.z * moveSpeed);
		}
		else
		{
			coolTime -= deltaTime;
		}

		return NodeState::SUCCESS;
	}

	void PlayerMove::Stop()
	{

	}
}