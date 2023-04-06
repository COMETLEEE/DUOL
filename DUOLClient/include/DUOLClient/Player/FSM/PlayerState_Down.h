#pragma once
#include "PlayerStateBase.h"

namespace DUOLClient
{
	/**
	 * \brief �ǰ� �� ���� ��ġ �̻��� ����Ʈ�� ���̸� �ش� ������Ʈ�� �����մϴ�.
	 */
	class PlayerState_Down : public DUOLClient::PlayerStateBase
	{
	public:
		PlayerState_Down(DUOLClient::Player* player);

		virtual ~PlayerState_Down();

	public:
		virtual void OnStateEnter(float deltaTime) override;

		virtual void OnStateStay(float deltaTime) override;

		virtual void OnStateExit(float deltaTime) override;
	};
}