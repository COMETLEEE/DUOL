#pragma once
#include "PlayerStateBase.h"

#include "DUOLClient/Util/BinaryTree.h"
#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorController.h"

namespace DUOLGameEngine
{
	class ParticleRenderer;
}

namespace DUOLClient
{
	constexpr DUOLMath::Vector3 SWORD_FOURTH_HIT_BOX = { 6.f, 3.f, 3.f };

	constexpr float SWORD_FOURTH_HIT_RANGE = 3.f;

	constexpr float OVERDRIVE_ATTACK_SPEED_SWORD = 1.1f;

	constexpr float OVERDRIVE_ATTACK_SPEED_FIST = 0.9f;


	/**
	 * \brief 플레이어 공격 유형
	 */
	enum class Player_AttackType
	{
		SWORD
		, FIST
		, SWORD_WAVE
		, FIST_WAVE
		, SWORD_AREA_WAVE
	};

	/**
	 * \brief 플레이어 기본 공격 데이터. 마지막 애니메이션들은 다 이펙트를 가지고 있다.
	 */
	struct Player_AttackData
	{
		Player_AttackType _attackType;

		// 애니메이터 파라미터 이름 / 애니메이터 파라미터 타입 / 데이터 값
		std::vector<std::tuple<DUOLCommon::tstring, DUOLGameEngine::AnimatorControllerParameterType, std::any>> _animatorParameterTable;

		// 플레이어 중심으로부터 Local 방향 기준 Forward, Right, Up Offset
		DUOLMath::Vector3 _hitCenterOffset;

		// 공격 반지름
		float _hitRadius;

		// 공격 최대 거리
		float _hitMaxDistance;

		// 웨이브 계열 공격의 속도
		DUOLMath::Vector3 _waveVelocity;

		// 웨이브 계열 공격의 지속 시간
		float _waveTime;

		// 웨이브 계열 공격의 시작 박스의 크기
		DUOLMath::Vector3 _startWaveBoxHalfExtents;

		// 웨이브 계열 공격의 마지막 박스의 크기
		DUOLMath::Vector3 _endWaveBoxHalfExtents;

		// 카메라 쉐이크 사용 여부
		bool _useCamShake = false;

		// 카메라 쉐이크 파워
		DUOLMath::Vector2 _shakePower = DUOLMath::Vector2::Zero;
	};

	class PlayerState_Attack : public DUOLClient::PlayerStateBase
	{
	public:
		PlayerState_Attack(DUOLClient::Player* player);

		virtual ~PlayerState_Attack();

	private:
		/**ㅉㅇㅇ
		 * \brief 모든 기본 검 공격 콤보의 트리
		 */
		BinaryTree<Player_AttackData> _swordComboTree;

		/**
		 * \brief 모든 기본 주먹 공격 콤보의 트리
		 */
		BinaryTree<Player_AttackData> _fistComboTree;

		/**
		 * \brief 모든 오버드라이브 상태 공격 콤보의 트리
		 */
		BinaryTree<Player_AttackData> _overdriveSwordComboTree;

		/**
		 * \brief 모든 오버드라이브 상태 공격 콤보의 트리
		 */
		BinaryTree<Player_AttackData> _overdriveFistComboTree;

		/**
		 * \brief 현재 플레이어가 진행 중인 콤보 노드
		 */
		BinaryTree<Player_AttackData>* _currentComboTreeNode;

		/**
		 * \brief 다음 콤보 노드
		 */
		BinaryTree<Player_AttackData>* _nextComboTreeNode;

		/**
		 * \brief 캔슬 프레임 중인지 여부
		 */
		bool _isInCancle;

		/**
		 * \brief 현재의 타격 사운드 이펙트 IDX
		 */
		PlayerSoundTable _currentAudioClip;

		DUOLGameEngine::ParticleRenderer* _leftFistFormAura;

		DUOLGameEngine::ParticleRenderer* _rightFistFormAura;

		DUOLGameEngine::ParticleRenderer* _normalSwordTrail;

		/**
		 * \brief 캔슬 프레임 구간을 시작합니다.
		 */
		void StartCancleFrame();

		/**
		 * \brief 재생 속도를 느리게 하는 프레임을 설정합니다.
		 */
		void StartSlowFrame();

		/**
		 * \brief 캔슬 프레임 구간을 종료합니다.
		 */
		void EndCancleFrame();

		/**
		 * \brief 기본 검 활성화
		 */
		void StartSwordAttackFrame();

		/**
		 * \brief 기본 검 비활성화
		 */
		void EndSwordAttackFrame();

		/**
		 * \brief 기본 검 트레일 이펙트 활성화
		 */
		void StartSwordTrailFrame();

		/**
		 * \brief 기본 검 트레일 이펙트 비활성화
		 */
		void EndSwordTrailFrame();

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

		/**
		 * \brief 오버드라이브 소드 막타 공격
		 */
		void AreaWaveHit();

		/**
		* \brief 오버드라이브 소드 스피드설정;
		*/
		void OverdriveSwordDefaultSpeed();

		/**
		* \brief 오버드라이브 소드 스피드설정;
		*/
		void OverdriveFistDefaultSpeed();

#pragma endregion

		// 후딜 
		DUOLGameEngine::CoroutineHandler SetPostDelay(float delayTime);

		// 막타 끝난 후 대쉬하지 못 하도록 막기
		DUOLGameEngine::CoroutineHandler SetCannotDash();

		DUOLGameEngine::CoroutineHandler LaunchWave();

		DUOLGameEngine::CoroutineHandler LaunchAreaWave();

	public:
		virtual void OnStateEnter(float deltaTime) override;

		virtual void OnStateStay(float deltaTime) override;

		virtual void OnStateExit(float deltaTime) override;
	};
}
