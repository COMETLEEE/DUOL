#pragma once
#include "PlayerStateBase.h"

namespace DUOLClient
{
	/**
	 * \brief ���� ����. �ش� ���¿����� ���͸� �аų�, ����Ʈ���� �������� ������ ���� Ư�� ������� �� �� ������
	 * ���� ������Ʈ�� �������ϴ�.
	 */
	class PlayerState_Run : public DUOLClient::PlayerStateBase
	{
	public:
		PlayerState_Run();

		virtual ~PlayerState_Run() override;

	public:
		virtual void OnStateEnter(float deltaTime) override;

		virtual void OnStateStay(float deltaTime) override;

		virtual void OnStateExit(float deltaTime) override;
	};
}