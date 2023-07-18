#pragma once
#include "DUOLClient/Player/FSM/PlayerStateBase.h"
#include "DUOLClient/Util/EventListenerIDHolder.h"

namespace DUOLGameEngine
{
	class ParticleRenderer;
}

namespace DUOLClient
{
	constexpr float DASH_IMPULSE = 9.1f;
		
	/**
	 * \brief ȸ�� ������Ʈ. �ϴ�, �ִϸ��̼��� �������� ���� Idle �ִϸ��̼����� ����������.
	 */
	class PlayerState_Dash : public DUOLClient::PlayerStateBase
	{
	public:
		PlayerState_Dash(DUOLClient::Player* player);

		virtual ~PlayerState_Dash() override;

	private:
		DUOLGameEngine::ParticleRenderer* _particleRenderer;

		void StartDash();

		void EndDash();

		void DashSoundEffect();

	public:
		virtual void OnStateEnter(float deltaTime) override;

		virtual void OnStateExit(float deltaTime) override;
	};
}
