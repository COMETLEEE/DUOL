#include "DUOLGame/ProtoType/PlayerStates/PlayerIdle.h"

#include "DUOLGameEngine/Manager/ResourceManager.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"

#include "DUOLCommon/LogHelper.h"

namespace DUOLGame
{
	PlayerIdle::PlayerIdle() :
		ActionNode(_T("PlayerIdle"))
	{

	}

	PlayerIdle::~PlayerIdle()
	{

	}

	NodeState PlayerIdle::Tick()
	{
		auto deltaTime = GetDataFromBlackBoard<float>(_T("DeltaTime"));

		auto animator = GetDataFromBlackBoard<GameObject*>(_T("Player"))->GetComponent<Animator>();

		static float coolTime = 0.0f;

		if (coolTime <= 0.0f)
		{
			coolTime = 1.0f;

			DUOL_INFO("Player Idle");
		}
		else
		{
			coolTime -= deltaTime;
		}

		animator->SetAnimationClip(ResourceManager::GetInstance()->GetAnimationClip(TEXT("Idle")));

		return NodeState::SUCCESS;
	}

	void PlayerIdle::Stop()
	{

	}
}