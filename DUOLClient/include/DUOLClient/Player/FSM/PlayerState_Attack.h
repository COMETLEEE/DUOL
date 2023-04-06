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
		 * \brief ĵ�� ������ ���ο� �ֳ���.
		 */
		bool _isInCancle;

		/**
		 * \brief ĵ�� ������ ���߿� ���� ��û�� �־�����.
		 */
		bool _isAttackCheckedInCancle;

		/**
		 * \brief ��Ŭ��
		 */
		bool _nextSwordAttack;

		/**
		 * \brief ��Ŭ��
		 */
		bool _nextPunchAttack;

		/**
		 * \brief ���� �� �� ���ȳ���. �ִϸ��̼� �޺� ���̺��� �ΰ� �����ϰڽ��ϴ�.
		 */
		int _attackCount;






		/**
		 * \brief ĵ�� �������� �����մϴ�.
		 */
		void StartCancleFrame();

		/**
		 * \brief ĵ�� �������� �����մϴ�.
		 */
		void EndCancleFrame();

		/**
		 * \brief ��� ���� ������ �ִϸ��̼��� ����� �� �߻��ϴ� �̺�Ʈ. ���� ���ǿ� ����
		 * ���� ���� �ִϸ��̼��� ����Ѵ�.
		 */
		void EndAnimation();


		void SwordAttackStart();

		void SwordAttackEnd();

		void SwordFourthHitFrame();

		/**
		 * \brief ������ ����˴ϴ�. ������ ����� �����մϴ�.
		 */
		void EndAttack();

	public:
		virtual void OnStateEnter(float deltaTime) override;

		virtual void OnStateStay(float deltaTime) override;

		virtual void OnStateExit(float deltaTime) override;
	};
}