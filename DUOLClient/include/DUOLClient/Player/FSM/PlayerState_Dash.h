#pragma once
#include "DUOLClient/Player/FSM/PlayerStateBase.h"
#include "DUOLClient/Util/EventListenerIDHolder.h"

namespace DUOLClient
{
	/**
	 * \brief ȸ�� ������Ʈ. �ϴ�, �ִϸ��̼��� �������� ���� Idle �ִϸ��̼����� ����������.
	 */
	class PlayerState_Dash : public DUOLClient::PlayerStateBase
	{
	public:
		PlayerState_Dash(DUOLClient::Player* player);

		virtual ~PlayerState_Dash() override;

	private:
		void StartDash();

		void EndDash();

	public:
		virtual void OnStateEnter(float deltaTime) override;

		virtual void OnStateExit(float deltaTime) override;
	};
}
