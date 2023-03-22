#include "DUOLClient/Player/FSM/PlayerStateBase.h"

#include "DUOLClient/Camera/MainCameraController.h"
#include "DUOLGameEngine/Manager/InputManager.h"
#include "DUOLGameEngine/Manager/PhysicsManager.h"

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
		// Lock - On 상태에서 찾으려고 하면 .. 그냥 기존 락온을 취소시킨다.
		if (_player->_isLockOnMode)
		{
			_mainCamController->SetViewTransform(nullptr);

			_player->_isLockOnMode = false;
		}

		DUOLPhysics::RaycastHit hit;

		std::vector<DUOLPhysics::RaycastHit> hits;

		// Lock on 설정 가능한 최대 거리
		static float lockOnDistance = 50.f;

		// Look on 설정 구형 범위
		static float lockOnRadius = 3.f;

		const DUOLMath::Vector3& direction = _cameraTransform->GetLook();

		const DUOLGameEngine::Vector3& start = _transform->GetWorldPosition() + (lockOnRadius + 1.f) * direction;

		//if (DUOLGameEngine::PhysicsManager::GetInstance()->Spherecast(start, direction, lockOnRadius, lockOnDistance, hit))
		//{
		//	DUOLGameEngine::GameObject* lockedObject = 	reinterpret_cast<DUOLGameEngine::GameObject*>(hit._userData);

		//	// 락온 가능한 대상을 찾았습니다.
		//	if (lockedObject->GetTag() == TEXT("EliteMonster") || lockedObject->GetTag() == TEXT("BossMonster"))
		//	{
		//		// 메인 카메라 Lock on state.
		//		_mainCamController->SetViewTransform(lockedObject->GetTransform());

		//		_player->_isLockOnMode = true;

		//		return;
		//	}
		//}

		if (DUOLGameEngine::PhysicsManager::GetInstance()->SpherecastAll(start, direction, lockOnRadius, lockOnDistance, hits))
		{
			for (auto hited : hits)
			{
				DUOLGameEngine::GameObject* lockedObject = reinterpret_cast<DUOLGameEngine::GameObject*>(hited._userData);

				// 락온 가능한 대상을 찾았습니다.
				if (lockedObject->GetTag() == TEXT("EliteMonster") || lockedObject->GetTag() == TEXT("BossMonster"))
				{
					// 메인 카메라 Lock on state.
					_mainCamController->SetViewTransform(lockedObject->GetTransform());

					_player->_isLockOnMode = true;

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

		// X 벡터
		float horizontal = DUOLGameEngine::InputManager::GetInstance()->GetAxis(DUOLGameEngine::InputAxis::Horizontal);

		// Z 벡터.
		float vertical = DUOLGameEngine::InputManager::GetInstance()->GetAxis(DUOLGameEngine::InputAxis::Vertical);

		DUOLMath::Vector3 playerLookInCamLocal = DUOLMath::Vector3{ horizontal, 0.f, vertical };

		_desiredLook = DUOLMath::Vector3::TransformNormal(playerLookInCamLocal, _cameraTransform->GetWorldMatrix());

		// y - value는 필요 없습니다.
		_desiredLook.y = 0.f;

		_desiredLook.Normalize(_desiredLook);
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
}