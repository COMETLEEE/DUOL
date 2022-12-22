#include "DUOLGame/ProtoType/Scripts/CharacterController.h"

#include "DUOLGameEngine/Manager/InputManager.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"

namespace DUOLGame
{
	CharacterController::CharacterController(const std::weak_ptr<GameObject>& owner) :
		MonoBehaviourBase(owner)
	{

	}

	CharacterController::~CharacterController()
	{

	}

	void CharacterController::OnUpdate(float deltaTime)
	{

	}
}