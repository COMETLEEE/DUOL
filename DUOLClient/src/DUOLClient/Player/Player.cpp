#include "DUOLClient/Player/Player.h"

#include "DUOLClient/Player/FSM/PlayerState_Idle.h"
#include "DUOLClient/Player/FSM/PlayerState_Move.h"
#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Transform.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"
#include "DUOLGameEngine/Manager/InputManager.h"

#include <rttr/registration>
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
		MonoBehaviourBase(owner, name)
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
		DUOLGameEngine::Transform* cameraTransform = nullptr;

		for (auto gameObject : allGOs)
		{
			if (gameObject->GetTag() == TEXT("MainCamera"))
				cameraTransform = gameObject->GetTransform();
		}

		DUOLGameEngine::Transform* transform = GetGameObject()->GetTransform();

		DUOLGameEngine::Animator* animator = GetGameObject()->GetComponent<DUOLGameEngine::Animator>();

		DUOLGameEngine::Rigidbody* rigidbody = GetGameObject()->GetComponent<DUOLGameEngine::Rigidbody>();

		PlayerState_Idle* idle = _playerStateMachine.AddState<PlayerState_Idle>();

		idle->_transform = transform;

		idle->_animator = animator;

		idle->_rigidbody = rigidbody;

		idle->_cameraTransform = cameraTransform;

		idle->_player = this;

		PlayerState_Move* move = _playerStateMachine.AddState<PlayerState_Move>();

		move->_transform = transform;

		move->_animator = animator;

		move->_rigidbody = rigidbody;

		move->_cameraTransform = cameraTransform;

		move->_player = this;
	}
	
	void Player::OnStart()
	{
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
