#pragma once
#include "PlayerStateBase.h"

namespace DUOLClient
{
	/**
	 * \brief ���� ����. �ִϸ��̼� �� ���� �Ϸ� �� �ʱ� ���·� ������ �ٽ� �����Ѵ�.
	 */
	class PlayerState_Die : public DUOLClient::PlayerStateBase
	{
	public:
		PlayerState_Die(DUOLClient::Player* player);

		virtual ~PlayerState_Die() override;

	private:
		virtual void OnStateEnter(float deltaTime) override;

		virtual void OnStateStay(float deltaTime) override;

		virtual void OnStateExit(float deltaTime) override;
	};
}
