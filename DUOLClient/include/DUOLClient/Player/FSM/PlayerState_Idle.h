#pragma once
#include "DUOLClient/Player/FSM/PlayerStateBase.h"

namespace DUOLClient
{
	class PlayerState_Idle : public PlayerStateBase
	{
	public:
		PlayerState_Idle(DUOLClient::Player* player);

		virtual ~PlayerState_Idle() override;

	public:
		virtual void OnStateEnter(float deltaTime) override;

		virtual void OnStateStay(float deltaTime) override;

		virtual void OnStateExit(float deltaTime) override;

		friend class Player;
	};
}
