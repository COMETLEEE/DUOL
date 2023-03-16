#pragma once
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"
#include "DUOLGameEngine/FiniteStateMachine/FiniteStateMachine.h"
#include "DUOLGameEngine/Manager/InputManager.h"

namespace DUOLClient
{
	constexpr DUOLGameEngine::KeyCode UP_KEY = DUOLGameEngine::KeyCode::W;
	constexpr DUOLGameEngine::KeyCode LEFT_KEY = DUOLGameEngine::KeyCode::A;
	constexpr DUOLGameEngine::KeyCode DOWN_KEY = DUOLGameEngine::KeyCode::S;
	constexpr DUOLGameEngine::KeyCode RIGHT_KEY= DUOLGameEngine::KeyCode::D;

	constexpr DUOLGameEngine::MouseCode ATTACK_KEY = DUOLGameEngine::MouseCode::Left;
	constexpr DUOLGameEngine::KeyCode DASH_KEY = DUOLGameEngine::KeyCode::LeftShift;

	constexpr DUOLGameEngine::KeyCode EVADE_KEY = DUOLGameEngine::KeyCode::Space;

	struct PlayerProperty
	{
		float _hp = 10;

		float _swordDamage = 1;

		float _punchDamage = 1;

		float _overdriveDamage = 2.f;

		float _swordAttackSpeed = 1.f;

		float _punchAttackSpeed = 1.f;

		float _overdriveAttackSpeed = 1.f;

		// 현재 이동 속도
		float _moveSpeed = 0.f;

		float _maxMoveSpeed = 6.f;

		float _maxDashSpeed = 10.f;
	};

	/**
	 * \brief Player State Machine
	 */
	class Player : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		Player(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("Player"));

		virtual ~Player();

	public:
		DUOLGameEngine::FiniteStateMachine _playerStateMachine;

		PlayerProperty _currentPlayerProperty;

	private:
		void InitializeStateMachine();

	public:
		virtual void OnStart() override;

		virtual void OnUpdate(float deltaTime) override;

		virtual void OnFixedUpdate(float fixedTimeStep) override;

		friend class PlayerStateBase;

		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)
	};
}
