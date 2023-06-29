#include "DUOLClient/Player/FSM/PlayerState_Overdrive.h"

#include "DUOLClient/Manager/ParticleManager.h"
#include "DUOLClient/Player/Weapon_Sword.h"
#include "DUOLClient/Player/FSM/PlayerState_Idle.h"

#include "DUOLGameEngine/ECS/Component/Animator.h"
#include "DUOLGameEngine/ECS/Component/ParticleRenderer.h"
#include "DUOLGameEngine/Manager/ResourceManager.h"
#include "DUOLGameEngine/Util/Coroutine/WaitForSeconds.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/AudioSource.h"
#include "DUOLGameEngine/ECS/Component/BoxCollider.h"
#include "DUOLGameEngine/Manager/TimeManager.h"

namespace DUOLClient
{
	PlayerState_Overdrive::PlayerState_Overdrive(DUOLClient::Player* player) :
		PlayerStateBase(TEXT("PlayerState_Overdrive"), player)
		, _isEnter(false)
		, _isSword(false)
	{
#pragma region OVERDRIVE_EVENT
		_player->AddEventFunction(TEXT("EquipOverdriveWeaponSword"),
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

		_particleOverdrive = DUOLClient::ParticleManager::GetInstance()->Pop(ParticleEnum::OverdriveEnter);

		// ����Ʈ �ڷ�ƾ
		std::function<DUOLGameEngine::CoroutineHandler(void)> routine
			= std::bind(&DUOLClient::PlayerState_Overdrive::UpdatePositionParticleOverdrive, this);

		_player->StartCoroutine(routine);
	}

	void PlayerState_Overdrive::EnterOverdriveFist()
	{
		_isEnter = true;

		_isSword = false;

		_animator->SetBool(TEXT("IsOverdriveFistEnter"), true);

		_player->_currentPlayerWeapon->HouseSword();

		_particleOverdrive = DUOLClient::ParticleManager::GetInstance()->Pop(ParticleEnum::OverdriveEnter);

		// ����Ʈ �ڷ�ƾ
		std::function<DUOLGameEngine::CoroutineHandler(void)> routine
			= std::bind(&DUOLClient::PlayerState_Overdrive::UpdatePositionParticleOverdrive, this);

		_player->StartCoroutine(routine);

		// �ָԽ�
		_leftOverdriveFistAura = DUOLClient::ParticleManager::GetInstance()->Pop(ParticleEnum::OverdriveFistForm);

		_rightOverdriveFistAura = DUOLClient::ParticleManager::GetInstance()->Pop(ParticleEnum::OverdriveFistForm);

		_leftOverdriveFistAura->GetTransform()->SetParent(_player->_playerLeftFistHolder->GetTransform(), false);

		_leftOverdriveFistAura->GetTransform()->SetLocalPosition(DUOLMath::Vector3::Zero);

		_rightOverdriveFistAura->GetTransform()->SetParent(_player->_playerRightFistHolder->GetTransform(), false);

		_rightOverdriveFistAura->GetTransform()->SetLocalPosition(DUOLMath::Vector3::Zero);

	}

	void PlayerState_Overdrive::ExitOverdrive()
	{
		_isEnter = false;

		_animator->SetBool(TEXT("ODExit"), true);

		DUOL_TRACE(DUOL_CONSOLE, "OverDrive | ExitOverdrive (AnimStart)");
	}

	void PlayerState_Overdrive::EquipOverdriveWeaponSword()
	{
		// TODO : ��¥ ������� ����
		_player->_playerOverdriveWeaponSwordCollider->SetIsEnabled(false);
		_player->_playerWeaponSwordCollider->SetIsEnabled(false);
		_player->_playerWeaponSword->OffSword();
		_player->_playerOverdriveWeaponSword->OnSword();

		_player->_playerWeaponSword->HouseSword();
		_player->_playerOverdriveWeaponSword->HoldSword();

		_player->_currentPlayerWeapon = _player->_playerOverdriveWeaponSword;
		_player->_currentplayerWeaponSwordCollider = _player->_playerOverdriveWeaponSwordCollider;

	}

	void PlayerState_Overdrive::UnequipOverdriveWeaponSword()
	{
		// TODO : ��¥ ������� ����
		_player->_playerOverdriveWeaponSwordCollider->SetIsEnabled(false);
		_player->_playerWeaponSwordCollider->SetIsEnabled(false);
		_player->_playerWeaponSword->OffSword();
		_player->_playerOverdriveWeaponSword->OnSword();

		_player->_playerWeaponSword->HouseSword();
		_player->_playerOverdriveWeaponSword->HouseSword();

		_player->_currentPlayerWeapon = _player->_playerWeaponSword;
		_player->_currentplayerWeaponSwordCollider = _player->_playerWeaponSwordCollider;

	}

	void PlayerState_Overdrive::EndOverdriveEnter()
	{
		_stateMachine->TransitionTo(TEXT("PlayerState_Idle"), 0.f);
		DUOL_TRACE(DUOL_CONSOLE, "OverDrive | ExitOverdriveEnter (AnimEnd)");
	}

	void PlayerState_Overdrive::EndOverdriveExit()
	{
		_stateMachine->TransitionTo(TEXT("PlayerState_Idle"), 0.f);
		DUOL_TRACE(DUOL_CONSOLE, "OverDrive | ExitOverdriveExit (AnimEnd)");
	}

	DUOLGameEngine::CoroutineHandler PlayerState_Overdrive::ReserveEndOverdriveState()
	{
		co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(OVERDRIVE_DURATION);

		// ��������̺갡 ���� �� ������ �˸���.
		auto idleState = reinterpret_cast<DUOLClient::PlayerState_Idle*>(_stateMachine->GetState(TEXT("PlayerState_Idle")));

		idleState->ReserveEndOverdrive();

		DUOL_TRACE(DUOL_CONSOLE, "OverDrive | ReserveEndOverDirveState");
	}

	DUOLGameEngine::CoroutineHandler PlayerState_Overdrive::UpdatePositionParticleOverdrive()
	{
		auto auraComp = _player->GetAuraSoundSource();
		auraComp->Play();

		float soundValue = 0;

		while (true)
		{
			if (_particleOverdrive == nullptr)
				break;

			auto delta = DUOLGameEngine::TimeManager::GetInstance()->GetDeltaTime();

			soundValue += delta;

			if (soundValue >= 1)
			{
				soundValue = 1;
			}

			auraComp->SetVolume(soundValue);
			_particleOverdrive->GetTransform()->SetPosition(_transform->GetWorldPosition());

			co_yield nullptr;
		}

		//�Ҹ� ��������!
		soundValue = 1;

		while(true)
		{
			auto delta = DUOLGameEngine::TimeManager::GetInstance()->GetDeltaTime();
			soundValue -= delta;

			if(soundValue < 0)
			{
				break;
			}

			auraComp->SetVolume(soundValue);

			co_yield nullptr;
		}

		auraComp->Stop();

		co_return;
	}

	void PlayerState_Overdrive::OnStateEnter(float deltaTime)
	{
		PlayerStateBase::OnStateEnter(deltaTime);

		// TODO : ī�޶� ���� �غ� ?
	}

	void PlayerState_Overdrive::OnStateStay(float deltaTime)
	{
		PlayerStateBase::OnStateStay(deltaTime);

		if (DUOLGameEngine::InputManager::GetInstance()->GetKeyDown(DUOLGameEngine::KeyCode::Keypad5))
		{
			_animator->SetBool(TEXT("ODExit"), true);
		}

		// TODO : ī�޶� ���� �� ..
		//��������
	}

	void PlayerState_Overdrive::OnStateExit(float deltaTime)
	{
		PlayerStateBase::OnStateExit(deltaTime);

		// ���� ����̺� ����. ���۾Ƹ� ����
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

			_player->SetSuperArmor(true);

			_animator->SetBool(TEXT("IsOverdriveSwordEnter"), false);

			_animator->SetBool(TEXT("IsOverdriveFistEnter"), false);
		}
		// ���� ����̺� ����. ��ƼŬ ���� + ���۾Ƹ� ����
		else
		{
			_animator->SetAnimatorController(_playerNormalAnimCon);

			if (_player->_isOverdriveFistMode)
			{
				if (_leftOverdriveFistAura != nullptr)
				{
					_leftOverdriveFistAura->Stop();

					_leftOverdriveFistAura = nullptr;
				}

				if (_rightOverdriveFistAura != nullptr)
				{
					_rightOverdriveFistAura->Stop();

					_rightOverdriveFistAura = nullptr;
				}
			}

			_player->_isOverdriveSwordMode = false;

			_player->_isOverdriveFistMode = false;

			_player->SetSuperArmor(false);

			_animator->SetBool(TEXT("ODExit"), false);

			// ��������̺� ����Ʈ ����
			if (_particleOverdrive != nullptr)
			{
				_particleOverdrive->Stop();

				_particleOverdrive = nullptr;
			}
		}

		_isEnter = false;

		_isSword = false;

		_player->_currentOverdrivePoint = 0.f;
	}
}
