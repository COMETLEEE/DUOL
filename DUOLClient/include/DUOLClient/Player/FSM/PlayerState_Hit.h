#pragma once
#include "PlayerStateBase.h"

namespace DUOLClient
{
	constexpr float DOWN_POINT_PER_ATTACK = 3.f;

	constexpr float MAX_DOWN_POINT = 21.f;

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
		 * \brief ���� ����ġ
		 */
		float _downPoint;

		/**
		 * \brief ���� ���� ���� Ÿ��
		 */
		AttackType _currentAttackType;

		/**
		 * \brief ������ �ߺ��Ǿ�����
		 */
		bool _isAccumulatedHit;

	private:
		virtual void OnStateEnter(float deltaTime) override;

		virtual void OnStateStay(float deltaTime) override;

		virtual void OnStateExit(float deltaTime) override;

		void StartHit1Animation();

		void StartHit2Animation();

		void StartHit3Animation();

		void StartHeavyHitAnimation();

	public:
		/**
		 * \brief ���� Ÿ���� �����մϴ�.
		 * \param attackType ���� ����
		 */
		void SetCurrentAttackType(AttackType attackType);

		/**
		 * \brief �ߺ� ��Ʈ ó��
		 */
		void AccumulateHit();
	};
}