#pragma once
#include "DUOLClient/Player/FSM/PlayerStateBase.h"

namespace DUOLClient
{
	/**
	 * \brief 상호작용 스테이트
	 */
	class PlayerState_Interaction : public DUOLClient::PlayerStateBase
	{
	public:
		PlayerState_Interaction();

		virtual ~PlayerState_Interaction();

	public:
		virtual void OnStateEnter(float deltaTime) override;

		virtual void OnStateStay(float deltaTime) override;

		virtual void OnStateExit(float deltaTime) override;
	};
}