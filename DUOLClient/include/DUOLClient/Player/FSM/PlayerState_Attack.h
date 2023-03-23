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

		bool _isInCancle;

		bool _isAttackCheckedInCancle;

		/**
		 * \brief ĵ�� �������� �����մϴ�.
		 */
		void StartCancleFrame();

		void SwordFirstHitFrame();

		void SwordSecondHitFrame();

		void SwordThirdHitFrame();

		void SwordFourthHitFrame();

		/**
		 * \brief ĵ�� �������� �����մϴ�.
		 */
		void EndCancleFrame();

		/**
		 * \brief ���� �ִϸ��̼��� ����˴ϴ�. ������ ����� �����մϴ�.
		 */
		void EndAttack();

	public:
		virtual void OnStateEnter(float deltaTime) override;

		virtual void OnStateStay(float deltaTime) override;

		virtual void OnStateExit(float deltaTime) override;
	};
}