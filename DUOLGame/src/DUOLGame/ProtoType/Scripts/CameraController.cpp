#include "DUOLGame/ProtoType/Scripts/CameraController.h"

#include "DUOLGameEngine/Manager/InputManager.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"

namespace DUOLGame
{
	CameraController::CameraController(const std::weak_ptr<GameObject>& owner) :
		MonoBehaviourBase(owner)
		, _cameraController(nullptr)
		, _prevMousePosition(Vector2::Zero)
	{

	}

	CameraController::~CameraController()
	{

	}

	void CameraController::OnStart()
	{
		auto children = GetTransform()->GetChildren();

		for (auto child : children)
		{
			auto childName = child->GetGameObject()->GetName();

			if (childName.compare(_T("CameraController")) == 0)
			{
				_cameraController = child->GetGameObject();

				break;
			}
		}
	}

	void CameraController::OnLateUpdate(float deltaTime)
	{
		const Vector2& currMousePosition = InputManager::GetInstance()->GetMousePosition();

		if (InputManager::GetInstance()->GetMouseButtonPressed(MouseCode::Right))
		{
			const Vector2 deltaMouseMove = 0.15f * (_prevMousePosition - currMousePosition);

			_cameraController->GetTransform()->Rotate(Vector3::Up, -deltaMouseMove.x, Space::World);

			_cameraController->GetTransform()->Rotate(Vector3::Right, -deltaMouseMove.y);
		}
		else
		{
			const Vector2 deltaMouseMove = 0.15f * (_prevMousePosition - currMousePosition);

			_cameraController->GetTransform()->Rotate(Vector3::Up, -deltaMouseMove.x, Space::World);

			//InputManager::GetInstance()->SetMousePositionToCenter();
		}

		_prevMousePosition = InputManager::GetInstance()->GetMousePosition();
	}

	GameObject* CameraController::GetCameraController() const
	{
		return _cameraController;
	}
}