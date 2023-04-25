#include "DUOLClient/Player/FSM/PlayerState_Overdrive.h"

#include "DUOLClient/Manager/ParticleManager.h"
#include "DUOLClient/Player/Weapon_Sword.h"
#include "DUOLClient/Player/FSM/PlayerState_Idle.h"

#include "DUOLGameEngine/ECS/Component/Animator.h"
#include "DUOLGameEngine/ECS/Component/ParticleRenderer.h"
#include "DUOLGameEngine/Manager/ResourceManager.h"
#include "DUOLGameEngine/Util/Coroutine/WaitForSeconds.h"

namespace DUOLClient
{
	PlayerState_Overdrive::PlayerState_Overdrive(DUOLClient::Player* player) :
		PlayerStateBase(TEXT("PlayerState_Overdrive"), player)
		, _isEnter(false)
		, _isSword(false)
	{
#pragma region OVERDRIVE_EVENT
		_player->AddEventFunction(TEXT("EquipOverdriveSwordWeapon"),
			std::bind(&DUOLClient::PlayerState_Overdrive::EquipOverdriveWeaponSword, this));

		_player->AddEventFunction(TEXT("EndOverdriveEnter"),
			std::bind(&DUOLClient::PlayerState_Overdrive::EndOverdriveEnter, this));

		_player->AddEventFunction(TEXT("UnequipOverdriveWeaponSword"),
			std::bind(&DUOLClient::PlayerState_Overdrive::UnequipOverdriveWeaponSword, this));

		_player->AddEventFunction(TEXT("EndOverdriveExit"),
			std::bind(&DUOLClient::PlayerState_Overdrive::EndOverdriveExit, this));
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

		_particleOverdriveEnter = DUOLClient::ParticleManager::GetInstance()->Pop(ParticleEnum::OverdriveEnter);

		_particleOverdriveEnter->GetTransform()->SetPosition(_transform->GetWorldPosition());
	}

	void PlayerState_Overdrive::EnterOverdriveFist()
	{
		_isEnter = true;

		_isSword = false;

		_animator->SetBool(TEXT("IsOverdriveFistEnter"), true);

		_player->_playerWeaponSword->HouseSword();

		_particleOverdriveEnter = DUOLClient::ParticleManager::GetInstance()->Pop(ParticleEnum::OverdriveEnter);

		_particleOverdriveEnter->GetTransform()->SetPosition(_transform->GetWorldPosition());
	}

	void PlayerState_Overdrive::ExitOverdrive()
	{
		_isEnter = false;

		_animator->SetBool(TEXT("IsOverdriveExit"), true);
	}

	void PlayerState_Overdrive::EquipOverdriveWeaponSword()
	{
		// TODO : 진짜 대검으로 변경
		_player->_playerOverdriveWeaponSword->HoldSword();
	}

	void PlayerState_Overdrive::UnequipOverdriveWeaponSword()
	{
		// TODO : 진짜 대검으로 변경
		_player->_playerOverdriveWeaponSword->HouseSword();
	}

	void PlayerState_Overdrive::EndOverdriveEnter()
	{
		_stateMachine->TransitionTo(TEXT("PlayerState_Idle"), 0.f);
	}

	void PlayerState_Overdrive::EndOverdriveExit()
	{
		_stateMachine->TransitionTo(TEXT("PlayerState_Idle"), 0.f);
	}

	DUOLGameEngine::CoroutineHandler PlayerState_Overdrive::ReserveEndOverdriveState()
	{
		co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(OVERDRIVE_DURATION);

		// 오버드라이브가 끝날 수 있음을 알린다.
		auto idleState = reinterpret_cast<DUOLClient::PlayerState_Idle*>(_stateMachine->GetState(TEXT("PlayerState_Idle")));

		idleState->ReserveEndOverdrive();
	}

	void PlayerState_Overdrive::OnStateEnter(float deltaTime)
	{
		PlayerStateBase::OnStateEnter(deltaTime);

		// TODO : 카메라 연출 준비 ?
	}

	void PlayerState_Overdrive::OnStateStay(float deltaTime)
	{
		PlayerStateBase::OnStateStay(deltaTime);

		// TODO : 카메라 연출 중 .. 
	}

	void PlayerState_Overdrive::OnStateExit(float deltaTime)
	{
		PlayerStateBase::OnStateExit(deltaTime);

		if (_isEnter)
		{
			if (_isSword)
			{
				_animator->SetAnimatorController(_playerOverdriveSwordAnimCon);

				_player->_isOverdriveSwordMode = true;
			}
			else
			{
				_animator->SetAnimatorController(_playerOverdriveFistAnimCon);

				_player->_isOverdriveFistMode = true;
			}

			std::function<DUOLGameEngine::CoroutineHandler(void)> reserveFunc = std::bind(&DUOLClient::PlayerState_Overdrive::ReserveEndOverdriveState, this);

			_player->StartCoroutine(reserveFunc);

			_animator->SetBool(TEXT("IsOverdriveSwordEnter"), false);

			_animator->SetBool(TEXT("IsOverdriveFistEnter"), false);

			if (_particleOverdriveEnter != nullptr)
			{
				_particleOverdriveEnter->Stop();

				_particleOverdriveEnter = nullptr;
			}
		}
		else
		{
			_animator->SetAnimatorController(_playerNormalAnimCon);

			_player->_isOverdriveSwordMode = false;

			_player->_isOverdriveFistMode = false;

			_animator->SetBool(TEXT("IsOverdriveExit"), false);
		}

		_isEnter = false;

		_isSword = false;

		_player->_currentOverdrivePoint = 0.f;
	}
}