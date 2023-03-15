#include "DUOLClient/Player/Player.h"

#include "DUOLClient/Player/FSM/PlayerState_Idle.h"
#include "DUOLClient/Player/FSM/PlayerState_Move.h"
#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Transform.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"
#include "DUOLGameEngine/Manager/InputManager.h"

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
		auto& allGOs = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

		// Using other game object (e.x. main camera's transform) caching
		for (auto gameObject : allGOs)
		{
			if (gameObject->GetTag() == TEXT("MainCamera"))
				_cameraTransform = gameObject->GetTransform();
		}

		DUOLGameEngine::Transform* transform = GetGameObject()->GetTransform();

		DUOLGameEngine::Animator* animator = GetGameObject()->GetComponent<DUOLGameEngine::Animator>();

		PlayerState_Idle* idle = _playerStateMachine.AddState<PlayerState_Idle>();

		idle->_transform = transform;

		idle->_animator = animator;

		idle->_cameraTransform = _cameraTransform;

		PlayerState_Move* move = _playerStateMachine.AddState<PlayerState_Move>();

		move->_transform = transform;

		move->_animator = animator;

		move->_cameraTransform = _cameraTransform;
	}
	
	void Player::OnStart()
	{
		MonoBehaviourBase::OnStart();

		// State Machine 을 초기화합니다.
		InitializeStateMachine();
	}

	void Player::OnLateUpdate(float deltaTime)
	{
		MonoBehaviourBase::OnLateUpdate(deltaTime);

		// 모든 기타 제반 사항에 대해서 갱신을 마무리하고, 플레이어의 스테이트 머신을 갱신합니다.
		_playerStateMachine.UpdateStateMachine(deltaTime);
	}
}
