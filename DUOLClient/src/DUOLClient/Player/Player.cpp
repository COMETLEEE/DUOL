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
#include "DUOLClient/Player/Weapon_Wave.h"
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
#include "DUOLClient/Player/FSM/PlayerState_Overdrive.h"
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
	)
	.property("_currentOverdrivePoint", &DUOLClient::Player::_currentOverdrivePoint)
	(
		metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
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
		, _currentDownPoint(0.f)
		, _currentOverdrivePoint(0.f)
		, _canStartAttack(true)
		, _isLockOnMode(false)
		, _isOverdriveSwordMode(false)
		, _isOverdriveFistMode(false)
		, _playerTransform(nullptr)
		, _playerAnimator(nullptr)
		, _playerRigidbody(nullptr)
		, _cameraTransform(nullptr)
	{
		SetHP(1000.f);
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
		if (currentStateName == TEXT("PlayerState_Die") || currentStateName == TEXT("PlayerState_Down") 
			|| currentStateName == TEXT("PlayerState_Dash") || currentStateName == TEXT("PlayerState_Interaction"))
			return;

		_hp -= damage;

		_currentDownPoint += DOWN_POINT_PER_ATTACK;

		// ���۾Ƹ��� ��쵵 �����ٵ� ..
		DUOLClient::PlayerState_Hit* hitState = reinterpret_cast<DUOLClient::PlayerState_Hit*>(_playerStateMachine.GetState(TEXT("PlayerState_Hit")));

		// HP �� 0���� �۾�����.
		if (_hp <= 0)
		{
			_playerStateMachine.TransitionTo(TEXT("PlayerState_Die"), 0.f);
		}
		// �ٿ� �������� �� ���� Down state�� ..!
		else if (_currentDownPoint >= MAX_DOWN_POINT)
		{
			_playerStateMachine.TransitionTo(TEXT("PlayerState_Down"), 0.f);
		}
		else if (currentStateName != TEXT("PlayerState_Hit"))
		{
			// Hit state �� �ƴ϶�� Hit state�� ..!
			hitState->SetCurrentAttackType(attackType);

			_playerStateMachine.TransitionTo(TEXT("PlayerState_Hit"), 0.f);
		}
		else
		{
			hitState->SetCurrentAttackType(attackType);

			// Hit state ��� �ߺ� ��Ʈ ����
			hitState->AccumulateHit();
		}
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

				// TODO : ��˵� ���⼭ ĳ��
				_playerOverdriveWeaponSword = _playerWeaponSword;
			}
			else if (gameObject->GetTag() == TEXT("FistFormHolder_L"))
			{
				_playerLeftFistHolder = gameObject;
			}
			else if (gameObject->GetTag() == TEXT("FistFormHolder_R"))
			{
				_playerRightFistHolder = gameObject;
			}
		}

		// ����� ������Ʈ
		DUOLGameEngine::GameObject* weaponWave = GetGameObject()->GetScene()->CreateEmpty();

		_playerWeaponWave = weaponWave->AddComponent<DUOLClient::Weapon_Wave>();

		_playerWeaponWave->_player = this;

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

		PlayerState_Overdrive* overdrive = _playerStateMachine.AddState<PlayerState_Overdrive>(this);
#pragma endregion
	}

	void Player::OnStart()
	{
		// State Machine �� �ʱ�ȭ�մϴ�.
		InitializeStateMachine();
	}
	
	void Player::OnUpdate(float deltaTime)
	{
		// ��� ��Ÿ ���׿� ���ؼ� ������ �������ϰ�, �÷��̾��� ������Ʈ �ӽ��� �����մϴ�.
		_playerStateMachine.UpdateStateMachine(deltaTime);
	}

	void Player::OnFixedUpdate(float fixedTimeStep)
	{
		// ������Ʈ �ӽ��� ���� ����
		_playerStateMachine.FixedUpdateStateMachine(fixedTimeStep);
	}
}