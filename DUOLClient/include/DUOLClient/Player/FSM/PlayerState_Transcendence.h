#pragma once
#include "DUOLClient/Player/FSM/PlayerStateBase.h"

namespace DUOLClient
{
	/**
	 * \brief �ʻ�� ������Ʈ.
	 */
	class PlayerState_Transcendence : public DUOLClient::PlayerStateBase
	{
	public:
		PlayerState_Transcendence();

		virtual ~PlayerState_Transcendence();

	public:
		virtual void OnStateEnter(float deltaTime) override;

		virtual void OnStateStay(float deltaTime) override;

		virtual void OnStateExit(float deltaTime) override;
	};
}