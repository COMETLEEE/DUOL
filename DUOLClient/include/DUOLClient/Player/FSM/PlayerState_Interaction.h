#pragma once
#include "DUOLClient/Player/FSM/PlayerStateBase.h"

namespace DUOLClient
{
	/**
	 * \brief ��ȣ�ۿ� ������Ʈ
	 */
	class PlayerState_Interaction : public DUOLClient::PlayerStateBase
	{
	public:
		PlayerState_Interaction(DUOLClient::Player* player);

		virtual ~PlayerState_Interaction();

	public:
		virtual void OnStateEnter(float deltaTime) override;

		virtual void OnStateStay(float deltaTime) override;

		virtual void OnStateExit(float deltaTime) override;
	};
}