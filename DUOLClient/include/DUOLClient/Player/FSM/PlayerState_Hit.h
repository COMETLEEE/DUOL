#pragma once
#include "PlayerStateBase.h"

namespace DUOLClient
{
	/**
	 * \brief 맞고 있는 스테이트. 스턴 축적치가 일정 수치에 도달하면 다운에 들어갑니다.
	 */
	class PlayerState_Hit : public DUOLClient::PlayerStateBase
	{
	public:
		PlayerState_Hit();

		virtual ~PlayerState_Hit();

	private:
		/**
		 * \brief 스턴 축적치
		 */
		float _downPoint;

	private:
		virtual void OnStateEnter(float deltaTime) override;

		virtual void OnStateStay(float deltaTime) override;

		virtual void OnStateExit(float deltaTime) override;
	};
}
