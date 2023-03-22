#pragma once
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"
#include "DUOLGameEngine/FiniteStateMachine/FiniteStateMachine.h"
#include "DUOLGameEngine/Manager/InputManager.h"

namespace DUOLGameEngine
{
	class AnimatorController;
	class Rigidbody;
	class Animator;
}

namespace DUOLClient
{
	class MainCameraController;
}

namespace DUOLClient
{
#pragma region 플레이어 키 입력
	constexpr DUOLGameEngine::KeyCode UP_KEY = DUOLGameEngine::KeyCode::W;
	constexpr DUOLGameEngine::KeyCode LEFT_KEY = DUOLGameEngine::KeyCode::A;
	constexpr DUOLGameEngine::KeyCode DOWN_KEY = DUOLGameEngine::KeyCode::S;
	constexpr DUOLGameEngine::KeyCode RIGHT_KEY= DUOLGameEngine::KeyCode::D;

	constexpr DUOLGameEngine::MouseCode ATTACK_KEY = DUOLGameEngine::MouseCode::Left;
	constexpr DUOLGameEngine::KeyCode RUN_KEY = DUOLGameEngine::KeyCode::LeftShift;

	constexpr DUOLGameEngine::KeyCode DASH_KEY = DUOLGameEngine::KeyCode::Space;
	constexpr DUOLGameEngine::KeyCode LOCKON_KEY = DUOLGameEngine::KeyCode::V;
#pragma endregion

	/**
	 * \brief Player State Machine 
	 */
	class Player : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		Player(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("Player"));

		virtual ~Player();

	private:
		/**
		 * \brief Sword Animator Controller
		 */
		DUOLGameEngine::AnimatorController* _swordAnimatorController;

		DUOLGameEngine::FiniteStateMachine _playerStateMachine;

		float _hp;

		float _defaultSwordDamage;

		float _defaultPunchDamage;

		float _defaultOverdriveDamage;

		float _defaultSwordAttackSpeed;

		float _defaultPunchAttackSpeed;

		float _defaultOverdriveAttackSpeed;

		float _defaultMaxMoveSpeed;

		float _defaultMaxRunSpeed;

		float _currentMoveSpeed;

		bool _isLockOnMode;

#pragma region 기본 참조 컴포넌트
		DUOLGameEngine::Transform* _playerTransform;

		DUOLGameEngine::Animator* _playerAnimator;

		DUOLGameEngine::Rigidbody* _playerRigidbody;

		/**
		 * \brief 메인 카메라 트랜스폼.
		 */
		DUOLGameEngine::Transform* _cameraTransform;

		DUOLClient::MainCameraController* _mainCamController;
#pragma endregion

	private:
		void InitializeStateMachine();

	public:
		virtual void OnStart() override;

		virtual void OnUpdate(float deltaTime) override;

		virtual void OnFixedUpdate(float fixedTimeStep) override;

		friend class PlayerStateBase;

		friend class PlayerState_Idle;

		friend class PlayerState_Move;

		friend class PlayerState_Attack;

		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)
	};
}
