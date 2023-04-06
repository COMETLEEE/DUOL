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
		 * \brief 현재까지의 공격 커맨드들을 기록하는 리스트. 해당 리스트를 참조해서 다음 공격 애니메이션 및 로직을
		 * 재생합니다.
		 */
		std::vector<int> _currentAttackTypeList;

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
		 * \brief 좌클릭. 소드 모션.
		 */
		bool _nextSwordAttack;

		/**
		 * \brief 우클릭. 펀치 모션.
		 */
		bool _nextPunchAttack;
		



		/**
		 * \brief 캔슬 프레임 구간을 시작합니다.
		 */
		void StartCancleFrame();

		/**
		 * \brief 캔슬 프레임 구간을 종료합니다.
		 */
		void EndCancleFrame();

		/**
		 * \brief 모든 공격 종류의 애니메이션이 종료될 때 발생하는 이벤트. 현재 조건에 따라서
		 * 다음 공격 애니메이션을 재생한다. 각각 공격 애니메이션들 끝나는 시점 키에 박아놓으면 될듯 ..
		 */
		void EndAnimation();



#pragma region 공격 모션 및 로직 처리 함수들
		void SwordAttackStart();

		void SwordAttackEnd();

		/**
		 * \brief 소드 기본 콤보 4타. 충격파를 날린다.
		 */
		void SwordFourthHitFrame();

		/**
		 * \brief 공격이 종료됩니다. 마지막 페이즈를 진행합니다.
		 */
		void EndAttack();
#pragma endregion

	public:
		virtual void OnStateEnter(float deltaTime) override;

		virtual void OnStateStay(float deltaTime) override;

		virtual void OnStateExit(float deltaTime) override;
	};
}