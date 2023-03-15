#include "DUOLClient/Player/FSM/PlayerState_Idle.h"

#include "DUOLGameEngine/ECS/Component/Transform.h"
#include "DUOLGameEngine/Manager/InputManager.h"
#include "DUOLMath/DUOLMath.h"

namespace DUOLClient
{
	PlayerState_Idle::PlayerState_Idle() :
		StateBase(TEXT("PlayerState_Idle"))
	{
	}

	PlayerState_Idle::~PlayerState_Idle()
	{
	}

	void PlayerState_Idle::LookDirectionUpdate()
	{
		const DUOLMath::Vector3& cameraDir = _cameraTransform->GetLook();

		// X 벡터
		float horizontal = DUOLGameEngine::InputManager::GetInstance()->GetAxis(DUOLGameEngine::InputAxis::Horizontal);

		// Z 벡터.
		float vertical = DUOLGameEngine::InputManager::GetInstance()->GetAxis(DUOLGameEngine::InputAxis::Vertical);

		DUOLMath::Vector3 playerLookInCamLocal = DUOLMath::Vector3{ horizontal, 0.f, vertical };
	}

	void PlayerState_Idle::OnStateEnter(float deltaTime)
	{
		// StateBase::OnStateEnter(deltaTime);
	}

	void PlayerState_Idle::OnStateStay(float deltaTime)
	{
		// StateBase::OnStateStay(deltaTime);

		// 입력을 감지해야겠지.
	}

	void PlayerState_Idle::OnStateExit(float deltaTime)
	{
		// StateBase::OnStateExit(deltaTime);
	}
}
