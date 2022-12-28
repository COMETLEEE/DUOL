#include "DUOLGame/ProtoType/Scripts/CameraController.h"

#include "DUOLGameEngine/Manager/InputManager.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"

namespace DUOLGame
{
	void AttachedState(Transform* controllerTransform, DUOLMath::Vector2& prevMousePosition)
	{
		const Vector2& currMousePosition = InputManager::GetInstance()->GetMousePosition();

		if (InputManager::GetInstance()->GetMouseButtonPressed(MouseCode::Right))
		{
			const Vector2 deltaMouseMove = 0.15f * (prevMousePosition - currMousePosition);

			controllerTransform->Rotate(Vector3::Up, -deltaMouseMove.x, Space::World);

			controllerTransform->Rotate(Vector3::Right, -deltaMouseMove.y);
		}
		else
		{
			const Vector2 deltaMouseMove = 0.15f * (prevMousePosition - currMousePosition);

			controllerTransform->Rotate(Vector3::Up, -deltaMouseMove.x, Space::World);

			//InputManager::GetInstance()->SetMousePositionToCenter();
		}

		prevMousePosition = InputManager::GetInstance()->GetMousePosition();
	}

	void FreeFlyState(Transform* controllerTransform, DUOLMath::Vector2& prevMousePosition)
	{
		auto children = controllerTransform->GetChildGameObjects();

		Transform* cameraTransform = nullptr;

		for (auto& child : children)
		{
			if (child->GetName().compare(_T("MainCamera")) == 0)
			{
				cameraTransform = child->GetTransform();

				break;
			}
		}

		if (cameraTransform == nullptr)
			return;

		const Vector2& currMousePosition = InputManager::GetInstance()->GetMousePosition();

		const Vector3& look = cameraTransform->GetLook();
		const Vector3& up = cameraTransform->GetUp();
		const Vector3& right = cameraTransform->GetRight();

		float moveSpeed = 5.0f;

		if (InputManager::GetInstance()->GetKeyPressed(KeyCode::W))
			cameraTransform->Translate(look * moveSpeed);

		if (InputManager::GetInstance()->GetKeyPressed(KeyCode::A))
			cameraTransform->Translate(-right * moveSpeed);

		if (InputManager::GetInstance()->GetKeyPressed(KeyCode::S))
			cameraTransform->Translate(-look * moveSpeed);

		if (InputManager::GetInstance()->GetKeyPressed(KeyCode::D))
			cameraTransform->Translate(right * moveSpeed);

		if (InputManager::GetInstance()->GetKeyPressed(KeyCode::Q))
			cameraTransform->Translate(-up * moveSpeed);

		if (InputManager::GetInstance()->GetKeyPressed(KeyCode::E))
			cameraTransform->Translate(up * moveSpeed);

		const Vector2 deltaMouseMove = 0.15f * (prevMousePosition - currMousePosition);

		cameraTransform->Rotate(Vector3::Up, -deltaMouseMove.x, Space::World);

		cameraTransform->Rotate(Vector3::Right, deltaMouseMove.y);

		//InputManager::GetInstance()->SetMousePositionToCenter();

		prevMousePosition = InputManager::GetInstance()->GetMousePosition();
	}

	CameraController::CameraController(const std::weak_ptr<GameObject>& owner) :
		MonoBehaviourBase(owner)
		, _cameraController(nullptr)
		, _prevMousePosition(Vector2::Zero)
		, _cameraUpdate(AttachedState)
		, _isAttached(true)
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
		_cameraUpdate(_cameraController->GetTransform(), _prevMousePosition);
	}

	GameObject* CameraController::GetCameraController() const
	{
		return _cameraController;
	}

	void CameraController::SwitchingCameraMode()
	{
		if (_isAttached == true)
			_cameraUpdate = FreeFlyState;
		else
			_cameraUpdate = AttachedState;

		_isAttached = !_isAttached;
	}
}