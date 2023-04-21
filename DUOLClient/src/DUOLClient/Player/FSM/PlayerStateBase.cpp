#include "DUOLClient/Player/FSM/PlayerStateBase.h"

#include "DUOLClient/Camera/MainCameraController.h"
#include "DUOLGameEngine/Manager/InputManager.h"
#include "DUOLGameEngine/Manager/PhysicsManager.h"

#include "DUOLGameEngine/ECS/Component/Animator.h"
#include "DUOLGameEngine/ECS/Component/Transform.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"
#include "DUOLMath/DUOLMath.h"

namespace DUOLClient
{
	PlayerStateBase::PlayerStateBase(const DUOLCommon::tstring& stateName, DUOLClient::Player* player) :
		StateBase(stateName)
		, _player(player)
	{
		Initialize();

		_slopeLayer = DUOLGameEngine::PhysicsManager::GetInstance()->GetLayerNumber(TEXT("Slope"));

		_obstacleLayer = DUOLGameEngine::PhysicsManager::GetInstance()->GetLayerNumber(TEXT("Obstacle"));
	}

	void PlayerStateBase::Initialize()
	{
		_transform = _player->_playerTransform;

		_animator = _player->_playerAnimator;

		_rigidbody = _player->_playerRigidbody;

		_cameraTransform = _player->_cameraTransform;

		_mainCamController = _player->_mainCamController;
	}

	void PlayerStateBase::FindLockOnTarget()
	{
		// Lock on ���¿��� ã������ �ϸ� .. �׳� ���� ������ ��ҽ�Ų��.
		if (_player->_isLockOnMode)
		{
			_mainCamController->SetViewTransform(nullptr);

			// Lock Off
			_animator->SetBool(TEXT("IsLockOn"), false);

			_player->_isLockOnMode = false;

			return;
		}

		std::vector<DUOLPhysics::RaycastHit> hits;

		// Look on ���� ���� ����
		static float lockOnRadius = 20.f;

		const DUOLMath::Vector3& direction = _cameraTransform->GetLook();

		if (DUOLGameEngine::PhysicsManager::GetInstance()->OverlapSphereAll(_transform->GetWorldPosition(), lockOnRadius, hits))
		{
			for (auto hited : hits)
			{
				DUOLGameEngine::GameObject* lockedObject = reinterpret_cast<DUOLGameEngine::GameObject*>(hited._userData);

				// ���� ������ ����� ã�ҽ��ϴ�.
				if (lockedObject->GetTag() == TEXT("LockOnTarget"))
				{
					// ���� ī�޶� Lock on target.
					auto lockOnTargetTransform = lockedObject->GetTransform();

					_mainCamController->SetViewTransform(lockOnTargetTransform);

					// �÷��̾ Lock on target�� �������.
					_player->_lockOnTargetTransform = lockOnTargetTransform;

					_player->_isLockOnMode = true;

					// �Ĵٺ����� ����.
					DUOLMath::Vector3 lockOnYZero = _player->_lockOnTargetTransform->GetWorldPosition();

					lockOnYZero.y = 0;

					_transform->LookAt(lockOnYZero, DUOLMath::Vector3::Up);

					// Lock on animation.
					_animator->SetBool(TEXT("IsLockOn"), true);

					return;
				}
			}
		}
	}

	bool PlayerStateBase::LockOnCheck()
	{
		return DUOLGameEngine::InputManager::GetInstance()->GetKeyDown(LOCKON_KEY) ? true : false;
	}

	void PlayerStateBase::CalculateGravity(float fixedTimeStep)
	{
		GroundCheck() ? _gravity = 0.f : _gravity += (-9.8f) * fixedTimeStep;
	}

