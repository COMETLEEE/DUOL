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
		PlayerState_Attack(DUOLClient::Player* player);

		virtual ~PlayerState_Attack();

	private:
		/**
		 * \brief ��������� ���� Ŀ�ǵ���� ����ϴ� ����Ʈ. �ش� ����Ʈ�� �����ؼ� ���� ���� �ִϸ��̼� �� ������
		 * ����մϴ�.
		 */
		std::vector<int> _currentAttackTypeList;

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
		 * \brief ��Ŭ��. �ҵ� ���.
		 */
		bool _nextSwordAttack;

		/**
		 * \brief ��Ŭ��. ��ġ ���.
		 */
		bool _nextPunchAttack;
		



		/**
		 * \brief ĵ�� ������ ������ �����մϴ�.
		 */
		void StartCancleFrame();

		/**
		 * \brief ĵ�� ������ ������ �����մϴ�.
		 */
		void EndCancleFrame();

		/**
		 * \brief ��� ���� ������ �ִϸ��̼��� ����� �� �߻��ϴ� �̺�Ʈ. ���� ���ǿ� ����
		 * ���� ���� �ִϸ��̼��� ����Ѵ�. ���� ���� �ִϸ��̼ǵ� ������ ���� Ű�� �ھƳ����� �ɵ� ..
		 */
		void EndAnimation();



#pragma region ���� ��� �� ���� ó�� �Լ���
		void SwordAttackStart();

		void SwordAttackEnd();

		/**
		 * \brief �ҵ� �⺻ �޺� 4Ÿ. ����ĸ� ������.
		 */
		void SwordFourthHitFrame();

		/**
		 * \brief ������ ����˴ϴ�. ������ ����� �����մϴ�.
		 */
		void EndAttack();
#pragma endregion

	public:
		virtual void OnStateEnter(float deltaTime) override;

		virtual void OnStateStay(float deltaTime) override;

		virtual void OnStateExit(float deltaTime) override;
	};
}