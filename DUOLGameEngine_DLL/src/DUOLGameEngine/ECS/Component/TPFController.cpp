#include "DUOLGameEngine/ECS/Component/TPFController.h"

#include <rttr/registration>
#include <rttr/policy.h>

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Transform.h"
#include "DUOLGameEngine/Manager/InputManager.h"

using namespace rttr;

RTTR_PLUGIN_REGISTRATION
{
	rttr::registration::class_<DUOLGameEngine::TPFController>("TPFController")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	);
}

namespace DUOLGameEngine
{
	TPFController::TPFController(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		DUOLGameEngine::BehaviourBase(owner, name)
		, _moveSpeed(10.f)
	{

	}

	TPFController::TPFController() :
		DUOLGameEngine::BehaviourBase(nullptr, TEXT("TPFController"))
	{
	}

	TPFController::~TPFController()
	{
	}

	void TPFController::OnLateUpdate(float deltaTime)
	{
		Transform* transform = GetTransform();

		const Vector3& look = transform->GetLook();
		const Vector3& up = transform->GetUp();
		const Vector3& right = transform->GetRight();

		if (InputManager::GetInstance()->GetKeyPressed(KeyCode::LeftShift))
			_moveSpeed = 20.f;
		else if (InputManager::GetInstance()->GetKeyUp(KeyCode::LeftShift))
			_moveSpeed = 10.f;

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

		const Vector2& prevMousePosition = InputManager::GetInstance()->GetPrevMousePosition();

		const Vector2& currMousePosition = InputManager::GetInstance()->GetMousePosition();

		if (InputManager::GetInstance()->GetMouseButtonPressed(MouseCode::Right))
		{
			// 여기서 각각 X : World Up / Y : World Right 만큼 회전시킵니다.
			const Vector2 deltaMouseMove = 0.15f * (prevMousePosition - currMousePosition);

#pragma region AXIS_ANGLE_VER
			// Y world up rotate.
			transform->Rotate(Vector3::Up, -deltaMouseMove.x, Space::World);

			// X local Right rotate.
			transform->Rotate(Vector3::Right, -deltaMouseMove.y);
#pragma endregion

#pragma region EULER_ROTATION_VER
			//// Y world up rotate.
			// transform->Rotate(Vector3(0.f, -deltaMouseMove.x, 0.f), Space::World);

			//// X local Right rotate.
			// transform->Rotate(Vector3(-deltaMouseMove.y, 0.f, 0.f));
#pragma endregion
		}
	}
}
