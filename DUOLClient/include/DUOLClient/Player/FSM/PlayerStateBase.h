#pragma once
#include "DUOLGameEngine/FiniteStateMachine/StateBase.h"

#include "DUOLClient/Player/Player.h"

namespace DUOLGameEngine
{
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
		PlayerStateBase(const DUOLCommon::tstring& stateName);

		virtual ~PlayerStateBase() = default;

	protected:
		DUOLGameEngine::Transform* _transform;

		DUOLGameEngine::Animator* _animator;

		DUOLGameEngine::Rigidbody* _rigidbody;

		DUOLGameEngine::Transform* _cameraTransform;

		DUOLClient::Player* _player;

		/**
		 * \brief  플레이어가 이동, 대쉬, 공격을 시행할 때 바라보도록 요구되는 방향을 의미합니다.
		 */
		DUOLMath::Vector3 _desiredLook;

		// Player 가 바라보도록 요구되는 보는 방향을 정합니다.
		void LookDirectionUpdate();

		// To Die ?
		bool DieCheck();

		// Can Dash ?
		bool DashCheck();

		// Can Attack ?
		bool AttackCheck();

		// Can Move ?
		bool MoveCheck();

		friend class Player;
	};
}