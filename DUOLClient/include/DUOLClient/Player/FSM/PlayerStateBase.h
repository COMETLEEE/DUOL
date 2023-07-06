#pragma once
#include "DUOLGameEngine/FiniteStateMachine/StateBase.h"

#include "DUOLClient/Player/Player.h"
#include "DUOLPhysics/Util/PhysicsDataStructure.h"

namespace DUOLGameEngine
{
	class CapsuleCollider;
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

	private:
		int _slopeLayer = 0;

		int _obstacleLayer = 0;

		int _playerLayer = 0;

		int _playerDashLayer = 0;

		int _slopeRaycastDistance = 2.f;

		float _maxSlopeAngle = DUOLMath::PI / 3;

	protected:
		DUOLGameEngine::Transform* _transform;

		DUOLGameEngine::Animator* _animator;

		DUOLGameEngine::Rigidbody* _rigidbody;

		DUOLGameEngine::CapsuleCollider* _collider;

		DUOLGameEngine::Transform* _cameraTransform;

		DUOLClient::MainCameraController* _mainCamController;

		DUOLClient::Player* _player;

		DUOLPhysics::RaycastHit _slopeHit;

		/**
		 * \brief �߷¿� ���� �ӵ�
		 */
		float _gravity = 0.f;

		void Initialize();

		/**
		 * \brief  �÷��̾ �̵�, �뽬, ������ ������ �� �ٶ󺸵��� �䱸�Ǵ� ������ �ǹ��մϴ�.
		 */
		DUOLMath::Vector3 _desiredLook;

		// �߷��� ����մϴ�.
		void CalculateGravity(float fixedTimeStep);

		// Player �� �ٶ󺸵��� �䱸�Ǵ� ���� ������ ���մϴ�.
		void LookDirectionUpdate();

		// ���� �Է¿� ���� �ִϸ����� ���� �Ķ���͸� �����մϴ�.
		void SetAnimatorDirectionParameter();
		
		void FindLockOnTarget();

		// Can Lock on ?
		bool LockOnCheck();

		// Can Left Attack (�����)
		bool SwordAttackCheck();

		// Can Left Attack (�����)
		bool FistAttackCheck();

		bool UltimateCheck();

		bool EnterOverdriveSwordCheck();

		bool EnterOverdriveFistCheck();

		// Can Move ?
		bool MoveCheck();

		// Can Run ?
		bool RunCheck();

		// Can Dash ?
		bool DashCheck();

		// Upper slope ?
		bool SlopeCheck();

		// Upper slope ? (������ ����Ž��)
		bool SlopeAreaCheck();

		// Upper Groud ? (��� �ö� �� �ִ� ��ġ�� �ִ°���)
		bool GroundCheck();

		// In overdrive sword mode.
		bool InOverdriveSwordCheck();

		// In overdrive fist mode.
		bool InOverdriveFistCheck();

		// In overdrive mode.
		bool InOverdriveCheck();

	protected:
		virtual void OnStateStayFixed(float fixedTimeStep) override;
		
#pragma region FRIEND_CLASS
		friend class Player;
#pragma endregion
	};
}