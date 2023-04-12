#pragma once
#include "PlayerStateBase.h"

#include "DUOLClient/Util/BinaryTree.h"
#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorController.h"

namespace DUOLClient
{
	constexpr DUOLMath::Vector3 SWORD_FOURTH_HIT_BOX = { 6.f, 3.f, 3.f };

	constexpr float SWORD_FOURTH_HIT_RANGE = 3.f;

	/**
	 * \brief 플레이어 공격 유형
	 */
	enum class Player_AttackType
	{
		SWORD
		, FIST
	};

	/**
	 * \brief 플레이어 기본 공격 데이터
	 */
	struct Player_NormalAttack
	{
		Player_AttackType _attackType;

		// 애니메이터 파라미터 이름 / 애니메이터 파라미터 타입 / 데이터 값
		std::vector<std::tuple<DUOLCommon::tstring, DUOLGameEngine::AnimatorControllerParameterType, std::any>> _animatorParameterTable;

		// 플레이어 중심으로부터 Local 방향 기준 Forward, Right, Up Offset
		DUOLMath::Vector3 _hitCenterOffset;

		// 공격 파장의 반지름
		float _hitRadius;

		// 공격 파장의 최대 거리
		float _hitMaxDistance;

		// 충격파 계열 공격이라면 .. 필요한 옵션이 뭐가 있을까 속도 ?
	};

	class PlayerState_Attack : public DUOLClient::PlayerStateBase
	{
	public:
		PlayerState_Attack(DUOLClient::Player* player);

		virtual ~PlayerState_Attack();

	private:
		/**
		 * \brief 모든 기본 검 공격 콤보의 트리
		 */
		BinaryTree<Player_NormalAttack> _swordComboTree;

		/**
		 * \brief 모든 기본 주먹 공격 콤보의 배열
		 */
		BinaryTree<Player_NormalAttack> _fistComboTree;

		/**
		 * \brief 현재 플레이어가 진행 중인 콤보 노드
		 */
		BinaryTree<Player_NormalAttack>* _currentComboTreeNode;

		/**
		 * \brief 다음 콤보 노드
		 */
		BinaryTree<Player_NormalAttack>* _nextComboTreeNode;

		/**
		 * \brief 캔슬 프레임 중인지 여부
		 */
		bool _isInCancle;

		/**
		 * \brief 캔슬 프레임 구간을 시작합니다.
		 */
		void StartCancleFrame();

		/**
		 * \brief 캔슬 프레임 구간을 종료합니다.
		 */
		void EndCancleFrame();

		/**
		 * \brief 캔슬 프레임이 끝날 때, 다음 콤보로의 전환을 체크합니다.
		 */
		void CheckCanEnterNextAttack();

		/**
		 * \brief TODO : 기획자의 데이터 문서를 받아서 콤보 트리를 빌드합니다.
		 */
		void BuildComboTree();

		/**
		 * \brief 현재 공격 스테이트에 맞게 애니메이션 파라미터 등을 전환합니다.
		 */
		void SettingCurrentComboNodeState();

		/**
		 * \brief 공격이 종료됩니다. 마지막 페이즈를 진행합니다.
		 */
		void EndAttack();

#pragma region 오버랩 계열 공격 함수
		/**
		 * \brief 마지막 일격을 제외한 피스트 공격 정의
		 */
		void FistHit();

		/**
		 * \brief 충격파 계열 공격 정의 (모든 마지막 일격)
		 */
		void WaveHit();
#pragma endregion

		DUOLGameEngine::CoroutineHandler SetPostDelay(float delayTime);

	public:
		virtual void OnStateEnter(float deltaTime) override;

		virtual void OnStateStay(float deltaTime) override;

		virtual void OnStateExit(float deltaTime) override;
	};
}