	void PlayerStateBase::LookDirectionUpdate()
	{
		if (!MoveCheck())
			return;

		const DUOLMath::Vector3& cameraDir = _cameraTransform->GetLook();

		// X ����
		float horizontal = DUOLGameEngine::InputManager::GetInstance()->GetAxis(DUOLGameEngine::InputAxis::Horizontal);

		// Z ����.
		float vertical = DUOLGameEngine::InputManager::GetInstance()->GetAxis(DUOLGameEngine::InputAxis::Vertical);

		DUOLMath::Vector3 playerLookInCamLocal = DUOLMath::Vector3{ horizontal, 0.f, vertical };

		// ī�޶� ���ؿ��� ���� ���� ���͸� ���� �� .. ���� ���ʹ� ?
		_desiredLook = DUOLMath::Vector3::TransformNormal(playerLookInCamLocal, _cameraTransform->GetWorldMatrix());

		// y - value�� �ʿ� �����ϴ�.
		_desiredLook.y = 0.f;

		_desiredLook.Normalize(_desiredLook);

		// ���⼭ ���⿡ ���� �ִϸ����� ��Ʈ�ѷ� ���� .. ȣ���غ��� ..
 		SetAnimatorDirectionParameter();
	}

	void PlayerStateBase::SetAnimatorDirectionParameter()
	{
		// X ����
		float horizontal = DUOLGameEngine::InputManager::GetInstance()->GetAxis(DUOLGameEngine::InputAxis::Horizontal);

		// Z ����.
		float vertical = DUOLGameEngine::InputManager::GetInstance()->GetAxis(DUOLGameEngine::InputAxis::Vertical);

		if (horizontal > 0.f)
		{
			_animator->SetBool(TEXT("IsRight"), true);

			_animator->SetBool(TEXT("IsLeft"), false);
		}
		else if (horizontal < 0.f)
		{
			_animator->SetBool(TEXT("IsLeft"), true);

			_animator->SetBool(TEXT("IsRight"), false);
		}
		else
		{
			_animator->SetBool(TEXT("IsLeft"), false);

			_animator->SetBool(TEXT("IsRight"), false);
		}

		if (vertical > 0.f)
		{
			_animator->SetBool(TEXT("IsFront"), true);

			_animator->SetBool(TEXT("IsBack"), false);
		}
		else if (vertical < 0.f)
		{
			_animator->SetBool(TEXT("IsBack"), true);

			_animator->SetBool(TEXT("IsFront"), false);
		}
		else
		{
			_animator->SetBool(TEXT("IsFront"), false);

			_animator->SetBool(TEXT("IsBack"), false);
		}
	}

	bool PlayerStateBase::SwordAttackCheck()
	{
		return DUOLGameEngine::InputManager::GetInstance()->GetMouseButtonDown(SWORD_ATTACK_KEY) && _player->_canStartAttack ? true : false;
	}

	bool PlayerStateBase::FistAttackCheck()
	{
		return DUOLGameEngine::InputManager::GetInstance()->GetMouseButtonDown(FIST_ATTACK_KEY) && _player->_canStartAttack ? true : false;
	}

	bool PlayerStateBase::EnterOverdriveSwordCheck()
	{
		return _player->_currentOverdrivePoint >= 100.f && DUOLGameEngine::InputManager::GetInstance()->GetMouseButtonDown(SWORD_ATTACK_KEY);
	}

	bool PlayerStateBase::EnterOverdriveFistCheck()
	{
		return _player->_currentOverdrivePoint >= 100.f && DUOLGameEngine::InputManager::GetInstance()->GetMouseButtonDown(SWORD_ATTACK_KEY);
	}

