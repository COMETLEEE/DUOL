#pragma once
#include "DUOLClient/Player/FSM/PlayerStateBase.h"

namespace DUOLClient
{
	class PlayerState_Jump : public DUOLClient::PlayerStateBase
	{
	public:
		PlayerState_Jump(DUOLClient::Player* player);

		virtual ~PlayerState_Jump() override;

		float _currentHoldTime;

		float _currentFlyTime;

	public:
		virtual void OnStateEnter(float deltaTime) override;

		virtual void OnStateStay(float deltaTime) override;

		virtual void OnStateExit(float deltaTime) override;
	};
}
