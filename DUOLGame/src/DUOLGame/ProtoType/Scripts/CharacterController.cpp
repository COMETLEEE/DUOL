#include "DUOLGame/ProtoType/Scripts/CharacterController.h"

#include "DUOLGameEngine/Manager/InputManager.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"

namespace DUOLGame
{
	CharacterController::CharacterController(const std::weak_ptr<GameObject>& owner) :
		MonoBehaviourBase(owner)
		, _camera(nullptr)
		, _prevMousePosition(Vector2::Zero)
	{

	}

	CharacterController::~CharacterController()
	{

	}

	void CharacterController::OnStart()
	{
		auto children = GetTransform()->GetChildren();

		for (auto child : children)
		{
			_camera = child->GetGameObject()->GetComponent<Camera>();

			if (_camera != nullptr)
				break;
		}
	}

	void CharacterController::OnLateUpdate(float deltaTime)
	{
		Transform* transform = GetTransform();

		auto children = transform->GetChildren();

		const Vector2& currMousePosition = InputManager::GetInstance()->GetMousePosition();

		if (InputManager::GetInstance()->GetMouseButtonPressed(MouseCode::Right))
		{
			const Vector2 deltaMouseMove = 0.15f * (_prevMousePosition - currMousePosition);

			transform->Rotate(Vector3::Up, -deltaMouseMove.x, Space::World);

			transform->Rotate(Vector3::Right, -deltaMouseMove.y);
		}
		else
		{
			const Vector2 deltaMouseMove = 0.15f * (_prevMousePosition - currMousePosition);

			transform->Rotate(Vector3::Up, -deltaMouseMove.x, Space::World);
		}

		_prevMousePosition = currMousePosition;
	}
}