#include "DUOLGameEngine/ECS/Component/TPFController.h"

#include "DUOLGameEngine/ECS/Component/Transform.h"
#include "DUOLGameEngine/Manager/InputManager.h"

namespace DUOLGameEngine
{
	TPFController::TPFController(const std::weak_ptr<DUOLGameEngine::GameObject> owner, const DUOLCommon::tstring& name) :
		DUOLGameEngine::BehaviourBase(owner, name)
		, _moveSpeed(10.f)
		, _prevMousePosition(Vector2::Zero)
	{

	}

	TPFController::~TPFController()
	{

	}

	void TPFController::OnLateUpdate(float deltaTime)
	{
		const std::shared_ptr<DUOLGameEngine::Transform> transform = GetTransform();

		const Vector3& look = transform->GetLook();
		const Vector3& up = transform->GetUp();
		const Vector3& right = transform->GetRight();

		if (InputManager::GetInstance()->GetKeyPressed(KeyCode::W))
			transform->Translate(Vector3::Forward * deltaTime * _moveSpeed);

		if (InputManager::GetInstance()->GetKeyPressed(KeyCode::A))
			transform->Translate(Vector3::Left * deltaTime * _moveSpeed);

		if (InputManager::GetInstance()->GetKeyPressed(KeyCode::S))
			transform->Translate(Vector3::Backward * deltaTime * _moveSpeed);

		if (InputManager::GetInstance()->GetKeyPressed(KeyCode::D))
			transform->Translate(Vector3::Right * deltaTime * _moveSpeed);

		if (InputManager::GetInstance()->GetKeyPressed(KeyCode::Q))
			transform->Translate(Vector3::Down * deltaTime * _moveSpeed);

		if (InputManager::GetInstance()->GetKeyPressed(KeyCode::E))
			transform->Translate(Vector3::Up * deltaTime * _moveSpeed);

		const Vector2& currMousePosition = InputManager::GetInstance()->GetMousePosition();

		if (InputManager::GetInstance()->GetMouseButtonPressed(MouseCode::Right))
		{
			// 여기서 각각 X : World Up / Y : World Right 만큼 회전시킵니다.
			const Vector2 deltaMouseMove = 0.15f * (_prevMousePosition - currMousePosition);

			const Vector3 worldRot = Vector3(deltaMouseMove.y, deltaMouseMove.x, 0.f);

			transform->Rotate(worldRot, Space::World);
		}
		
		_prevMousePosition = currMousePosition;
	}
}
