#include "DUOLClient/Player/FSM/PlayerStateBase.h"

#include "DUOLClient/Camera/MainCameraController.h"
#include "DUOLGameEngine/Manager/InputManager.h"
#include "DUOLGameEngine/Manager/PhysicsManager.h"

#include "DUOLGameEngine/ECS/Component/Animator.h"
#include "DUOLGameEngine/ECS/Component/Transform.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLMath/DUOLMath.h"

namespace DUOLClient
{
	PlayerStateBase::PlayerStateBase(const DUOLCommon::tstring& stateName) :
		StateBase(stateName)
	{

	}

	void PlayerStateBase::Initialize(Player* player)
	{
		_transform = player->_playerTransform;

		_animator = player->_playerAnimator;

		_rigidbody = player->_playerRigidbody;

		_cameraTransform = player->_cameraTransform;

		_mainCamController = player->_mainCamController;

		_player = player;
	}

	void PlayerStateBase::FindLockOnTarget()
	{
		// Lock - On ���¿��� ã������ �ϸ� .. �׳� ���� ������ ��ҽ�Ų��.
		if (_player->_isLockOnMode)
		{
			_mainCamController->SetViewTransform(nullptr);

			// Lock Off
			_animator->SetBool(TEXT("IsLockOn"), false);

			_player->_isLockOnMode = false;

			return;
		}

		std::vector<DUOLPhysics::RaycastHit> hits;

		// Lock on ���� ������ �ִ� �Ÿ�
		static float lockOnDistance = 50.f;

		// Look on ���� ���� ����
		static float lockOnRadius = 3.f;

		const DUOLMath::Vector3& direction = _cameraTransform->GetLook();

		const DUOLGameEngine::Vector3& start = _transform->GetWorldPosition() + (lockOnRadius + 1.f) * direction;

		if (DUOLGameEngine::PhysicsManager::GetInstance()->SpherecastAll(start, direction, lockOnRadius, lockOnDistance, hits))
		{
			for (auto hited : hits)
			{
				DUOLGameEngine::GameObject* lockedObject = reinterpret_cast<DUOLGameEngine::GameObject*>(hited._userData);

				// ���� ������ ����� ã�ҽ��ϴ�.
				if (lockedObject->GetTag() == TEXT("EliteMonster") || lockedObject->GetTag() == TEXT("BossMonster"))
				{
					// ���� ī�޶� Lock on target.
					auto lockOnTargetTransform = lockedObject->GetTransform();

					_mainCamController->SetViewTransform(lockOnTargetTransform);

					// �÷��̾ Lock on target�� �������.
					_player->_lockOnTargetTransform = lockOnTargetTransform;

					_player->_isLockOnMode = true;

					// Lock On
					_animator->SetBool(TEXT("IsLockOn"), true);

					return;
				}
			}
		}

		_mainCamController->SetViewTransform(nullptr);

		_player->_isLockOnMode = false;
	}

	bool PlayerStateBase::LockOnCheck()
	{
		if (DUOLGameEngine::InputManager::GetInstance()->GetKeyDown(LOCKON_KEY))
			return true;
		else
			return false;
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

		// ī�޶� �������� ȸ����Ų��.
		_desiredLook = DUOLMath::Vector3::TransformNormal(playerLookInCamLocal, _cameraTransform->GetWorldMatrix());

		// y - value�� �ʿ� �����ϴ�.
		_desiredLook.y = 0.f;

		_desiredLook.Normalize(_desiredLook);

		// ���⼭ ȣ���غ��� ..
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

	bool PlayerStateBase::DieCheck()
	{
		return _player->_hp <= 0.f ? true : false;
	}

	bool PlayerStateBase::AttackCheck()
	{
		return DUOLGameEngine::InputManager::GetInstance()->GetMouseButtonDown(ATTACK_KEY) ? true : false;
	}

	bool PlayerStateBase::MoveCheck()
	{
		if (DUOLGameEngine::InputManager::GetInstance()->GetKeyPressed(LEFT_KEY) ||
			DUOLGameEngine::InputManager::GetInstance()->GetKeyPressed(RIGHT_KEY) ||
			DUOLGameEngine::InputManager::GetInstance()->GetKeyPressed(UP_KEY) ||
			DUOLGameEngine::InputManager::GetInstance()->GetKeyPressed(DOWN_KEY))
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
}