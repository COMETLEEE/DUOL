/**

    @file      PlayerState_Overdrive.h
    @brief     Enter Overdrive state.
    @details   ~
    @author    COMETLEE
    @date      20.04.2023
    @copyright © COMETLEE, 2023. All right reserved.

**/
#pragma once
#include "DUOLClient/Player/FSM/PlayerStateBase.h"

namespace DUOLClient
{
	/**
	 * \brief 플레이어 오버 드라이브 진입, 해제 담당 스테이트.
	 */
	class PlayerState_Overdrive : public DUOLClient::PlayerStateBase
	{
	public:
		PlayerState_Overdrive(DUOLClient::Player* player);

		virtual ~PlayerState_Overdrive();

	private:
		bool _isEnter;

		bool _isSword;

		DUOLGameEngine::AnimatorController* _playerNormalAnimCon;

		DUOLGameEngine::AnimatorController* _playerOverdriveSwordAnimCon;

		DUOLGameEngine::AnimatorController* _playerOverdriveFistAnimCon;

	public:
		void EnterOverdriveSword();

		void EnterOverdriveFist();

		void ExitOverdriveSword();

		void ExitOverdriveFist();

	private:
		void EndOverdriveEnter();

	public:
		virtual void OnStateEnter(float deltaTime) override;

		virtual void OnStateStay(float deltaTime) override;

		virtual void OnStateExit(float deltaTime) override;
	};
}