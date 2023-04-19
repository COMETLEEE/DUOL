#pragma once
#include "DUOLClient/Player/FSM/PlayerStateBase.h"

namespace DUOLGameEngine
{
	class Transform;
	class Animator;
	class Rigidbody;
}

namespace DUOLClient
{
	class Player;

	class PlayerState_Move : public DUOLClient::PlayerStateBase
	{
	public:
		PlayerState_Move(DUOLClient::Player* player);

		virtual ~PlayerState_Move() override;

		/**
		 * \brief 가속, 감속 스무스니스
		 */
		float _moveSpeedSmoothnesss = 10.f;

	public:
		virtual void OnStateEnter(float deltaTime) override;

		virtual void OnStateStayFixed(float fixedTimeStep) override;

		virtual void OnStateStay(float deltaTime) override;

		virtual void OnStateExit(float deltaTime) override;

#pragma region FRIEND_CLASS
		friend class Player;
#pragma endregion
	};
}