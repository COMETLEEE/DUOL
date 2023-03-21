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
		 * \brief ĵ�� �������� �����մϴ�.
		 */
		void StartCancleFrame();

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