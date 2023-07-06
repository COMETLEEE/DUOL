#include "DUOLClient/Player/FSM/PlayerState_Down.h"

#include "DUOLGameEngine/ECS/Component/Animator.h"
#include "DUOLGameEngine/ECS/Component/Transform.h"
#include "DUOLGameEngine/Manager/PhysicsManager.h"

namespace DUOLClient
{
	PlayerState_Down::PlayerState_Down(DUOLClient::Player* player) :
		PlayerStateBase(TEXT("PlayerState_Down"), player)
	{
#pragma region DOWN_EVENT
		_player->AddEventFunction(TEXT("GroundHitSound"), std::bind(&DUOLClient::PlayerState_Down::GroundHitSound, this));

		_player->AddEventFunction(TEXT("EndDown"), std::bind(&DUOLClient::PlayerState_Down::EndDown, this));

		_player->AddEventFunction(TEXT("EndDownup"), std::bind(&DUOLClient::PlayerState_Down::EndDownup, this));

		auto physManager = DUOLGameEngine::PhysicsManager::GetInstance();
		_groundLayer = physManager->GetLayerNumber(TEXT("Ground"));
		_obstacleLayer = physManager->GetLayerNumber(TEXT("Obstacle"));
		_slopeLayer = physManager->GetLayerNumber(TEXT("Slope"));

		_groundCheckLayer += _groundLayer + _obstacleLayer + _slopeLayer;

#pragma endregion
	}

	PlayerState_Down::~PlayerState_Down()
	{

	}

	void PlayerState_Down::GroundHitSound()
	{
		_player->PlaySoundClip(PlayerSoundTable::Hitting_Ground, false);
	}

	void PlayerState_Down::EndDown()
	{
		_animator->SetBool(TEXT("IsDown"), false);

		_animator->SetBool(TEXT("IsDownup"), true);
	}

	void PlayerState_Down::EndDownup()
	{
		_animator->SetBool(TEXT("IsDownup"), false);

		_stateMachine->TransitionTo(TEXT("PlayerState_Idle"), 0.f);
	}

	void PlayerState_Down::OnStateEnter(float deltaTime)
	{
		_isGroundSound = false;

		PlayerStateBase::OnStateEnter(deltaTime);

		// 다운 햇으니 다운 포인트 초기화
		_player->_currentDownPoint = 0.f;

		_animator->SetBool(TEXT("IsDown"), true);
	}

	void PlayerState_Down::OnStateStay(float deltaTime)
	{
		PlayerStateBase::OnStateStay(deltaTime);
	}

	void PlayerState_Down::OnStateStayFixed(float fixedTimeStep)
	{
		PlayerStateBase::OnStateStayFixed(fixedTimeStep);

		//바닥에 레이를 쏴서 일정거리에 들어오면 사운드를 플레이한다
		//if(!_isGroundSound)
		//{

		//	DUOLMath::Vector3 position = _player->GetTransform()->GetWorldPosition();
		//	float length = 0.5f;

		//	DUOLPhysics::RaycastHit outRaycastHit;

		//	DUOLGameEngine::PhysicsManager::GetInstance()->Raycast(position, DUOLMath::Vector3::Down, length, _groundCheckLayer, outRaycastHit);
		//	if(outRaycastHit._isBlocking)
		//	{
		//		_player->PlaySoundClip(PlayerSoundTable::Hitting_Ground, false);
		//		_isGroundSound = true;
		//	}
		//}
	}


	void PlayerState_Down::OnStateExit(float deltaTime)
	{
		PlayerStateBase::OnStateExit(deltaTime);
	}
}
