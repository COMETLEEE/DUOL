#include "DUOLClient/Player/FSM/PlayerState_Jump.h"

#include "DUOLGameEngine/ECS/Component/Animator.h"
#include "DUOLGameEngine/ECS/Component/Transform.h"
#include "DUOLGameEngine/Manager/PhysicsManager.h"

DUOLClient::PlayerState_Jump::PlayerState_Jump(DUOLClient::Player* player) :
	PlayerStateBase(TEXT("PlayerState_Jump"), player)
{

}

DUOLClient::PlayerState_Jump::~PlayerState_Jump()
{

}

void DUOLClient::PlayerState_Jump::OnStateEnter(float deltaTime)
{
	_currentHoldTime = 0;
	_currentFlyTime = 0;
}

void DUOLClient::PlayerState_Jump::OnStateStay(float deltaTime)
{
	_currentHoldTime += deltaTime;
	_currentFlyTime += deltaTime;

	if (_player->_jumpHoldTime < _currentHoldTime)
	{
		_animator->SetBool(TEXT("IsFlying"), true);

		auto worldPos = _player->GetTransform()->GetWorldPosition();

		//체공시간절반쯤왔을때 착지 테스트를합니다.
		if (_currentFlyTime > (_player->_jumpFlyTime / 2.f))
		{
			DUOLPhysics::RaycastHit rayHitInfo;

			auto layernum = DUOLGameEngine::PhysicsManager::GetInstance()->GetLayerNumber(TEXT("Obstacle"));
			DUOLGameEngine::PhysicsManager::GetInstance()->Raycast(worldPos, DUOLMath::Vector3::Down, 50, layernum, rayHitInfo);

			if (rayHitInfo._isBlocking)
			{
				if (rayHitInfo._hitDistance < 0.3f)
				{
					_animator->SetBool(TEXT("IsJumping"), false);
					_animator->SetBool(TEXT("IsLanding"), true);
				}
			}
		}
	}
}

void DUOLClient::PlayerState_Jump::OnStateExit(float deltaTime)
{
	_currentHoldTime = 0;
	_currentFlyTime = 0;

	_animator->SetBool(TEXT("IsFlying"), false);
	_animator->SetBool(TEXT("IsLanding"), false);
}

