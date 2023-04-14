#pragma once
#include "PlayerStateBase.h"

namespace DUOLClient
{
	/**
	 * \brief �°� �ִ� ������Ʈ. ���� ����ġ�� ���� ��ġ�� �����ϸ� �ٿ ���ϴ�.
	 */
	class PlayerState_Hit : public DUOLClient::PlayerStateBase
	{
	public:
		PlayerState_Hit(DUOLClient::Player* player);

		virtual ~PlayerState_Hit();

	private:
		/**
		 * \brief ��Ʈ �ִϸ��̼� ���� �Ķ����
		 */
		std::vector<DUOLCommon::tstring> _hitAnimParameters;

		/**
		 * \brief ���� ���� ���� Ÿ��
		 */
		AttackType _currentAttackType;

		/**
		 * \brief ������ �ߺ��Ǿ�����
		 */
		bool _isAccumulatedHit;

		/**
		 * \brief �ߺ� ���̶�� ���� ��� ���� �ε����� ?
		 */
		int _currentAnimIndex;

	private:
		virtual void OnStateEnter(float deltaTime) override;

		virtual void OnStateStay(float deltaTime) override;

		virtual void OnStateExit(float deltaTime) override;

		void StartHit();

		void StartHeavyHit();

		void EndHit();

		DUOLGameEngine::CoroutineHandler ResetDownPoint();

		std::shared_ptr<DUOLGameEngine::Coroutine> _resetDownPoint;

	public:
		/**
		 * \brief ���� Ÿ���� �����մϴ�.
		 * \param attackType ���� ����
		 */
		void SetCurrentAttackType(AttackType attackType);

		/**
		 * \brief �ߺ� ��Ʈ ó���� ����
		 */
		void AccumulateHit();
	};
}