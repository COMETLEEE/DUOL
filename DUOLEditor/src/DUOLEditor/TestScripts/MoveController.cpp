#include "DUOLEditor/TestScripts/MoveController.h"

#include "DUOLGameEngine/ECS/Component/Transform.h"
#include "DUOLGameEngine/Manager/InputManager.h"
#include "DUOLMath/DUOLMath.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"

#include "DUOLCommon/LogHelper.h"
#include "DUOLGameEngine/Manager/EventManager.h"

namespace DUOLEditor
{
	MoveController::MoveController(const std::weak_ptr<DUOLGameEngine::GameObject>& owner) :
		DUOLGameEngine::MonoBehaviourBase(owner)
		, _moveSpeed(15.f)
		, _isWorldMode(true)
	{
	}

	MoveController::~MoveController()
	{
	}

	void MoveController::OnLeftFoot()
	{
		DUOL_TRACE("OnLeftFoot ..!");
	}

	void MoveController::OnRightFoot()
	{
		DUOL_TRACE("OnRightFoot ..!");
	}

	void MoveController::OnAwake()
	{
		_animator = GetGameObject()->GetComponent<DUOLGameEngine::Animator>();

		AddEventFunction(TEXT("OnLeftFoot"), std::bind(&MoveController::OnLeftFoot, this));

		AddEventFunction(TEXT("OnRightFoot"), std::bind(&MoveController::OnRightFoot, this));
	}

	void MoveController::OnUpdate(float deltaTime)
	{
		static bool isMove = false;

		DUOLGameEngine::Transform* transform = GetTransform();

		if (DUOLGameEngine::InputManager::GetInstance()->GetKeyDown(DUOLGameEngine::KeyCode::V))
			_isWorldMode = !_isWorldMode;

		_moveSpeed = 3.f;

		if (_isWorldMode)
		{
			// World Translation
			if (DUOLGameEngine::InputManager::GetInstance()->GetKeyPressed(DUOLGameEngine::KeyCode::UpArrow))
			{
				transform->Translate(DUOLMath::Vector3::Forward * deltaTime * _moveSpeed, DUOLGameEngine::Space::World);

				isMove = true;

				_animator->SetBool(TEXT("TrueIsIdle"), false);
			}

			if (DUOLGameEngine::InputManager::GetInstance()->GetKeyPressed(DUOLGameEngine::KeyCode::LeftArrow))
			{
				transform->Translate(DUOLMath::Vector3::Left * deltaTime * _moveSpeed, DUOLGameEngine::Space::World);

				isMove = true;

				_animator->SetBool(TEXT("TrueIsIdle"), false);
			}

			if (DUOLGameEngine::InputManager::GetInstance()->GetKeyPressed(DUOLGameEngine::KeyCode::DownArrow))
			{
				transform->Translate(DUOLMath::Vector3::Backward * deltaTime * _moveSpeed, DUOLGameEngine::Space::World);

				isMove = true;

				_animator->SetBool(TEXT("TrueIsIdle"), false);
			}

			if (DUOLGameEngine::InputManager::GetInstance()->GetKeyPressed(DUOLGameEngine::KeyCode::RightArrow))
			{
				transform->Translate(DUOLMath::Vector3::Right * deltaTime * _moveSpeed, DUOLGameEngine::Space::World);
				isMove = true;

				_animator->SetBool(TEXT("TrueIsIdle"), false);
			}

			if (!isMove)
			{
				_animator->SetBool(TEXT("TrueIsIdle"), true);
			}

			isMove = false;

			if (DUOLGameEngine::InputManager::GetInstance()->GetKeyPressed(DUOLGameEngine::KeyCode::Z))
				transform->Translate(DUOLMath::Vector3::Down * deltaTime * _moveSpeed, DUOLGameEngine::Space::World);

			if (DUOLGameEngine::InputManager::GetInstance()->GetKeyPressed(DUOLGameEngine::KeyCode::C))
				transform->Translate(DUOLMath::Vector3::Up * deltaTime * _moveSpeed, DUOLGameEngine::Space::World);
		}
		else if (!_isWorldMode)
		{
			// Local Translation
			if (DUOLGameEngine::InputManager::GetInstance()->GetKeyPressed(DUOLGameEngine::KeyCode::UpArrow))
				transform->Translate(DUOLMath::Vector3::Forward * deltaTime * _moveSpeed);

			if (DUOLGameEngine::InputManager::GetInstance()->GetKeyPressed(DUOLGameEngine::KeyCode::LeftArrow))
				transform->Translate(DUOLMath::Vector3::Left * deltaTime * _moveSpeed);

			if (DUOLGameEngine::InputManager::GetInstance()->GetKeyPressed(DUOLGameEngine::KeyCode::DownArrow))
				transform->Translate(DUOLMath::Vector3::Backward * deltaTime * _moveSpeed);

			if (DUOLGameEngine::InputManager::GetInstance()->GetKeyPressed(DUOLGameEngine::KeyCode::RightArrow))
				transform->Translate(DUOLMath::Vector3::Right * deltaTime * _moveSpeed);

			if (DUOLGameEngine::InputManager::GetInstance()->GetKeyPressed(DUOLGameEngine::KeyCode::Z))
				transform->Translate(DUOLMath::Vector3::Down * deltaTime * _moveSpeed);

			if (DUOLGameEngine::InputManager::GetInstance()->GetKeyPressed(DUOLGameEngine::KeyCode::C))
				transform->Translate(DUOLMath::Vector3::Up * deltaTime * _moveSpeed);
		}

		// Rotation Test
		if (DUOLGameEngine::InputManager::GetInstance()->GetKeyPressed(DUOLGameEngine::KeyCode::X))
			transform->Rotate(DUOLMath::Vector3(0.f, 90.f, 0.f) * deltaTime);

		//// Animator Transition Test
		//if (DUOLGameEngine::InputManager::GetInstance()->GetKeyDown(DUOLGameEngine::KeyCode::V))
		//{
		//	_animator->SetBool(TEXT("TrueIsIdle"), true);
		//}
		//else if (DUOLGameEngine::InputManager::GetInstance()->GetKeyDown(DUOLGameEngine::KeyCode::B))
		//{
		//	_animator->SetBool(TEXT("TrueIsIdle"), false);
		//}
	}
}
