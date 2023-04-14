#pragma once
#include "PlayerStateBase.h"

namespace DUOLClient
{
	/**
	 * \brief 맞고 있는 스테이트. 스턴 축적치가 일정 수치에 도달하면 다운에 들어갑니다.
	 */
	class PlayerState_Hit : public DUOLClient::PlayerStateBase
	{
	public:
		PlayerState_Hit(DUOLClient::Player* player);

		virtual ~PlayerState_Hit();

	private:
		/**
		 * \brief 히트 애니메이션 조작 파라미터
		 */
		std::vector<DUOLCommon::tstring> _hitAnimParameters;

		/**
		 * \brief 현재 맞은 어택 타입
		 */
		AttackType _currentAttackType;

		/**
		 * \brief 공격이 중복되었나요
		 */
		bool _isAccumulatedHit;

		/**
		 * \brief 중복 중이라면 현재 재생 중인 인덱스는 ?
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
		 * \brief 공격 타입을 설정합니다.
		 * \param attackType 받은 공격
		 */
		void SetCurrentAttackType(AttackType attackType);

		/**
		 * \brief 중복 히트 처리를 위함
		 */
		void AccumulateHit();
	};
}