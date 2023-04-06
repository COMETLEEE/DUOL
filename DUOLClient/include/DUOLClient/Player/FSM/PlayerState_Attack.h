#pragma once
#include "PlayerStateBase.h"

namespace DUOLClient
{
	constexpr DUOLMath::Vector3 SWORD_HIT_BOX = { 4.f, 2.f, 3.f };

	constexpr DUOLMath::Vector3 SWORD_FOURTH_HIT_BOX = { 6.f, 3.f, 3.f };

	constexpr float SWORD_FOURTH_HIT_RANGE = 3.f;

	class PlayerState_Attack : public DUOLClient::PlayerStateBase
	{
	public:
		PlayerState_Attack();

		virtual ~PlayerState_Attack();

	private:
		std::vector<std::pair<DUOLCommon::tstring, DUOLCommon::EventListenerID>> _eventListenerIDs;

		/**
		 * \brief 캔슬 프레임 내부에 있나요.
		 */
		bool _isInCancle;

		/**
		 * \brief 캔슬 프레임 도중에 공격 요청이 있었나요.
		 */
		bool _isAttackCheckedInCancle;

		/**
		 * \brief 좌클릭
		 */
		bool _nextSwordAttack;

		/**
		 * \brief 우클릭
		 */
		bool _nextPunchAttack;

		/**
		 * \brief 현재 몇 번 때렸나요. 애니메이션 콤보 테이블을 두고 연동하겠습니다.
		 */
		int _attackCount;






		/**
		 * \brief 캔슬 프레임을 시작합니다.
		 */
		void StartCancleFrame();

		/**
		 * \brief 캔슬 프레임을 종료합니다.
		 */
		void EndCancleFrame();

		/**
		 * \brief 모든 공격 종류의 애니메이션이 종료될 때 발생하는 이벤트. 현재 조건에 따라서
		 * 다음 공격 애니메이션을 재생한다.
		 */
		void EndAnimation();


		void SwordAttackStart();

		void SwordAttackEnd();

		void SwordFourthHitFrame();

		/**
		 * \brief 공격이 종료됩니다. 마지막 페이즈를 진행합니다.
		 */
		void EndAttack();

	public:
		virtual void OnStateEnter(float deltaTime) override;

		virtual void OnStateStay(float deltaTime) override;

		virtual void OnStateExit(float deltaTime) override;
	};
}