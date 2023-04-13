#pragma once
#include "PlayerStateBase.h"

namespace DUOLClient
{
	constexpr float DOWN_POINT_PER_ATTACK = 3.f;

	constexpr float MAX_DOWN_POINT = 10000.f;

	/**
	 * \brief �°� �ִ� ������Ʈ. ���� ����ġ�� ���� ��ġ�� �����ϸ� �ٿ ���ϴ�.
	 */
	class PlayerState_Hit : public DUOLClient::PlayerStateBase
	{
	public:
		PlayerState_Hit(DUOLClient::Player* player);

		virtual ~PlayerState_Hit();

	private:
		std::vector<DUOLCommon::tstring> _hitAnimParameters;

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