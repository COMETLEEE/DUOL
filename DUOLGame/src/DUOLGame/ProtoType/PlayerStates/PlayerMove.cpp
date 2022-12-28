#include "DUOLGame/ProtoType/PlayerStates/PlayerMove.h"

#include "DUOLGameEngine/Manager/InputManager.h"
#include "DUOLGameEngine/Manager/ResourceManager.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"

#include "DUOLCommon/LogHelper.h"

namespace DUOLGame
{
	PlayerMove::PlayerMove() :
		ActionNode(_T("PlayerMove"))
	{

	}

	PlayerMove::~PlayerMove()
	{

	}

	NodeState PlayerMove::Tick()
	{
		auto player = GetDataFromBlackBoard<GameObject*>(_T("Player"));
		auto animator = player->GetComponent<Animator>();
		auto cameraController = GetDataFromBlackBoard<GameObject*>(_T("CameraController"))->GetTransform();

		DUOLMath::Vector3 force = DUOLMath::Vector3::Zero;
		DUOLMath::Vector3 beforeVelocity = player->GetComponent<Rigidbody>()->GetLinearVelocity();

		if (InputManager::GetInstance()->GetKeyPressed(KeyCode::W) == true)
			force += cameraController->GetLook();

		if (InputManager::GetInstance()->GetKeyPressed(KeyCode::S) == true)
			force -= cameraController->GetLook();

		if (InputManager::GetInstance()->GetKeyPressed(KeyCode::A) == true)
			force -= cameraController->GetRight();

		if (InputManager::GetInstance()->GetKeyPressed(KeyCode::D) == true)
			force += cameraController->GetRight();

		if (force == DUOLMath::Vector3::Zero)
		{
			player->GetComponent<Rigidbody>()->SetLinearVelocity({ 0.0f, beforeVelocity.y, 0.0f });

			return NodeState::FAILURE;
		}

		force.y = 0.0f;

		force.Normalize();

		auto moveSpeed = GetDataFromBlackBoard<float>(_T("MoveSpeed"));

		if (force != player->GetTransform()->GetLook())
		{
			auto originControllerTM = cameraController->GetTransform()->GetWorldMatrix();

			cameraController->GetTransform()->LookAt(cameraController->GetTransform()->GetWorldPosition() + force);

			//auto controllerRotation = cameraController->GetTransform()->GetWorldRotation();
			//
			//player->GetTransform()->SetRotation(controllerRotation);

			//player->GetTransform()->LookAt(player->GetTransform()->GetWorldPosition() + force);

			cameraController->GetTransform()->SetWorldTM(originControllerTM);
		}

		player->GetComponent<Rigidbody>()->SetLinearVelocity(force * moveSpeed);

		static float coolTime = 0.0f;

		if (coolTime <= 0.0f)
		{
			coolTime = 1.0f;

			DUOL_INFO("Player Move : {0}, {1}, {2}", force.x * moveSpeed, force.y * moveSpeed, force.z * moveSpeed);

			//auto playerRotation = player->GetTransform()->GetWorldRotation();

			//DUOL_INFO("Player Rotation : {0}, {1}, {2}", playerRotation.x, playerRotation.y, playerRotation.z);

			//DUOL_INFO("Controller Rotation : {0}, {1}, {2}", controllerRotation.x, controllerRotation.y, controllerRotation.z);
		
			//auto changedRotation = cameraController->GetTransform()->GetLocalRotation();

			//DUOL_INFO("Changed Rotation : {0}, {1}, {2}", changedRotation.x, changedRotation.y, changedRotation.z);
		}
		else
		{
			auto deltaTime = GetDataFromBlackBoard<float>(_T("DeltaTime"));

			coolTime -= deltaTime;
		}

		animator->SetAnimationClip(ResourceManager::GetInstance()->GetAnimationClip(TEXT("Run")));

		return NodeState::SUCCESS;
	}

	void PlayerMove::Stop()
	{

	}
}