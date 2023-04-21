#pragma once
#include "DUOLClient/Player/FSM/PlayerStateBase.h"

namespace DUOLClient
{
	/**
	 * \brief 플레이어 기본 상태
	 */
	class PlayerState_Idle : public PlayerStateBase
	{
	public:
		PlayerState_Idle(DUOLClient::Player* player);

		virtual ~PlayerState_Idle() override;

		void OnNormalStateStay(float deltaTime);

		void OnOverdriveStateStay(float deltaTime);

	public:
		virtual void OnStateEnter(float deltaTime) override;

		virtual void OnStateStay(float deltaTime) override;

		virtual void OnStateExit(float deltaTime) override;

		friend class Player;
	};
}
