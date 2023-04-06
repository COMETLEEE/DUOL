#pragma once
#include "DUOLGameEngine/FiniteStateMachine/StateBase.h"

#include "DUOLClient/Player/Player.h"

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

	protected:
		DUOLGameEngine::Transform* _transform;

		DUOLGameEngine::Animator* _animator;

		DUOLGameEngine::Rigidbody* _rigidbody;

		DUOLGameEngine::Transform* _cameraTransform;

		DUOLClient::MainCameraController* _mainCamController;

		DUOLClient::Player* _player;

		void Initialize();

		/**
		 * \brief  플레이어가 이동, 대쉬, 공격을 시행할 때 바라보도록 요구되는 방향을 의미합니다.
		 */
		DUOLMath::Vector3 _desiredLook;

		// Player 가 바라보도록 요구되는 보는 방향을 정합니다.
		void LookDirectionUpdate();

		// 현재 입력에 따라서 애니메이터 방향 파라미터를 변경합니다.
		void SetAnimatorDirectionParameter();

		/**
		 * \brief 화면 상에 방사형으로 레이를 쏘아서 맞은 녀석들 중, 가장 가까운 녀석을 락온합니다.
		 */
		void FindLockOnTarget();

		// Can Lock on ?
		bool LockOnCheck();

		// To Die ?
		bool DieCheck();

		// Can Left Attack (약공격)
		bool LeftAttackCheck();

		// Can Left Attack (약공격)
		bool RightAttackCheck();

		// Can Move ?
		bool MoveCheck();

		// Can Run ?
		bool RunCheck();

		// Can Dash ?
		bool DashCheck();

#pragma region
		friend class Player;
#pragma endregion
	};
}