#pragma once
#include "DUOLClient/Player/FSM/PlayerStateBase.h"

namespace DUOLClient
{
	class PlayerState_Overdrive;
}

namespace DUOLClient
{
	/**
	 * \brief 플레이어 기본 상태
	 */
	class PlayerState_Idle : public PlayerStateBase
	{
	public:
		PlayerState_Idle(DUOLClient::Player* player);

		virtual ~PlayerState_Idle() override;

	private:
		bool _isReservedEndOverdrive;

	private:
		void OnNormalStateStay(float deltaTime);

		void OnOverdriveStateStay(float deltaTime);

		void ReserveEndOverdrive();

	public:
		virtual void OnStateEnter(float deltaTime) override;

		virtual void OnStateStay(float deltaTime) override;

		virtual void OnStateExit(float deltaTime) override;

#pragma region FRIEND_CLASS
		friend class Player;

		friend class PlayerState_Overdrive;
#pragma endregion
	};
}