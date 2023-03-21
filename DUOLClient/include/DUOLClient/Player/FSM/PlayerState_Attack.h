#pragma once
#include "PlayerStateBase.h"


namespace DUOLClient
{
	class PlayerState_Attack : public DUOLClient::PlayerStateBase
	{
	public:
		PlayerState_Attack();

		virtual ~PlayerState_Attack();

	private:
		std::vector<std::pair<DUOLCommon::tstring, DUOLCommon::EventListenerID>> _eventListenerIDs;

		bool _isInCancle;

		bool _isAttackCheckedInCancle;

		/**
		 * \brief 캔슬 프레임을 시작합니다.
		 */
		void StartCancleFrame();

		/**
		 * \brief 캔슬 프레임을 종료합니다.
		 */
		void EndCancleFrame();

		/**
		 * \brief 공격 애니메이션이 종료됩니다. 마지막 페이즈를 진행합니다.
		 */
		void EndAttack();

	public:
		virtual void OnStateEnter(float deltaTime) override;

		virtual void OnStateStay(float deltaTime) override;

		virtual void OnStateExit(float deltaTime) override;
	};
}