	bool PlayerStateBase::MoveCheck()
	{
		if ((DUOLGameEngine::InputManager::GetInstance()->GetKeyPressed(LEFT_KEY) &&
			DUOLGameEngine::InputManager::GetInstance()->GetKeyPressed(RIGHT_KEY)) ||
			DUOLGameEngine::InputManager::GetInstance()->GetKeyPressed(UP_KEY) &&
			DUOLGameEngine::InputManager::GetInstance()->GetKeyPressed(DOWN_KEY))
			return false;

		if (DUOLGameEngine::InputManager::GetInstance()->GetKeyPressed(LEFT_KEY) ||
			DUOLGameEngine::InputManager::GetInstance()->GetKeyPressed(RIGHT_KEY) ||
			DUOLGameEngine::InputManager::GetInstance()->GetKeyPressed(UP_KEY) ||
			DUOLGameEngine::InputManager::GetInstance()->GetKeyPressed(DOWN_KEY) ||
			DUOLGameEngine::InputManager::GetInstance()->GetKeyDown(LEFT_KEY) ||
			DUOLGameEngine::InputManager::GetInstance()->GetKeyDown(RIGHT_KEY) ||
			DUOLGameEngine::InputManager::GetInstance()->GetKeyDown(UP_KEY) ||
			DUOLGameEngine::InputManager::GetInstance()->GetKeyDown(DOWN_KEY))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool PlayerStateBase::RunCheck()
	{
		if (DUOLGameEngine::InputManager::GetInstance()->GetKeyPressed(RUN_KEY))
			return true;
		else
			return false;
	}

	bool PlayerStateBase::DashCheck()
	{
		return DUOLGameEngine::InputManager::GetInstance()->GetKeyDown(DASH_KEY) ? true : false;
	}

	bool PlayerStateBase::SlopeCheck()
	{
		const DUOLMath::Vector3& playerPos = _transform->GetWorldPosition();

		// Layer Mask
		if (DUOLGameEngine::PhysicsManager::GetInstance()->Raycast(playerPos + DUOLMath::Vector3(0.f, 1.3f, 0.f), DUOLMath::Vector3::Down
			, _slopeRaycastDistance, _slopeLayer, _slopeHit))
		{
			// ������ �¾Ҵ�.
			float angle = std::acosf(DUOLMath::Vector3::Up.Dot(_slopeHit._hitNormal));

			return angle != 0.f && angle < _maxSlopeAngle;
		}

		return false;
	}

	bool PlayerStateBase::SlopeAreaCheck()
	{
		return DUOLGameEngine::PhysicsManager::GetInstance()->CheckBox(_player->_playerTransform->GetWorldPosition() + DUOLMath::Vector3::Down * 0.5f
			, DUOLMath::Vector3(0.5f, 0.3f, 0.5f), DUOLMath::Quaternion::Identity, _slopeLayer);
	}

	bool PlayerStateBase::GroundCheck()
	{
		const DUOLMath::Vector3& playerPos = _transform->GetWorldPosition();

		DUOLPhysics::RaycastHit hit;

		if (DUOLGameEngine::PhysicsManager::GetInstance()->Raycast(playerPos + DUOLMath::Vector3(0.f, 1.3f, 0.f), DUOLMath::Vector3::Down
			, _slopeRaycastDistance, _slopeLayer | _obstacleLayer, hit))
		{
			if (hit._hitDistance <= 1.4f)
				return true;
			else
				return false;
		}

		return false;
	}

	bool PlayerStateBase::InOverdriveSwordCheck()
	{
		return _player->_isOverdriveSwordMode;
	}

	bool PlayerStateBase::InOverdriveFistCheck()
	{
		return _player->_isOverdriveFistMode;
	}

	bool PlayerStateBase::InOverdriveCheck()
	{
		return _player->_isOverdriveSwordMode || _player->_isOverdriveFistMode;
	}

	void PlayerStateBase::OnStateStayFixed(float fixedTimeStep)
	{
		StateBase::OnStateStayFixed(fixedTimeStep);

		bool isSlopeArea = SlopeAreaCheck();

		bool isSlope = SlopeCheck();

		if (isSlopeArea && isSlope)
			_player->_playerRigidbody->SetUseGravity(false);
		else
			_player->_playerRigidbody->SetUseGravity(true);
	}
}