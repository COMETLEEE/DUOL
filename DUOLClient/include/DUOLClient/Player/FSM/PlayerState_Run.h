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
		PlayerState_Run(DUOLClient::Player* player);

		virtual ~PlayerState_Run() override;

	private:
		/**
		 * \brief ����, ���� �������Ͻ�
		 */
		float _runSpeedSmoothness = 10.f;

	public:
		virtual void OnStateEnter(float deltaTime) override;

		virtual void OnStateStayFixed(float fixedTimeStep) override;

		virtual void OnStateStay(float deltaTime) override;

		virtual void OnStateExit(float deltaTime) override;
	};
}