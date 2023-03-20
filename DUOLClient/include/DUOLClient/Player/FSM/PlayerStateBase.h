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
	 * \brief ��� �÷��̾� ������Ʈ�� �⺻ Ŭ����
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

		void Initialize(Player* player);

		/**
		 * \brief  �÷��̾ �̵�, �뽬, ������ ������ �� �ٶ󺸵��� �䱸�Ǵ� ������ �ǹ��մϴ�.
		 */
		DUOLMath::Vector3 _desiredLook;

		// Player �� �ٶ󺸵��� �䱸�Ǵ� ���� ������ ���մϴ�.
		void LookDirectionUpdate();

		// To Die ?
		bool DieCheck();

		// Can Attack ?
		bool AttackCheck();

		// Can Move ?
		bool MoveCheck();

		friend class Player;
	};
}