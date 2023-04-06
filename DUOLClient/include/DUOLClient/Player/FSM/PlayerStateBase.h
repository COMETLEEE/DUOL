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
		 * \brief  �÷��̾ �̵�, �뽬, ������ ������ �� �ٶ󺸵��� �䱸�Ǵ� ������ �ǹ��մϴ�.
		 */
		DUOLMath::Vector3 _desiredLook;

		// Player �� �ٶ󺸵��� �䱸�Ǵ� ���� ������ ���մϴ�.
		void LookDirectionUpdate();

		// ���� �Է¿� ���� �ִϸ����� ���� �Ķ���͸� �����մϴ�.
		void SetAnimatorDirectionParameter();

		/**
		 * \brief ȭ�� �� ��������� ���̸� ��Ƽ� ���� �༮�� ��, ���� ����� �༮�� �����մϴ�.
		 */
		void FindLockOnTarget();

		// Can Lock on ?
		bool LockOnCheck();

		// To Die ?
		bool DieCheck();

		// Can Left Attack (�����)
		bool LeftAttackCheck();

		// Can Left Attack (�����)
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