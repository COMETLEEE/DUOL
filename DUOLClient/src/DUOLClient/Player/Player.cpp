#include "DUOLClient/Player/Player.h"

#include "DUOLClient/Player/FSM/PlayerState_Idle.h"
#include "DUOLClient/Player/FSM/PlayerState_Move.h"
#include "DUOLClient/Player/FSM/PlayerState_Attack.h"
#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Transform.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"
#include "DUOLGameEngine/Manager/InputManager.h"

#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"
#include "DUOLGameEngine/Manager/ResourceManager.h"

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
		MonoBehaviourBase(owner, name)
		, _hp(10.f)
		, _defaultSwordDamage(2.f)
		, _defaultPunchDamage(2.f)
		, _defaultOverdriveDamage(3.f)
		, _defaultSwordAttackSpeed(1.f)
		, _defaultPunchAttackSpeed(1.f)
		, _defaultOverdriveAttackSpeed(1.f)
		, _defaultMaxMoveSpeed(4.f)
		, _currentMoveSpeed(0.f)
		, _playerTransform(nullptr)
		, _playerAnimator(nullptr)
		, _playerRigidbody(nullptr)
		, _cameraTransform(nullptr)
	{
	}

	Player::~Player()
	{
	}

	void Player::InitializeStateMachine()
	{
		// Using other game object (e.x. main camera's transform) caching
		auto& allGOs = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

		// Main cameras transform.
		for (auto gameObject : allGOs)
		{
			if (gameObject->GetTag() == TEXT("MainCamera"))
				_cameraTransform = gameObject->GetTransform();
		}

		_playerTransform = GetGameObject()->GetTransform();

		_playerAnimator = GetGameObject()->GetComponent<DUOLGameEngine::Animator>();

		_playerRigidbody = GetGameObject()->GetComponent<DUOLGameEngine::Rigidbody>();

		PlayerState_Idle* idle = _playerStateMachine.AddState<PlayerState_Idle>();

		idle->Initialize(this);

		PlayerState_Move* move = _playerStateMachine.AddState<PlayerState_Move>();

		move->Initialize(this);

		PlayerState_Attack* attack = _playerStateMachine.AddState<PlayerState_Attack>();

		attack->Initialize(this);
	}
	
	void Player::OnStart()
	{
		_swordAnimatorController = DUOLGameEngine::ResourceManager::GetInstance()->GetAnimatorController(TEXT("Player_SwordAnimatorController"));

		// State Machine 을 초기화합니다.
		InitializeStateMachine();
	}

	void Player::OnUpdate(float deltaTime)
	{
		// 모든 기타 제반 사항에 대해서 갱신을 마무리하고, 플레이어의 스테이트 머신을 갱신합니다.
		_playerStateMachine.UpdateStateMachine(deltaTime);
	}

	void Player::OnFixedUpdate(float fixedTimeStep)
	{
		// 스테이트 머신의 물리 갱신
		_playerStateMachine.FixedUpdateStateMachine(fixedTimeStep);
	}
}
