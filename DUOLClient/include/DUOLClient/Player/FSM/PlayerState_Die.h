#pragma once
#include "PlayerStateBase.h"

namespace DUOLClient
{
	/**
	 * \brief 죽은 상태. 애니메이션 및 연출 완료 후 초기 상태로 게임을 다시 시작한다.
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
