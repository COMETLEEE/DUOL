#pragma once
#include "PlayerStateBase.h"

namespace DUOLClient
{
	/**
	 * \brief �°� �ִ� ������Ʈ. ���� ����ġ�� ���� ��ġ�� �����ϸ� �ٿ ���ϴ�.
	 */
	class PlayerState_Hit : public DUOLClient::PlayerStateBase
	{
	public:
		PlayerState_Hit();

		virtual ~PlayerState_Hit();

	private:
		/**
		 * \brief ���� ����ġ
		 */
		float _downPoint;

	private:
		virtual void OnStateEnter(float deltaTime) override;

		virtual void OnStateStay(float deltaTime) override;

		virtual void OnStateExit(float deltaTime) override;
	};
}
