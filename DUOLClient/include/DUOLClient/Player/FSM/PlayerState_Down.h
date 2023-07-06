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