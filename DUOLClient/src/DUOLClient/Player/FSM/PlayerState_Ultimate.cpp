#include "DUOLClient/Player/FSM/PlayerState_Ultimate.h"

#include "DUOLClient/ECS/Component/ParticleData.h"
#include "DUOLClient/Player/Weapon_Sword.h"
#include "DUOLClient/Player/FSM/PlayerState_Idle.h"

#include "DUOLClient/Manager/ParticleManager.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"
#include "DUOLGameEngine/ECS/Component/ParticleRenderer.h"
#include "DUOLGameEngine/Manager/ResourceManager.h"

#include "DUOLGameEngine/Util/Coroutine/WaitForSeconds.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorState.h"

DUOLClient::PlayerState_Ultimate::PlayerState_Ultimate(DUOLClient::Player* player) :
	PlayerStateBase(TEXT("PlayerState_Ultimate"), player)
{
#pragma region Ultimate_EVENT
	//이벤트가 
	_player->AddEventFunction(TEXT("EndUltimate"),
		std::bind(&DUOLClient::PlayerState_Ultimate::EndUltimateAnimation, this));
#pragma endregion

#pragma region Ultimate_EFFECT_EVENT
	//이벤트가 
	_player->AddEventFunction(TEXT("StartUltimateSwordTrail"),
		std::bind(&DUOLClient::PlayerState_Ultimate::StartSwordTrail, this));

	_player->AddEventFunction(TEXT("EndUltimateSwordTrail"),
		std::bind(&DUOLClient::PlayerState_Ultimate::EndSwordTrail, this));

#pragma endregion

#pragma region ANIMATOR_CONTROLLER_CACHING
	_playerNormalAnimCon = DUOLGameEngine::ResourceManager::GetInstance()
		->GetAnimatorController(TEXT("Player_NormalAnimatorController"));

	_playerOverdriveSwordAnimCon = DUOLGameEngine::ResourceManager::GetInstance()
		->GetAnimatorController(TEXT("Player_Overdrive_SwordAnimatorController"));

	_playerOverdriveFistAnimCon = DUOLGameEngine::ResourceManager::GetInstance()
		->GetAnimatorController(TEXT("Player_Overdrive_FistAnimatorController"));
#pragma endregion
}

DUOLClient::PlayerState_Ultimate::~PlayerState_Ultimate()
{
}

void DUOLClient::PlayerState_Ultimate::RunUltimateAnimation()
{
	//궁극기가 가능한 시점은 ultimate 일때만입니다.

	_animator->SetBool(TEXT("IsUltimate"), true);
}

void DUOLClient::PlayerState_Ultimate::EndUltimateAnimation()
{
	_stateMachine->TransitionTo(TEXT("PlayerState_Idle"), 0.f);
}

void DUOLClient::PlayerState_Ultimate::StartSwordEffect()
{
}

void DUOLClient::PlayerState_Ultimate::EndSwordEffect()
{
}

void DUOLClient::PlayerState_Ultimate::StartSwordTrail()
{
	_overdriveSwordTrail = DUOLClient::ParticleManager::GetInstance()->Pop(ParticleEnum::OverdriveSwordTrail, 0.5f);

	if (_overdriveSwordTrail == nullptr)
		return;

	_overdriveSwordTrail->GetTransform()->SetParent(_player->_playerWeaponSword->GetTransform());

	auto transform = _overdriveSwordTrail->GetTransform();

	transform->SetLocalPosition(DUOLMath::Vector3(-0.029f, -0.612f, -0.01f));

	transform->SetLocalEulerAngle(DUOLMath::Vector3(0.f, 0.f, 90.f));

}

void DUOLClient::PlayerState_Ultimate::EndSwordTrail()
{
	if (_overdriveSwordTrail != nullptr)
	{
		_overdriveSwordTrail->GetTransform()->SetParent(nullptr);

		_overdriveSwordTrail->GetGameObject()->SetIsActiveSelf(false);

		_overdriveSwordTrail = nullptr;
	}
}

void DUOLClient::PlayerState_Ultimate::OnStateEnter(float deltaTime)
{
	PlayerStateBase::OnStateEnter(deltaTime);

	RunUltimateAnimation();
}

void DUOLClient::PlayerState_Ultimate::OnStateStay(float deltaTime)
{
	PlayerStateBase::OnStateStay(deltaTime);

	RunUltimateAnimation();

}

void DUOLClient::PlayerState_Ultimate::OnStateExit(float deltaTime)
{
	PlayerStateBase::OnStateExit(deltaTime);
	_animator->SetBool(TEXT("IsUltimate"), false);
}
