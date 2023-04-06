#pragma once
#include "DUOLClient/Player/FSM/PlayerStateBase.h"
#include "DUOLClient/Util/EventListenerIDHolder.h"

namespace DUOLClient
{
	constexpr float DASH_FORCE = 2000.f;

	/**
	 * \brief 회피 스테이트. 일단, 애니메이션이 정해지기 전엔 Idle 애니메이션으로 움직여보자.
	 */
	class PlayerState_Dash : public DUOLClient::PlayerStateBase
	{
	public:
		PlayerState_Dash(DUOLClient::Player* player);

		virtual ~PlayerState_Dash() override;

	private:
		DUOLClient::EventListenerIDHolder<void> _eventListenerIDHolder;

		bool _isEndDash;

	private:
		void OnStartDash();

		void OnEndDash();

	public:
		virtual void OnStateEnter(float deltaTime) override;

		virtual void OnStateStay(float deltaTime) override;

		virtual void OnStateExit(float deltaTime) override;
	};
}
