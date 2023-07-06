#pragma once
#include "PlayerStateBase.h"

namespace DUOLClient
{
	/**
	 * \brief 피격 중 일정 수치 이상의 포인트가 쌓이면 해당 스테이트로 진입합니다.
	 */
	class PlayerState_Down : public DUOLClient::PlayerStateBase
	{
	public:
		PlayerState_Down(DUOLClient::Player* player);

		virtual ~PlayerState_Down();

	private:
		void GroundHitSound();

		void EndDown();

		void EndDownup();

		bool _isGroundSound;

		unsigned int _groundLayer;

		unsigned int _obstacleLayer;

		unsigned int _slopeLayer;

		unsigned int _groundCheckLayer;

	public:
		virtual void OnStateEnter(float deltaTime) override;

		virtual void OnStateStay(float deltaTime) override;

		virtual void OnStateStayFixed(float fixedTimeStep) override;

		virtual void OnStateExit(float deltaTime) override;
	};
}