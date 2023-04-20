#include "DUOLClient/Player/FSM/PlayerState_Overdrive.h"

#include "DUOLGameEngine/ECS/Component/Animator.h"
#include "DUOLGameEngine/Manager/ResourceManager.h"

namespace DUOLClient
{
	PlayerState_Overdrive::PlayerState_Overdrive(DUOLClient::Player* player) :
		PlayerStateBase(TEXT("PlayerState_Overdrive"), player)
		, _isEnter(false)
		, _isSword(false)
	{
#pragma region OVERDRIVE_ENTER_EVENT
		_player->AddEventFunction(TEXT("EndOverdriveEnter"),
			std::bind(&DUOLClient::PlayerState_Overdrive::EndOverdriveEnter, this));
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

	PlayerState_Overdrive::~PlayerState_Overdrive()
	{
	}

	void PlayerState_Overdrive::EnterOverdriveSword()
	{
		_isEnter = true;

		_isSword = true;

		_animator->SetBool(TEXT("IsOverdriveSwordEnter"), true);
	}

	void PlayerState_Overdrive::EnterOverdriveFist()
	{
		_isEnter = true;

		_isSword = false;

		_animator->SetBool(TEXT("IsOverdriveFistEnter"), true);
	}

	void PlayerState_Overdrive::ExitOverdriveSword()
	{
		_isEnter = false;
	}

	void PlayerState_Overdrive::ExitOverdriveFist()
	{
		_isEnter = false;
	}

	void PlayerState_Overdrive::EndOverdriveEnter()
	{
		_isSword
			? _animator->SetAnimatorController(_playerOverdriveSwordAnimCon)
			: _animator->SetAnimatorController(_playerOverdriveFistAnimCon);

		_isSword ? _player->_isOverdriveSwordMode = true : _player->_isOverdriveFistMode = true;

		_player->_currentOverdrivePoint = 0.f;

		_stateMachine->TransitionTo(TEXT("PlayerState_Idle"), 0.f);
	}

	void PlayerState_Overdrive::OnStateEnter(float deltaTime)
	{
		PlayerStateBase::OnStateEnter(deltaTime);

		// 카메라 연출 준비 ?
	}

	void PlayerState_Overdrive::OnStateStay(float deltaTime)
	{
		PlayerStateBase::OnStateStay(deltaTime);
	}

	void PlayerState_Overdrive::OnStateExit(float deltaTime)
	{
		PlayerStateBase::OnStateExit(deltaTime);

		_isEnter = false;

		_isSword = false;
	}
}