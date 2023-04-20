#pragma once
#include "DUOLGameEngine/FiniteStateMachine/StateBase.h"

#include "DUOLClient/Player/Player.h"
#include "DUOLPhysics/Util/PhysicsDataStructure.h"

namespace DUOLGameEngine
{
	class AnimatorController;
	class Rigidbody;
	class Transform;
	class Animator;
}

namespace DUOLClient
{
	/**
	 * \brief 모든 플레이어 스테이트의 기본 클래스
	 */
	class PlayerStateBase : public DUOLGameEngine::StateBase
	{
	public:
		PlayerStateBase(const DUOLCommon::tstring& stateName, DUOLClient::Player* player);

		virtual ~PlayerStateBase() = default;

	private:
		int _slopeLayer = 0;

		int _obstacleLayer = 0;

		int _slopeRaycastDistance = 2.f;

		float _maxSlopeAngle = DUOLMath::PI / 3;

	protected:
		DUOLGameEngine::Transform* _transform;

		DUOLGameEngine::Animator* _animator;

		DUOLGameEngine::Rigidbody* _rigidbody;

		DUOLGameEngine::Transform* _cameraTransform;

		DUOLClient::MainCameraController* _mainCamController;

		DUOLClient::Player* _player;

		DUOLPhysics::RaycastHit _slopeHit;

		/**
		 * \brief 중력에 의한 속도
		 */
		float _gravity = 0.f;

		void Initialize();

		/**
		 * \brief  플레이어가 이동, 대쉬, 공격을 시행할 때 바라보도록 요구되는 방향을 의미합니다.
		 */
		DUOLMath::Vector3 _desiredLook;

		// 중력을 계산합니다.
		void CalculateGravity(float fixedTimeStep);

		// Player 가 바라보도록 요구되는 보는 방향을 정합니다.
		void LookDirectionUpdate();

		// 현재 입력에 따라서 애니메이터 방향 파라미터를 변경합니다.
		void SetAnimatorDirectionParameter();
		
		void FindLockOnTarget();

		// Can Lock on ?
		bool LockOnCheck();

		// Can Left Attack (약공격)
		bool SwordAttackCheck();

		// Can Left Attack (약공격)
		bool FistAttackCheck();

		bool OverdriveSwordCheck();

		bool OverdriveFistCheck();

		// Can Move ?
		bool MoveCheck();

		// Can Run ?
		bool RunCheck();

		// Can Dash ?
		bool DashCheck();

		// Upper slope ?
		bool SlopeCheck();

		// Upper slope ? (광역탐지)
		bool SlopeAreaCheck();

		// Upper Groudn ? (밟고 올라설 수 있는 위치에 있는가요)
		bool GroundCheck();

	protected:
		virtual void OnStateStayFixed(float fixedTimeStep) override;

#pragma region FRIEND_CLASS
		friend class Player;
#pragma endregion
	};
}