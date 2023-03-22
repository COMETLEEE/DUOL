#pragma once
#include "DUOLClient/Player/FSM/PlayerStateBase.h"


namespace DUOLClient
{
	/**
	 * \brief ȸ�� ������Ʈ. �ϴ�, �ִϸ��̼��� �������� ���� Idle �ִϸ��̼����� ����������.
	 */
	class PlayerState_Dash : public DUOLClient::PlayerStateBase
	{
	public:
		PlayerState_Dash();

		virtual ~PlayerState_Dash() override;
		
	public:
		virtual void OnStateEnter(float deltaTime) override;

		virtual void OnStateStay(float deltaTime) override;

		virtual void OnStateExit(float deltaTime) override;
	};
}