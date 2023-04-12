#include "DUOLClient/Player/Player.h"

#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"
#include "DUOLGameEngine/Manager/ResourceManager.h"
#include "DUOLGameEngine/Manager/InputManager.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Transform.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"
#include "DUOLGameEngine/ECS/Component/BoxCollider.h"
#include "DUOLGameEngine/ECS/Component/ParticleRenderer.h"

#include "DUOLClient/Player/Weapon_Sword.h"
#include "DUOLClient/Player/FSM/PlayerState_Idle.h"
#include "DUOLClient/Player/FSM/PlayerState_Move.h"
#include "DUOLClient/Player/FSM/PlayerState_Attack.h"
#include "DUOLClient/Player/FSM//PlayerState_Hit.h"
#include "DUOLClient/Player/FSM//PlayerState_Dash.h"
#include "DUOLClient/Player/FSM//PlayerState_Run.h"
#include "DUOLClient/Player/FSM//PlayerState_Die.h"
#include "DUOLClient/Player/FSM//PlayerState_Transcendence.h"
#include "DUOLClient/Player/FSM//PlayerState_Interaction.h"

#include "DUOLClient/Camera/MainCameraController.h"
#include "DUOLClient/Manager/ParticleManager.h"



#include <rttr/registration>

#include "DUOLClient/Player/FSM/PlayerState_Down.h"
#include "DUOLCommon/MetaDataType.h"

using namespace rttr;

RTTR_REGISTRATION
{
	rttr::registration::class_<DUOLClient::Player>("Player")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	);
}

namespace DUOLClient
{
	Player::Player(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		CharacterBase(owner, name)
		, _defaultSwordDamage(2.f)
		, _defaultPunchDamage(2.f)
		, _defaultOverdriveDamage(3.f)
		, _defaultSwordAttackSpeed(1.f)
		, _defaultPunchAttackSpeed(1.f)
		, _defaultOverdriveAttackSpeed(1.f)
		, _defaultMaxMoveSpeed(4.f)
		, _defaultMaxRunSpeed(6.f)
		, _defaultMaxLockOnMoveSpeed(2.f)
		, _defaultMaxLockOnRunSpeed(4.f)
		, _currentDamage(20.f)
		, _currentMoveSpeed(0.f)
		, _inAttackPostDelay(0.5f)
		, _endAttackPostDelay(1.f)
		, _canStartAttack(true)
		, _isLockOnMode(false)
		, _playerTransform(nullptr)
		, _playerAnimator(nullptr)
		, _playerRigidbody(nullptr)
		, _cameraTransform(nullptr)
	{
		SetHP(10000000000.f);
	}

	Player::~Player()
	{
	}

	void Player::Attack(CharacterBase* other, float damage, AttackType attackType)
	{
		// OnHit ȣ�� 
		other->OnHit(this, damage, attackType);
	}

	void Player::OnHit(CharacterBase* other, float damage, AttackType attackType)
	{
		auto& currentStateName = _playerStateMachine.GetCurrentState()->GetName();

		// ������ ��Ȳ�� ���ؼ��� �Ѿ ..!
		if (currentStateName == TEXT("PlayerState_Die") || currentStateName == TEXT("PlayerState_Dash")
			|| currentStateName == TEXT("PlayerState_Interaction"))
			return;

		_hp -= damage;

		DUOLClient::PlayerState_Hit* hitState =	reinterpret_cast<DUOLClient::PlayerState_Hit*>(_playerStateMachine.GetState(TEXT("PlayerState_Hit")));

		//// TODO : ���� HIt state animation �� �����Ƿ� �Ѿ��.
		//// Hit state �� �ƴ϶�� Hit state�� ..!
		//if (currentStateName != TEXT("PlayerState_Hit"))
		//{
		//	_playerStateMachine.TransitionTo(TEXT("PlayerState_Hit"), 0.f);

		//	hitState->SetCurrentAttackType(attackType);
		//}
		//// �ߺ� ��Ʈ
		//else
		//{
		//	hitState->SetCurrentAttackType(attackType);

		//	hitState->AccumulateHit();
		//}
	}

	void Player::InitializeStateMachine()
	{
		// Using other game object (e.x. main camera's transform) caching
		auto& allGOs = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

		// Main cameras transform.
		for (auto gameObject : allGOs)
		{
			// ī�޶� ������Ʈ
			if (gameObject->GetTag() == TEXT("MainCamera"))
			{
				_cameraTransform = gameObject->GetTransform();

				// Main Camera Controller �� ���⿡ �޷��ֽ��ϴ�.
				_mainCamController = gameObject->GetTransform()->GetParent()->GetGameObject()->GetComponent<DUOLClient::MainCameraController>();
			}
			// �� ������Ʈ
			else if (gameObject->GetTag() == TEXT("Weapon_Sword"))
			{
				_playerWeaponSword = gameObject->GetComponent<DUOLClient::Weapon_Sword>();

				_playerWeaponSwordCollider = gameObject->GetComponent<DUOLGameEngine::BoxCollider>();

				_playerWeaponSwordObject = gameObject;
			}
		}

		_playerTransform = GetGameObject()->GetTransform();

		_playerAnimator = GetGameObject()->GetComponent<DUOLGameEngine::Animator>();

		_playerRigidbody = GetGameObject()->GetComponent<DUOLGameEngine::Rigidbody>();

#pragma region ADD_ALL_STATE
		PlayerState_Idle* idle = _playerStateMachine.AddState<PlayerState_Idle>(this);

		PlayerState_Move* move = _playerStateMachine.AddState<PlayerState_Move>(this);

		PlayerState_Run* run = _playerStateMachine.AddState<PlayerState_Run>(this);

		PlayerState_Dash* dash = _playerStateMachine.AddState<PlayerState_Dash>(this);

		PlayerState_Attack* attack = _playerStateMachine.AddState<PlayerState_Attack>(this);

		PlayerState_Hit* hit = _playerStateMachine.AddState<PlayerState_Hit>(this);

		PlayerState_Down* down = _playerStateMachine.AddState<PlayerState_Down>(this);

		PlayerState_Die* die = _playerStateMachine.AddState<PlayerState_Die>(this);
#pragma endregion
	}

	void Player::OnStart()
	{
		_swordAnimatorController = DUOLGameEngine::ResourceManager::GetInstance()->GetAnimatorController(TEXT("Player_SwordAnimatorController"));

		// State Machine �� �ʱ�ȭ�մϴ�.
		InitializeStateMachine();
	}
	
	void Player::OnUpdate(float deltaTime)
	{
		// ��� ��Ÿ ���� ���׿� ���ؼ� ������ �������ϰ�, �÷��̾��� ������Ʈ �ӽ��� �����մϴ�.
		_playerStateMachine.UpdateStateMachine(deltaTime);
	}

	void Player::OnFixedUpdate(float fixedTimeStep)
	{
		// ������Ʈ �ӽ��� ���� ����
		_playerStateMachine.FixedUpdateStateMachine(fixedTimeStep);
	}
}