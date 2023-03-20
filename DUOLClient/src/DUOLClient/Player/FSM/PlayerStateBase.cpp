#include "DUOLClient/Player/FSM/PlayerStateBase.h"

#include "DUOLGameEngine/ECS/Component/Transform.h"
#include "DUOLGameEngine/Manager/InputManager.h"
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

		_player = player;
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